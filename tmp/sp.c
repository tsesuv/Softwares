#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 2040
#define MAX_FILENAME 256

void split_file(const char *filename) {
    FILE *input = fopen(filename, "r");
    if (!input) {
        perror("ファイルを開けませんでした");
        return;
    }

    // 元ファイル名から拡張子を除いた名前を作成
    char base_name[MAX_FILENAME];
    strncpy(base_name, filename, MAX_FILENAME);
    char *dot = strrchr(base_name, '.');
    if (dot) *dot = '\0';  // 拡張子を除去

    char buffer[CHUNK_SIZE + 1];
    int chunk_index = 0;
    size_t read_size;

    while ((read_size = fread(buffer, sizeof(char), CHUNK_SIZE, input)) > 0) {
        buffer[read_size] = '\0';  // Null終端

        // 出力ファイル名を作成
        char output_filename[MAX_FILENAME];
        snprintf(output_filename, MAX_FILENAME, "%s-%d.txt", base_name, chunk_index);

        FILE *output = fopen(output_filename, "w");
        if (!output) {
            perror("出力ファイルを作成できませんでした");
            fclose(input);
            return;
        }

        fwrite(buffer, sizeof(char), read_size, output);
        fclose(output);
        chunk_index++;
    }

    fclose(input);
    printf("ファイルを %d 個のチャンクに分割しました。\n", chunk_index);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("使い方: %s <入力ファイル名>\n", argv[0]);
        return 1;
    }

    split_file(argv[1]);
    return 0;
}
