#include <iostream>
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

///*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
//fef
/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain(HINSTANCE hThisInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpszArgument,
                   int nCmdShow)
//HINSTANCE hThisInstance, ÄÈÑÊÐÈÏÒÎÐ
// HINSTANCE hPrevInstance,
//LPSTR lpszArgument, ÓÊÀÇÀÒÅËÜ ÍÀ ÍÀ×ÀËÎ ÑÒÐÎÊÈ
// int nCmdShow)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx(
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
            lpszArgument                /* No Window Creation data */
    );

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0)) {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */
typedef struct data {
    int Size;
    int lenCount;
    int maxRowLen;
    int countRows;
    char *text;
    int sizeX;

} DATA;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static DATA myData;
    static char *arr = NULL;
    HDC hdc;
    PAINTSTRUCT ps;
    HFONT font = (HFONT) GetStockObject(DEFAULT_GUI_FONT);

    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            free(arr);
            PostQuitMessage(0);/* send a WM_QUIT to the message queue */
            break;
        case WM_CREATE: {
            //myData = (DATA*)malloc(sizeof(DATA));
            CREATESTRUCT *myStruct = (CREATESTRUCT *) lParam;
            FILE *myFile = (FILE *) fopen((char *) myStruct->lpCreateParams, "rb");
            if (myFile == NULL)
                return 1;
            size_t fileSize = 0;
            fseek(myFile, 0, SEEK_END);
            fileSize = ftell(myFile);
            fseek(myFile, 0, SEEK_SET);
            myData.text = (char *) malloc(fileSize * sizeof(char));
            if (myData.text == NULL)
                return 1;
            fread(myData.text, sizeof(char), fileSize, myFile);
            myData.text[fileSize - 1] = '\0';
            fclose(myFile);
            myData.Size = fileSize;
            myData.maxRowLen = 10;
        }
            break;
        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);
            //TextOut(hdc, 10, 40, arr, strlen(arr));

            char *buffer = (char *) malloc(sizeof(char) * myData.maxRowLen);

            myData.countRows = 0;

            int i = 0;
            while (i < myData.Size) {
                int j;
                for (j = 0; j < myData.maxRowLen && *(myData.text + i + j) != '\n'; j++) {
                    buffer[j] = *(myData.text + i + j);
                }
                buffer[j] = '\0';
                TextOut(hdc, myData.Size, 40 + 20 * myData.countRows, buffer, strlen(buffer));
                i += j;
                myData.countRows++;
            }
            //HBRUSH brush = CreateSolidBrush(RGB(255, 0 ,0));
            //SetTextAlign(hdc, TA_LEFT, TA_TOP);
            EndPaint(hwnd, &ps);
        }
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc(hwnd, message, wParam, lParam);
    }


    return 0;
}

