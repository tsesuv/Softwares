(英語できないんで日本語でどうぞ。あと批判指摘注意等は受け付けてないんで。言われなくてもわかってるんでね？わざわざ言わないでくださいね？なおバージョンは1.0.3です。Githubにも上げてます。)
```library.c
#ifdef _WIN32
#define dllexp extern __declspec(dllexport)
#else
#define dllexp /*extern __declspec(dllexport)*/
#endif
#include <stdio.h>

dllexp char* rewrite(char *p, int l, int i, char d, char res_list[])
{
    /* 2024/12/07/12:49:入力配列pを破壊しないように変更しました */
    for (int j = 0; j < l; j++)
    {
        res_list[j] = *(p + j); /* 2024/12/19/09:40:バグがあったので修正 */
        if(j == i)
        {
            res_list[j] = d;
        }
    }
    return res_list;
}

dllexp char* trim(char *p, int l, int i, char res_list[])
{
    /* 2024/12/07/12:49:入力配列pを破壊しないように変更しました */
    for (int j = 0; j < l; j++)
    {
        if(i <= j)
        {
            if (j == l - 1)
            {
                res_list[j] = '\0';
            }
            else
            {
                res_list[j] = *(p + j + 1);
            }
        }
        else
        {
            res_list[j] = *(p + j);
        }
    }
    return res_list;
}

dllexp char* find(char *p, int l, char tgt ,char res_list[])
{
    int j = 0;
    for(int i = 0; i < l; i++)
    {
        if(*(p + i) == tgt)
        {
            res_list[j] = i;
            j++;
        }
    }
    return res_list;
}

/* 2024/12/06/21:24:old_splitは削除されました */

dllexp char* split(char *p, int l, char dchar, int i, char res_list[])
{
    
    int k = 0;
    int n = 0;
    char tmp[l + 2];

    for(int ti = 0; ti < l; ti++)
    {
        tmp[ti + 1] = *(p + ti);
    }
    tmp[l + 1] = dchar;
    tmp[0] = dchar;

    for(int j = 0; j < l + 2; j++)
    {
        if(tmp[j] == dchar)
        {
            k++;
            if(k - 1 == i)
            {
                j++;
                while(1)
                {
                    if(k - 1 == i + 1)
                    {
                        return res_list;
                    }
                    if(tmp[j] == dchar)
                    {
                        k++;
                    }
                    else
                    {
                        res_list[n] = tmp[j];
                    }
                    n++;
                    j++;
                }
            }
        }
    }
}

dllexp char* slice(char *p, int l, char si, int ei, char res_list[])
{
    char tmp[l + 2];
    for(int i = 0; i < l; i++)
    {
        tmp[i + 1] = *(p + i);
    }
    tmp[ei + 1] = '\0';
    tmp[si] = '\0';
    split(&tmp[0], l, '\0', 1, &res_list[0]);
    return res_list;
}

dllexp char* swap(char *p, int l, char res_list[])
{
    for(int i = 0; i < l; i++)
    {
        res_list[i] = *(p + l - i - 1);
    }
    return res_list;
}

dllexp char* unique(char *p, int l, char res_list[])
{
    int k = 0;
    for(int i = 0; i < l; i++)
    {
        /* 2024/12/06/21:21:修正完了。もともとのコードは消しました。 */
        /* n番目の要素と*(p + i)が一致したらflagを1にする、forする前に0にして置き、forし終わった後にflag==0ならres_list[k]に追加、k++をする */
       int flag = 0;
        for(int n = 0; n < l; n++)
        {
            if(*(p + i) == res_list[n])
            {
                flag = 1;
                break; // AIの指摘により、「flagを立てたら、そのあとは調べるの無意味じゃない？」ということでforから抜けるように。
            }
        }

        if(flag == 0)
        {
            res_list[k] = *(p + i);
            k++;
        }
    }
    return res_list;
}

dllexp void twss(int *p, int l)
{
    /* 2024/12/14/21:14:このアルゴリズムの名前が「双方向選択ソート」、
    英語では「Two Way Selection Sort」になったため、関数名をtwssとさせていただきました。
    なお、既存のプログラムでの利用でエラーが起きないよう、
    sort関数ではtwssを呼び出すよう変更しております。 */
    int tmp; // 値のコピー用
    int i = 0;
    int k = l - 1;

    while(1)
    {
        if(*(p + i) > *(p + k))
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
        if(i == l - 1)
        {
            return;
        }
    }
}

dllexp void sort(int *p, int l)
{
    twss(p, l);
    return;
}

dllexp int lhash(char *p, int l)
{
    /* 2024/12/19/21:18:実装。lはlikeのl。 */
    int i = 0;
    int res[] = {};

    for(i = 0; i < l; i++)
    {
        res[i] = (int)res % ((int)*(p + i) - (int)res[i - 1]) + (int)*(p + i); 
    }
    return res;
}

dllexp void msgtype(int tablenum)
{
    switch (tablenum)
    {
    case 0:
        puts("\e[40m \e[41m \e[42m \e[43m \e[44m \e[45m \e[46m \e[47m \e[0m");
        break;
    case 1:
        puts("\e[42m[\e[30m INFO ]\e[0m ");
        break;
    case 2:
        puts("\e[41m[\e[38m ERROR ]\e[0m ");
        break;
    case 3:
        puts("\e[43m[\e[30m WARNING ]\e[0m "); /* 2024/12/19/06:07:スペルミス発覚。どんなミスしてんねん。 */
        break;
    case 4:
        puts("\e[44m[\e[30m DEBUG ]\e[0m ");
        break;
    case 5:
        puts("\e[46m[\e[30m NOTE ]\e[0m ");
        break;
    }
    return;
}
```
