#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include <windows.h>

typedef struct tagRenderData
{
    int currentRow;     /* Current upper row */
    int currentSubstring;
    int currentColumn;  /* Current leftist symbol */
    int symsPerW;       /* Symbols count per screen width */
    int symsPerH;       /* Symbols count per screen height */
    int screenWidth;    /* Screen width */
    int screenHeight;   /* Screen height */
    int textHeight;
} RenderData;

typedef struct tagTextData
{
    char *buf;            /* Text itself as string array */
    int *offsets;         /* String offsets */
    int *substrCount;     /* Index of array is row number, value is count of substrings. Layout mode only */
    int rowCount;         /* Count of rows in text ?? */
    int longestStringLen; /* The longest string length */
    int longestStringIdx; /* Number of the longest string */
    int bufLen;           /* Buffer size */
} TextData;

typedef enum
{
    VIEW, LAYOUT
} Mode;

BOOL readFile( char *fileName, TextData *td );

/* maybe in win.h*/
int calcSymbolsPerScreen( int width, int height, TEXTMETRIC *tm, RenderData *rd );

int evalSymsPerW( HDC hDC, TextData *td, RenderData *rd );

void evalSubstrCount( HDC hDC, TextData *td, RenderData *rd );

int calcVScrollPos( TextData *td, RenderData *rd, int minScroll, int maxScroll );
int calcHScrollPos( TextData *td, RenderData *rd, int minScroll, int maxScroll );


void textLeft( HWND hWnd, TextData *td, RenderData *rd );
void textRight( HWND hWnd, TextData *td, RenderData *rd );
void textDown( HWND hWnd, TextData *td, RenderData *rd, Mode mode );
void textUp( HWND hWnd, TextData *td, RenderData *rd, Mode mode );

void textPgUp( HWND hWnd, TextData *td, RenderData *rd, Mode mode );
void textPgDown( HWND hWnd, TextData *td, RenderData *rd, Mode mode );


#endif // TEXT_H_INCLUDED
