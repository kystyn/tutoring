#include <tchar.h>
#include <stdio.h>

#include "win.h"

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
           _T("Code::Blocks Template Windows App"),       /* Title Text */
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

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LPSTR file_name;
    static int size = 0;
    static char* buf = NULL;
    switch (message)                  /* handle the messages */
    {

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_CREATE:
            {
            //CREATESTRUCT win = *lParam;
            file_name=(LPSTR)(((CREATESTRUCT*)lParam)->lpCreateParams);  //
            FILE* fp=fopen(file_name,"r");
            if(!fp)
                break;

            fseek(fp, 0L, SEEK_END);
            size = ftell(fp);
            rewind(fp);
            buf = (char*)malloc(sizeof(char)*(size+1));
            //fgets(buf,size,fp);
            fread(buf,sizeof(char),size,fp);
            printf(buf);

            InvalidateRect(hwnd, NULL, NULL);

            fclose(fp);
            free(buf);
            }
            break;

        case WM_PAINT:
            {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            TextOutA(hdc, 100, 50, buf, size);
            EndPaint(hwnd, &ps);
            }

            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}

