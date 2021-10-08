#include <stdio.h>
#include "text.h"

int calcSymbolsCount( char *buf, int bufLen, char sym )
{
    int i;
    int count = 0;

    for (i = 0; i < bufLen; i++)
        count += buf[i] == sym;

    if (buf[bufLen - 1] != '\n')
        count++;

    return count;
}

BOOL splitTextIntoStrings( char *buf, int bufLen, TextData *td )
{
    int curLen = 0, i, j, numStr = 0;

    td->strings = malloc(sizeof(char *) * td->rowCount);

    if (td->strings == NULL)
        return FALSE;

    for (i = 0; i < bufLen; i++)
        if (buf[i] == '\n' || i == bufLen - 1)
        {
            td->strings[numStr] = malloc(sizeof(char) * (curLen + 1));
            memset(td->strings[numStr], 0, sizeof(char) * (curLen + 1));
            if (td->strings[numStr] == NULL)
            {
                for (j = 0; j < numStr; j++)
                    free(td->strings[j]);
                break;
            }
            // куда, откуда, сколько
            // |[a;b)| == b - a
            // b==buf + i
            // b-a==curLen => a = b - curLen
            // TODO количество символов в последней строке
            strncpy(td->strings[numStr], buf + i - curLen, curLen);
            curLen = 0;
            numStr++;
        }
        else
            curLen++;

    return TRUE;
}

BOOL readFile( char *fileName, TextData *td )
{
    FILE* fp = fopen(fileName, "r");
    int size, rc;
    char *buf;

    if (!fp)
        return FALSE;

    fseek(fp, 0L, SEEK_END);
    size = ftell(fp);
    rewind(fp);
    buf = malloc(sizeof(char) * (size + 1));

    if (buf == NULL)
    {
        fclose(fp);
        return FALSE;
    }

    fread(buf, sizeof(char), size, fp);
    fclose(fp);

    td->rowCount = calcSymbolsCount(buf, size, '\n');

    rc = splitTextIntoStrings(buf, size, td);
    if (!rc)
    {
        free(buf);
        return FALSE;
    }

    free(buf);

    return TRUE;
}
