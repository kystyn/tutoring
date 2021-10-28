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

void textRight( HWND hWnd, TextData *td, RenderData *rd )
{
    int pos;
    int minScroll, maxScroll;

    GetScrollRange(hWnd, SB_HORZ, &minScroll, &maxScroll);
    rd->currentColumn = max(0, min(rd->currentColumn + 1, td->longestStringLen - rd->symsPerW));
    pos = calcHScrollPos(td, rd, minScroll, maxScroll);
    SetScrollPos(hWnd, SB_HORZ, pos, TRUE);
}

void textLeft( HWND hWnd, TextData *td, RenderData *rd )
{
    int pos;
    int minScroll, maxScroll;

    GetScrollRange(hWnd, SB_HORZ, &minScroll, &maxScroll);
    rd->currentColumn = max(rd->currentColumn - 1, 0);
    pos = calcHScrollPos(td, rd, minScroll, maxScroll);
    SetScrollPos(hWnd, SB_HORZ, pos, TRUE);
}

void textUp( HWND hWnd, TextData *td, RenderData *rd )
{
    int pos;
    int minScroll, maxScroll;

    GetScrollRange(hWnd, SB_VERT, &minScroll, &maxScroll);
    rd->currentRow = max(rd->currentRow - 1, 0);
    pos = calcVScrollPos(td, rd, minScroll, maxScroll);
    SetScrollPos(hWnd, SB_VERT, pos, TRUE);
}

void textDown( HWND hWnd, TextData *td, RenderData *rd )
{
    int pos;
    int minScroll, maxScroll;

    GetScrollRange(hWnd, SB_VERT, &minScroll, &maxScroll);
    rd->currentRow = max(0, min(rd->currentRow + 1, td->rowCount - rd->symsPerH));
    pos = calcVScrollPos(td, rd, minScroll, maxScroll);
    SetScrollPos(hWnd, SB_VERT, pos, TRUE);
}

void textPgUp(HWND hWnd, TextData *td, RenderData *rd){
    int pos;
    int minScroll, maxScroll;

    GetScrollRange(hWnd, SB_VERT, &minScroll, &maxScroll);
    rd->currentRow = max(rd->currentRow - rd->symsPerH, 0);
    pos = calcVScrollPos(td, rd, minScroll, maxScroll);
    SetScrollPos(hWnd, SB_VERT, pos, TRUE);
}

void textPgDown(HWND hWnd, TextData *td, RenderData *rd){
    int pos;
    int minScroll, maxScroll;

    GetScrollRange(hWnd, SB_VERT, &minScroll, &maxScroll);
    rd->currentRow = max(0, min(rd->currentRow + rd->symsPerH, td->rowCount - rd->symsPerH));
    pos = calcVScrollPos(td, rd, minScroll, maxScroll);
    SetScrollPos(hWnd, SB_VERT, pos, TRUE);
}

int evalSymsPerW( HDC hDC, TextData *td, RenderData *rd, int y, int substrOffset )
{
    char *str = td->buf + td->offsets[y] + rd->currentColumn + substrOffset;
    int strLen = td->offsets[y + 1] - td->offsets[y] - 1 - rd->currentColumn - substrOffset;

    if (y == td->longestStringIdx)
        printf("strlen longest %i\n", strLen);

    if (strLen <= 0)
        return strLen;

    int actualLen;
    SIZE size;
    GetTextExtentExPoint(hDC, (LPCSTR)str, strLen + 1, rd->screenWidth, &actualLen, NULL, &size);
    return actualLen;
}

int calcVScrollPos( TextData *td, RenderData *rd, int minScroll, int maxScroll )
{
    return (float)rd->currentRow /
            (td->rowCount - rd->symsPerH) *
            (maxScroll - minScroll) + minScroll;

}

int calcHScrollPos( TextData *td, RenderData *rd, int minScroll, int maxScroll )
{
    return max(0, (float)rd->currentColumn /
               (td->longestStringLen - rd->symsPerW) *
               (maxScroll - minScroll)) + minScroll;
}

BOOL evalSubstrOffsets( HDC hDC, TextData *td, RenderData *rd )
{
    int rowNum;

    td->substrOffsets = malloc(sizeof(int *) * td->rowCount);

    if (td->substrOffsets == NULL)
        return FALSE;

    for (rowNum = 0; rowNum < td->rowCount; rowNum++)
    {
        int substrCounter = 0;
        int curSubstrPos, curSubstrNum;

        int substrLen;
        for (curSubstrPos = 0;
              curSubstrPos < td->offsets[rowNum + 1] - td->offsets[rowNum] - 1;
              curSubstrPos += substrLen, substrCounter++)
            substrLen = evalSymsPerW(hDC, td, rd, rowNum, curSubstrPos);

        td->substrOffsets[rowNum] = malloc(sizeof(int) * substrCounter);

        if (td->substrOffsets[rowNum] == NULL)
        {
            int i;
            for (i = 0; i < rowNum; i++)
                free(td->substrOffsets[i]);
            free(td->substrOffsets);
            return FALSE;
        }

        for (curSubstrNum = 0; curSubstrNum < substrCounter; curSubstrNum++)
        {
            substrLen = evalSymsPerW(hDC, td, rd, rowNum, curSubstrPos);
            td->substrOffsets[rowNum][curSubstrNum] = substrLen;
        }
    }
    return TRUE;
}
