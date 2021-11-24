#include <stdio.h>
#include <math.h>
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

    td->substrCount = malloc(sizeof(int) * td->rowCount);
    if (td->substrCount == NULL)
        return FALSE;
    memset(td->substrCount, 0, sizeof(int) * td->rowCount);

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

void textUp( HWND hWnd, TextData *td, RenderData *rd, Mode mode )
{
    int pos;
    int minScroll, maxScroll;

    GetScrollRange(hWnd, SB_VERT, &minScroll, &maxScroll);

    if (mode == VIEW)
        rd->currentRow = max(rd->currentRow - 1, 0);
    else
    {
        if (rd->currentSubstring != 0)
            rd->currentSubstring--;
        else
        {
            rd->currentRow = max(rd->currentRow - 1, 0);
            rd->currentSubstring = td->substrCount[rd->currentRow] - 1;
        }
    }


    pos = calcVScrollPos(td, rd, minScroll, maxScroll);
    SetScrollPos(hWnd, SB_VERT, pos, TRUE);
}

void textDown( HWND hWnd, TextData *td, RenderData *rd, Mode mode )
{
    int pos;
    int minScroll, maxScroll;

    GetScrollRange(hWnd, SB_VERT, &minScroll, &maxScroll);
    if (mode == VIEW)
        rd->currentRow = max(0, min(rd->currentRow + 1, td->rowCount - rd->symsPerH));
    else
    {
        if (rd->currentSubstring != td->substrCount[rd->currentRow] - 1)
            rd->currentSubstring++;
        else
        {
            rd->currentRow = max(0, min(rd->currentRow + 1, td->rowCount - rd->symsPerH));
            rd->currentSubstring = 0;
        }
    }
    pos = calcVScrollPos(td, rd, minScroll, maxScroll);
    SetScrollPos(hWnd, SB_VERT, pos, TRUE);
}

void textPgUp(HWND hWnd, TextData *td, RenderData *rd, Mode mode)
{
    int pos;
    int minScroll, maxScroll;

    GetScrollRange(hWnd, SB_VERT, &minScroll, &maxScroll);
    rd->currentRow = max(rd->currentRow - rd->symsPerH, 0);
    if (mode == LAYOUT)
        rd->currentSubstring = 0;

    pos = calcVScrollPos(td, rd, minScroll, maxScroll);
    SetScrollPos(hWnd, SB_VERT, pos, TRUE);
}

void textPgDown(HWND hWnd, TextData *td, RenderData *rd, Mode mode)
{
    int pos;
    int minScroll, maxScroll;

    GetScrollRange(hWnd, SB_VERT, &minScroll, &maxScroll);

    rd->currentRow = max(0, min(rd->currentRow + rd->symsPerH, td->rowCount - rd->symsPerH));
    if (mode == LAYOUT)
        rd->currentSubstring = 0;

    pos = calcVScrollPos(td, rd, minScroll, maxScroll);
    SetScrollPos(hWnd, SB_VERT, pos, TRUE);
}

int evalSymsPerW( HDC hDC, TextData *td, RenderData *rd )
{
    char *str = td->buf;
    int strLen = td->longestStringLen;
    int actualLen;

    SIZE size;
    GetTextExtentExPoint(hDC,
                         (LPCSTR)(str + td->offsets[td->longestStringIdx]),
                         strLen, rd->screenWidth, &actualLen, NULL, &size);
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

void evalSubstrCount( HDC hDC, TextData *td, RenderData *rd )
{
    int rowNum;

    if (rd->symsPerW == 0)
        return;

    /* Calc count of substrings of current string */
    for (rowNum = 0; rowNum < td->rowCount; rowNum++)
        td->substrCount[rowNum] =
            max(1, ceil((double)(td->offsets[rowNum + 1] - td->offsets[rowNum] - 1) / rd->symsPerW));
}
