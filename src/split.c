#define CHUNK_SIZE 2040
#define MAX_FILENAME 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fsplit(const char *name)
{   FILE *input = fopen(name, "rb");
    if(!input) return 1;

    char base[MAX_FILENAME];
    strncpy(base, name, MAX_FILENAME);
    char *dot = strrchr(base, '.');
    if (dot) *dot = '\0';

    char buf[CHUNK_SIZE + 1];
    int chnk = 0;
    size_t size;

    while(0 < (size = fread(buf, sizeof(char), CHUNK_SIZE, input)))
    {   buf[size] = '\0';

        char outname[MAX_FILENAME];
        snprintf(outname, MAX_FILENAME, "%s-%d.txt", base, chnk);

        FILE *output = fopen(outname, "wb");
        if(!output)
        {   fclose(input);

             return 2;
        }

        fwrite(buf, sizeof(char), size, output);
        fclose(output);
        chnk++;
    }

    fclose(input);
}
