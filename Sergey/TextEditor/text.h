#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include <windows.h>

typedef struct tagRenderData
{
    int currentRow;     /* Current upper row */
    int currentColumn;  /* Current leftist symbol */
    int symsPerW;       /* Symbols count per screen width */
    int symsPerH;       /* Symbols count per screen height */
    int screenWidth;    /* Screen width */
    int screenHeight;   /* Screen height */
} RenderData;

typedef struct tagTextData
{
    char **strings;     /* Text itself as string array */
    int rowCount;       /* Count of rows in text ?? */
} TextData;

BOOL readFile( char *fileName, TextData *td );

/* maybe in win.h*/
int calcSymbolsPerScreen( int width, int height, TEXTMETRIC *tm, RenderData *rd );

#endif // TEXT_H_INCLUDED
