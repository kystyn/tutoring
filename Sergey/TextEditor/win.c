#include <tchar.h>
#include <stdio.h>

#include "win.h"
#include "text.h"

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

/* Declare window procedure */
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL initWindow(HINSTANCE hThisInstance,
                HINSTANCE hPrevInstance,
                LPSTR lpszArgument,
                int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return FALSE;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Sergey uber reader"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           lpszArgument                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);
    return TRUE;
}

void WMPaint( HWND hWnd, TextData *td, RenderData *rd, TEXTMETRIC *tm )
{
    int y;
    PAINTSTRUCT ps;

    BeginPaint(hWnd, &ps);

    for (y = rd->currentRow; y < min(rd->currentRow + rd->symsPerH, td->rowCount); y++)
        TextOut(ps.hdc, 0, (y - rd->currentRow) * tm->tmHeight,
                td->strings[y] + rd->currentColumn,
                strlen(td->strings[y]) - rd->currentColumn);

    EndPaint(hWnd, &ps);
}

void WMSize( HWND hWnd, RenderData *rd, TEXTMETRIC *tm, int newW, int newH )
{
    RECT rc;

    rd->symsPerW = newW / tm->tmAveCharWidth;
    rd->symsPerH = newH / tm->tmHeight;
    rd->screenWidth = newW;
    rd->screenHeight = newH;

    rc.top = 0;
    rc.left = 0;
    // TODO maybe pass width and height of screen received in WM_SIZE message
    rc.bottom = rd->screenHeight - 1;
    rc.right = rd->screenWidth - 1;
    InvalidateRect(hWnd, &rc, TRUE);
}

void WMKeyDown( HWND hWnd, WPARAM wParam,
                TextData *td, RenderData *rd )
{
    RECT rc;

    switch (wParam)
    {
    case VK_RIGHT:
        rd->currentColumn = rd->currentColumn + 1;
        break;
    case VK_LEFT:
        rd->currentColumn = max(rd->currentColumn - 1, 0);
        break;
    case VK_UP:
        rd->currentRow = max(rd->currentRow - 1, 0);
        break;
    case VK_DOWN:
        rd->currentRow = min(rd->currentRow + 1, td->rowCount - 1);
        break;
    }

    rc.top = 0;
    rc.left = 0;
    // TODO maybe pass width and height of screen received in WM_SIZE message
    rc.bottom = rd->screenHeight - 1;
    rc.right = rd->screenWidth - 1;
    InvalidateRect(hWnd, &rc, TRUE);
}

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    LPSTR file_name;
    static TextData td = {0};
    static RenderData rd = {0};

    static HDC hDC;
    static TEXTMETRIC tm;
    int rc;

    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_CREATE:
            file_name=(LPSTR)(((CREATESTRUCT*)lParam)->lpCreateParams);  //
            hDC = GetDC(hWnd);
            GetTextMetrics(hDC, &tm);
            rc = readFile((char *)file_name, &td);
            if (!rc)
                MessageBox(hWnd, "Couldn't read a file", "Error", 0);
            break;
        case WM_SIZE:
            WMSize(hWnd, &rd, &tm, LOWORD(lParam), HIWORD(lParam));
            break;
        case WM_PAINT:
            WMPaint(hWnd, &td, &rd, &tm);
            break;
        case WM_KEYDOWN:
            WMKeyDown(hWnd, wParam, &td, &rd);
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc(hWnd, message, wParam, lParam);
        // TODO WM_CLOSE and free all data
    }

    return 0;
}

