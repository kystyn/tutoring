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

void WMPaint( HWND hWnd, TextData *td, RenderData *rd, Mode mode )
{
    int y, sy;
    PAINTSTRUCT ps;

    BeginPaint(hWnd, &ps);


    if (mode == VIEW)
        for (y = rd->currentRow; y < min(rd->currentRow + rd->symsPerH, td->rowCount); y++)
            TextOut(ps.hdc, 0, (y - rd->currentRow) * rd->textHeight,
                    td->buf + td->offsets[y] + rd->currentColumn,
                    evalSymsPerW(ps.hdc, td, rd, y, 0));
    else
    {
        int strNum = 0;
        for (y = rd->currentRow; y < min(rd->currentRow + rd->symsPerH, td->rowCount); y++)
            for (sy = rd->currentSubstring; sy < td->substrCount[y]; sy++, strNum++)
                TextOut(ps.hdc, 0, strNum * rd->textHeight,
                    td->buf + td->offsets[y] + td->substrOffsets[y][sy],
                    evalSymsPerW(ps.hdc, td, rd, y, td->substrOffsets[y][sy]));
    }

    EndPaint(hWnd, &ps);
}

void WMSize( HWND hWnd, TextData *td, RenderData *rd, TEXTMETRIC *tm, int newW, int newH, Mode mode )
{
    RECT rc;

    rd->symsPerH = newH / tm->tmHeight;
    rd->screenWidth = newW;
    rd->screenHeight = newH;

    rd->symsPerW = evalSymsPerW(GetDC(hWnd), td, rd, td->longestStringIdx, 0);

    rc.top = 0;
    rc.left = 0;
    // TODO maybe pass width and height of screen received in WM_SIZE message
    rc.bottom = rd->screenHeight;
    rc.right = rd->screenWidth;
    InvalidateRect(hWnd, &rc, TRUE);

    if (mode == LAYOUT)
        evalSubstrOffsets(GetDC(hWnd), td, rd);
}

void WMKeyDown( HWND hWnd, WPARAM wParam,
                TextData *td, RenderData *rd )
{
    RECT rc;
    //GetScrollRange()

    switch (wParam)
    {
    case VK_RIGHT:
        textRight(hWnd, td, rd);
        break;
    case VK_LEFT:
        textLeft(hWnd, td, rd);
        break;
    case VK_UP:
        textUp(hWnd, td, rd);
        break;
    case VK_DOWN:
        textDown(hWnd, td, rd);
        break;
    case VK_PRIOR:
        textPgUp(hWnd, td, rd);
        break;
    case VK_NEXT:
        textPgDown(hWnd, td, rd);
        break;
    }

    rc.top = 0;
    rc.left = 0;
    // TODO maybe pass width and height of screen received in WM_SIZE message
    rc.bottom = rd->screenHeight;
    rc.right = rd->screenWidth;
    InvalidateRect(hWnd, &rc, TRUE);
}

void WMVScroll( HWND hWnd, WPARAM wParam,
                TextData *td, RenderData *rd )
{
    int pos = HIWORD(wParam);
    int minScroll, maxScroll;

    switch (LOWORD(wParam))
    {
    case SB_THUMBTRACK:
        GetScrollRange(hWnd, SB_VERT, &minScroll, &maxScroll);
        rd->currentRow = max(0, (float)(pos - minScroll) / (maxScroll - minScroll) * (td->rowCount - rd->symsPerH));
        pos = calcVScrollPos(td, rd, minScroll, maxScroll);
        SetScrollPos(hWnd, SB_VERT, pos, TRUE);
        printf("row: %i count: %i pos: %i\n", rd->currentRow, td->rowCount, pos);
        break;
    case SB_LINEDOWN:
        textDown(hWnd, td, rd);
        break;
    case SB_LINEUP:
        textUp(hWnd, td, rd);
        break;
    case SB_PAGEUP:
        textPgUp(hWnd, td, rd);
        break;
    case SB_PAGEDOWN:
        textPgDown(hWnd, td, rd);
        break;
    }

    RECT rc;
    // TODO
    GetWindowRect(hWnd, &rc);
    rc.right = rc.right - rc.left;
    rc.left = 0;
    rc.bottom = rc.bottom - rc.top;
    rc.top = 0;
    InvalidateRect(hWnd, &rc, TRUE);
}

void WMHScroll( HWND hWnd, WPARAM wParam,
                TextData *td, RenderData *rd )
{
    int pos = HIWORD(wParam);
    int minScroll, maxScroll;

    switch (LOWORD(wParam))
    {
    case SB_THUMBTRACK:
        GetScrollRange(hWnd, SB_HORZ, &minScroll, &maxScroll);
        rd->symsPerW = evalSymsPerW(GetDC(hWnd), td, rd, td->longestStringIdx, 0);
        printf("syms per w: %i\n", rd->symsPerW);
        rd->currentColumn = max(0, (float)(pos - minScroll) / (maxScroll - minScroll) * (td->longestStringLen - rd->symsPerW));
        pos = calcHScrollPos(td, rd, minScroll, maxScroll);
        SetScrollPos(hWnd, SB_HORZ, pos, TRUE);
        printf("col: %i pos: %i len %i\n", rd->currentColumn, pos, td->longestStringLen);
    break;
    case SB_LINEDOWN:
        textRight(hWnd, td, rd);
        break;
    case SB_LINEUP:
        textLeft(hWnd, td, rd);
        break;
    case SB_PAGEUP:
    case SB_PAGEDOWN:
        break;
    }

    RECT rc;
    // TODO
    GetWindowRect(hWnd, &rc);
    rc.right = rc.right - rc.left;
    rc.left = 0;
    rc.bottom = rc.bottom - rc.top;
    rc.top = 0;
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
    static Mode mode = LAYOUT;
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
            rd.textHeight = tm.tmHeight;
            rc = readFile((char *)file_name, &td);
            if (!rc)
                MessageBox(hWnd, "Couldn't read a file", "Error", 0);

            if (mode == VIEW)
            {
                ShowScrollBar(hWnd, SB_HORZ, TRUE);
                SetScrollRange(hWnd, SB_HORZ, 0, 1000, TRUE);
            }

            ShowScrollBar(hWnd, SB_VERT, TRUE);
            SetScrollRange(hWnd, SB_VERT, 0, 1000, TRUE);
            break;
        case WM_SIZE:
            WMSize(hWnd, &td, &rd, &tm, LOWORD(lParam), HIWORD(lParam), mode);
            break;
        case WM_PAINT:
            WMPaint(hWnd, &td, &rd, mode);
            break;
        case WM_KEYDOWN:
            WMKeyDown(hWnd, wParam, &td, &rd);
            break;
        case WM_VSCROLL:
            WMVScroll(hWnd, wParam, &td, &rd);
            break;
        case WM_HSCROLL:
            if (mode == VIEW)
                WMHScroll(hWnd, wParam, &td, &rd);
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc(hWnd, message, wParam, lParam);
        // TODO WM_CLOSE and free all data
    }

    return 0;
}
