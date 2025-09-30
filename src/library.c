#ifdef _WIN32
#define dllexp extern __declspec(dllexport)
#else
#define dllexp /*extern __declspec(dllexport)*/
#endif
#define TABLE_SIZE (2 << 16)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hash.h"

/* 関数宣言 */
dllexp char *rewrite(char *p, int i, char d);
dllexp char *trim(char *p, int i);
dllexp char *find(char *p, char tgt);
dllexp char *split(char *p, char dchar, int i);
dllexp char *slice(char *p, int si, int ei);
dllexp char *swap(char *p);
dllexp char *unique(char *p);
dllexp void sort(int *p, int l);
dllexp unsigned long long *dip(unsigned long long p);
dllexp int SetVarData(const char *VarName, const char *VarData);
dllexp void *GetVarData(const char *VarName);
dllexp void tag(int tablenum);
/* 2024/12/22/12:26:関数宣言部を作成。 */

// char res_list[0xFFFF] = "";
/* 2024/12/20/15:55:引数で変数を指定する方式から
関数の返り値を使用する方式に変更するため、
変数をグローバルとして宣言。65535文字までなら何とか。 */
/* 配列を使用するすべての関数において動的メモリ化が完了したため、各関数内でres_listを宣言する形に戻しました。 */

/* 2025/09/30/16:48:全体的にフォーマットを修正したりしました。コメントのフォーマットを統一したりコード内の無意味な文字を消したり・・・ */

typedef struct
{
	char *name;
	void *value;
	struct VarTable *next;
} VarTable;

VarTable *HashTable[TABLE_SIZE];

dllexp char *rewrite(char *p, int i, char d)
{
	/* 2024/12/07/12:49:入力配列pを破壊しないように変更しました */

	int memsize = 1;
	char *res_list = (char *)malloc(memsize); /* 2025/01/13/07:56:試験的に動的メモリを使用するようにしました。 */
	if(!res_list) return NULL;
	
	int k = 0;

	while(*(p + k) != '\0')
	{
		if(memsize <= k)
		{
			memsize++;
			char *tmp = (char *)realloc(res_list, memsize * sizeof(char));
			if(!tmp) return NULL;

			res_list = tmp;
		}

		*(res_list + k) = *(p + k); /* 2024/12/19/09:40:バグがあったので修正 */
		if(k == i)
		{
			*(res_list + k) = d;
		}
		k++;
	}
	*(res_list + k) = '\0';
	return res_list;
}

dllexp char *trim(char *p, int i)
{
	/* 2024/12/07/12:49:入力配列pを破壊しないように変更しました */
	int memsize = 1;
	char *res_list = (char *)malloc(memsize);
	if(!res_list) return NULL;
	
	int k = 0;
	int l = 0;

	while(*(p + l) != '\0') l++;

	while(*(p + k) != '\0')
	{
		if(memsize <= k)
		{
			memsize++;
			char *tmp = (char *)realloc(res_list, memsize * sizeof(char));
			if(!tmp) return NULL;
			
			res_list = tmp;
		}

		if(i <= k)
		{
			if(k == l - 1) *(res_list + k) = '\0';
			else *(res_list + k) = *(p + k + 1);
		}
		else *(res_list + k) = *(p + k);
	}
	return res_list;
}

dllexp char *find(char *p, char tgt)
{
	int memsize = 1;
	char *res_list = (char *)malloc(memsize);
	if(!res_list) return NULL;

	int k = 0;
	int i = 0;

	while(*(p + k) != '\0')
	{
		if(memsize <= k)
		{
			memsize++;
			char *tmp = (char *)realloc(res_list, memsize * sizeof(char));
			if(!tmp) return NULL;
			
			res_list = tmp;
		}

		if(*(p + k) == tgt)
		{
			*(res_list + i) = k;
			i++;
		}
		k++;
	}
	return res_list;
}

/* 2024/12/06/21:24:old_splitは削除されました */

dllexp char *split(char *p, char dchar, int i)
{
	/* 2025/01/19/17:55:遂にsplitで動的メモリ化。splitは独特な動きをするからめんどかった・・・ */

	int k = 0;
	int n = 0;
	int l = 0;
	int memsize = 0;
	while(*(p + l) != '\0')
	{
		l++;
		memsize++;
	}
	char *res_list = (char *)malloc(memsize);
	if(!res_list) return NULL;

	for(int m = 0; m < l; m++) *(res_list + m) = 0;

	char tmp[l + 2];

	for(int ti = 0; ti < l; ti++) *(tmp + ti + 1) = *(p + ti);

	*(tmp + l + 1) = dchar;
	*tmp = dchar;

	for(int j = 0; j < l + 2; j++)
	{
		if(*(tmp + j) == dchar)
		{
			k++;
			if(k - 1 == i)
			{
				j++;
				while(1)
				{
					if(k - 1 == i + 1) return res_list;

					if(*(tmp + j) == dchar) k++;
					else *(res_list + n) = *(tmp + j);
					
					n++;
					j++;
				}
			}
		}
	}
	return res_list;
}

dllexp char *slice(char *p, int si, int ei)
{
	int l = 0;
	while(*(p + l) != '\0') l++;
	
	char tmp[l + 2];

	for(int i = 0; i < l; i++) *(tmp + i + 1) = *(p + i);
	
	*(tmp + ei + 1) = 0x01;
	*(tmp + si) = 0x01;
	return split(tmp, 0x01, 1);
}

dllexp char *swap(char *p)
{
	int l = 0;
	int memsize = 0;
	while(*(p + l) != '\0')
	{
		l++;
		memsize++;
	}
	char *res_list = (char *)malloc(memsize);

	for(int i = 0; i < l; i++)
	{
		*(res_list + i) = *(p + l - i - 1);
	}
	return res_list;
}

dllexp char *unique(char *p)
{
	int k = 0;
	int l = 0;
	int memsize = 0;
	while(*(p + l) != '\0')
	{
		l++;
		memsize++;
	}
	char *res_list = (char *)malloc(memsize);

	for(int i = 0; i < l; i++)
	{
		/* 2024/12/06/21:21:修正完了。もともとのコードは消しました。 */
		/* n番目の要素と*(p + i)が一致したらflagを1にする、forする前に0にして置き、forし終わった後にflag==0ならres_list[k]に追加、k++をする */
		int flag = 0;
		for(int n = 0; n < l; n++)
		{
			if(*(p + i) == *(res_list + n))
			{
				flag = 1;
				break; // AIの指摘により、「flagを立てたら、そのあとは調べるの無意味じゃない？」ということでforから抜けるように。
			}
		}

		if(!flag)
		{
			*(res_list + k) = *(p + i);
			k++;
		}
	}
	return res_list;
}

dllexp void sort(int *p, int l)
{
	int tmp; // 値のコピー用
	/* 2025/01/26/14:21:やっぱりsortに関しては長さは明示的に指定するほうがいい気がするので戻してみました。 */
	int i = 0;
	int k = l - 1;

	/* 2025/01/26/14:26:AIの指摘により、whileループの条件のほうでループを抜けるようにしました。 */
	while(i < l - 1)
	{
		if(*(p + k) < *(p + i))
		{
			tmp = *(p + i);
			*(p + i) = *(p + k);
			*(p + k) = tmp;
		}
		k--;

		if(i == k)
		{
			i++;
			k = l - 1;
		}
	}
	return;
}

dllexp uint8_t chr2int(const char *p)
{
	/* 2025/09/30/16:46:charとしての数字列をintとしての数値に変換する */
	int result = 0;
	int i = 0;
	while(*(p + i) != 0)
	{
		result *= 10;
		result += (int)*(p + i) - 0x30;
		i++;
	}
	return result;
}

dllexp unsigned long long *dip(unsigned long long p)
{
	/* 2025/01/11/21:01:実装。8Byte整数を素因数分解する。 */

	unsigned long long k = 0;
	unsigned long long i = 2;
	unsigned long long memsize = 1;
	unsigned long long *result = (unsigned long long *)malloc(memsize * sizeof(unsigned long long));
	if(!result) return NULL;

	if(p == 1)
	{
		*result = 1;
		return result;
	}
	else if(p == 0) return NULL;

	while(p != 1)
	{
		if(memsize <= k)
		{
			memsize++;
			unsigned long long *tmp = (unsigned long long *)realloc(result, memsize * sizeof(unsigned long long));
			if(!tmp) return NULL;
			
			result = tmp;
		}

		if((p % i) == 0)
		{
			*(result + k) = i;
			p /= i;
			k++;
			i--;
		}
		i++;
	}
	return result;
}

dllexp int SetVarData(const char *VarName, const char *VarData)
{
	/* 2025/03/11/12:02:実装。動的型付けの動的変数。グローバルのみ。 */

	uint32_t index = fnvla(VarName) % TABLE_SIZE;
	VarTable *OldVar = HashTable[index];
	while(OldVar != NULL)
	{
		if(strcmp(OldVar->name, VarName) == 0)
		{
			OldVar->value = (void *)VarData;
			return 0;
		}
		OldVar = (VarTable *)OldVar->next;
	}

	VarTable *Var = (VarTable *)malloc(sizeof(VarTable));
	Var->name = (char *)VarName;
	Var->value = (void *)VarData;
	Var->next = (struct VarTable *)HashTable[index];
	HashTable[index] = Var;
	return 0;
}

dllexp void *GetVarData(const char *VarName)
{
	uint32_t index = fnvla(VarName) % TABLE_SIZE;
	VarTable *Var = HashTable[index];
	while(Var != NULL)
	{
		if(strcmp(Var->name, VarName) == 0)
		{
			return Var->value;
		}
		Var = (VarTable *)Var->next;
	}
	return NULL;
}

dllexp void tag(int tablenum)
{
	switch (tablenum)
	{
		case 0:
			printf("\e[40m \e[41m \e[42m \e[43m \e[44m \e[45m \e[46m \e[47m \e[0m");
			break;
		case 1:
			printf("\e[42m[\e[30m INFO ]\e[0m ");
			break;
		case 2:
			printf("\e[41m[\e[38m ERROR ]\e[0m ");
			break;
		case 3:
			printf("\e[43m[\e[30m WARNING ]\e[0m "); /* 2024/12/19/06:07:スペルミス発覚。どんなミスしてんねん。 */
			break;
		case 4:
			printf("\e[44m[\e[30m DEBUG ]\e[0m ");
			break;
		case 5:
			printf("\e[46m[\e[30m NOTE ]\e[0m ");
			break;
	}
	return;
}
