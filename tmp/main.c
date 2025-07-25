#define BUFFER_SIZE 16384
#define MAX_LANGS 14
#define MAX_PATH 10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

// 言語リスト（適当）
const char *langs[MAX_LANGS] =
{
	"en", "ja", "fr", "de", "es", "it", "ko", "zh-CN", "ru", "pt", "sus", "iu", "oc", "ar"
};

struct MemoryStruct
{
	char *memory;
	size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	char *ptr = realloc(mem->memory, mem->size + realsize + 1);
	if(ptr == NULL) return 0;  // out of memory

	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

char *send_http_request(const char *host, const char *request_path_and_query)
{
	CURL *curl;
	CURLcode res;
	struct MemoryStruct chunk = {0};

	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();

	if(!curl) return NULL;

	char url[2048];
	snprintf(url, sizeof(url), "https://%s%s", host, request_path_and_query);

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36");
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // リダイレクト追跡
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	curl_easy_setopt(curl, CURLOPT_CAINFO, "h:\\easyidec\\project\\translatest\\cacert.pem");

	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	if(res != CURLE_OK)
	{
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		free(chunk.memory);
		return NULL;
	}

	return chunk.memory;
}

// URLエンコード
void urlencode(char *dest, const char *src, size_t dest_size)
{
	const char *hex = "0123456789ABCDEF";
	size_t i = 0;

	while(*src && i + 3 < dest_size)
	{
		unsigned char c = (unsigned char)*src;
		if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~')
		{
			dest[i++] = c;
		}
		else
		{
			if(i + 3 >= dest_size) break;
			dest[i++] = '%';
			dest[i++] = hex[c >> 4];
			dest[i++] = hex[c & 15];
		}
		src++;
	}
	dest[i] = '\0';
}

// GETリクエスト生成
void make_request(char *dest, const char *text, const char *sl, const char *tl)
{
	char encoded[1024] = {0};
	urlencode(encoded, text, sizeof(encoded));

	// これは path + query 部分だけ作る
	sprintf(dest, "/?sl=%s&tl=%s&text=%s&op=translate", sl, tl, encoded);

	printf("[DEBUG] リクエストパス: %s\n", dest);
}

// 翻訳結果をHTMLから抜き出す（簡易版）
char *extract_translation(const char *html)
{
	printf("HTML:\n\n%s\n\n", html);
	const char *start = strstr(html, "<span class=\"HwtZe\"");
	if(!start) return NULL;
	printf("[DEBUG] span start found: %s\n", start);

	// タグの終端 '>' を探す
	start = strchr(start, '>');
	if(!start) return NULL;
	printf("[DEBUG] span start found: %s\n", start);

	start++; // '>' をスキップして中身へ

	const char *end = strstr(start, "</span>");
	if(!end) return NULL;

	int len = end - start;
	if(len <= 0) return NULL;

	char *result = malloc(len + 1);
	if(!result) return NULL;

	strncpy(result, start, len);
	result[len] = '\0';

	return result;
}

// 翻訳言語パスを生成（開始言語、終了言語、途中はランダム）
void choose_path(const char *start, const char *end, const char **path, int *count)
{
	int i, steps = 2 + rand() % 13; // 2?4ステップ
	path[0] = start;

	for(i = 1; i < steps; i++)
	{
		int r = rand() % MAX_LANGS;
		path[i] = langs[r];
	}

	path[steps] = end;
	*count = steps + 1;
}

int main(void)
{
	char request[2048];
	char *response, *translated;
	char input[512];
	const char *path[MAX_PATH];
	int i, steps;

	srand((unsigned int)time(NULL));

	printf("翻訳したい文字列を入力してください:\n> ");
	fgets(input, sizeof(input), stdin);
	input[strcspn(input, "\r\n")] = 0; // 改行除去

	printf("[DEBUG] 入力: \"%s\"\n", input);
	char encoded[1024] = {0};
	urlencode(encoded, input, sizeof(encoded));
	printf("[DEBUG] urlencode後: \"%s\"\n", encoded); // ← 追加


	char from[8], to[8];
	printf("Starting language code (ex.: en): ");
	scanf("%7s", from);
	printf("Ending language code (en: ja): ");
	scanf("%7s", to);

	choose_path(from, to, path, &steps);

	char *current_text = _strdup(input);

	for(i = 0; i < steps - 1; i++)
	{
		const char *sl = path[i];
		const char *tl = path[i + 1];

		make_request(request, current_text, sl, tl);
		free(current_text);

		printf("\n[%s -> %s] 翻訳中...\n", sl, tl);

		response = send_http_request("translate.google.com", request);
		if(!response)
		{
			printf("communication error\n");
			return 1;
		}

		translated = extract_translation(response);
		free(response);

		if(!translated)
		{
			printf("翻訳抽出失敗\n");
			return 1;
		}

		printf("結果: %s\n", translated);
		current_text = translated;
	}

	printf("\n=== 最終翻訳結果 ===\n%s\n", current_text);
	free(current_text);

	return 0;
}
