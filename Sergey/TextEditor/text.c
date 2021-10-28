#include <stdio.h>
#include "text.h"

BOOL splitTextIntoStrings( TextData *td )
{
    int i, curStr = 0;
    td->rowCount = 0;

    for (i = 0; i < td->bufLen; i++)
        if (td->buf[i] == '\n')
            td->rowCount++;
        else if (i == td->bufLen - 1)
            td->rowCount++;

    td->offsets = malloc(sizeof(int) * (td->rowCount + 1));

    if (td->offsets == NULL)
        return FALSE;

    td->offsets[curStr++] = 0;
    td->longestStringLen = 0;
    for (i = 0; i < td->bufLen; i++)
        if (td->buf[i] == '\n')
        {
            td->offsets[curStr++] = i + 1;
            if (td->longestStringLen < td->offsets[curStr - 1] - td->offsets[curStr - 2] - 1)
            {
                td->longestStringIdx = curStr - 2;
                td->longestStringLen = td->offsets[curStr - 1] - td->offsets[curStr - 2] - 1;
            }
        }

    td->offsets[curStr++] = td->bufLen;
    // TODO
    /*
    td->longestStringLen =
            max(td->longestStringLen, td->offsets[curStr - 1] - td->offsets[curStr - 2] - 1);
            */

    return TRUE;
}

BOOL readFile( char *fileName, TextData *td )
{
    FILE* fp = fopen(fileName, "r");
    int rc;
    if (!fp)
        return FALSE;

    fseek(fp, 0L, SEEK_END);
    td->bufLen = ftell(fp);
    rewind(fp);
    td->buf = malloc(sizeof(char) * (td->bufLen + 1));

    if (td->buf == NULL)
    {
        fclose(fp);
        return FALSE;
    }

    fread(td->buf, sizeof(char), td->bufLen, fp);
    fclose(fp);

    rc = splitTextIntoStrings(td);
    if (!rc)
    {
        free(td->buf);
        return FALSE;
    }

    return TRUE;
}
