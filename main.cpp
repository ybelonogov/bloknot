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

//#include <sysmets.h>
//typedef struct tagPAINTSTRUCT
//{
//    HDC hdc;
//    BOOL fErase;
//    RECT rcPaint;
//    BOOL fRestore;
//    BOOL fIncUpdate;
//    BYTE rgbReserved[32];
//} PAINTSTRUCT;
using namespace std;

///*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
//fef
/*  Make the class name into a global variable  */
TCHAR szClassName[] = _T("CodeBlocksWindowsApp");

bool check(char x) {
    return (x != ' ' && x != '\n' && x != '\r');
}

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
    WNDCLASSEX wndclass;    /* Data structure for the windowclass */

    /* The Window structure */
    wndclass.hInstance = hThisInstance;
    wndclass.lpszClassName = szClassName;
    wndclass.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wndclass.style = CS_HREDRAW | CS_VREDRAW;                 /* Catch double-clicks */
    wndclass.cbSize = sizeof(WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
//    wndclass.lpszMenuName = MAKEINTRESOURCE(DLG_MAIN);
//    wndclass.lpszMenuName=
    wndclass.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wndclass.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
//    wndclass.hbrBackground = (HBRUSH) GetStockObject;
    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx(&wndclass))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx(
            0,                   /* Extended possibilites for variation */
            szClassName,         /* Classname */
            _T("Code::Blocks Template Windows App"),       /* Title Text */
            WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, /* default window */

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
    UpdateWindow(hwnd);


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
/* struct with text and metoinformation about text */
typedef struct data {
    int Size{};
    int *curSize{};
//    int lenStings[1000000]{};
    int *lenStrings;
    int maxStrLen = 40;
    int countStrings{};
    char *text{};
    int sizeX{};

} DATA;

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static DATA myData;
    static char *arr = NULL;
    HDC hdc;
    PAINTSTRUCT ps;
//    HFONT font = (HFONT) GetStockObject(DEFAULT_GUI_FONT);
    static int cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth, iVscrollPos, iVscrollMax, iHscrollPos, iHscrollMax;
    int x, y, iPaintBeg, iPaintEnd, iVscrollInc, iHscrollInc;
    TEXTMETRIC tm;
    switch (message)                  /* handle the messages */

    {
//        case WM_COMMAND: {
//            switch (LOWORD(wParam)) {
//                //case меню ивенты
//                switch (LOWORD(wParam)) {
//                    case MENU_FILE_OPEN: {
//                        OPENFILENAME ofn;       // common dialog box structure
//                        char filename[255];     // buffer for file name
//
//                        // Initialize OPENFILENAME
//                        ZeroMemory(&ofn, sizeof(ofn));
//                        ofn.lStructSize = sizeof(ofn);
//                        ofn.hwndOwner = hwnd;
//                        ofn.lpstrFile = filename;
//                        ofn.lpstrFile[0] = '\0';
//                        ofn.nMaxFile = sizeof(filename);
//                        ofn.lpstrFilter = "*.TXT\0";
//                        ofn.nFilterIndex = 1;
//                        ofn.lpstrFileTitle = NULL;
//                        ofn.nMaxFileTitle = 0;
//                        ofn.lpstrInitialDir = NULL;
//                        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//
//                        if (GetOpenFileName(&ofn) == TRUE) {
//                            printf("\nfile opened\n");                  //debug info
//
//                            ClearFileBuffer(&global->fileBuffer);
//                            ClearScreenBuffer(&global->screenBuffer);
//
//                            global->fileLoaded = ReadToFileBuffer(ofn.lpstrFile, &global->fileBuffer);
//                            if (global->fileLoaded == TRUE)
//                                ResizeScreenBuffer(hwnd, &global->screenBuffer, &global->fileBuffer, &global->fontInfo);
//                        }
//                    }
//                        break;
//                    case MENU_FILE_CLOSE: {
//                        ClearFileBuffer(&global->fileBuffer);
//                        ClearScreenBuffer(&global->screenBuffer);
//                        global->fileLoaded = FALSE;
//                        InvalidateRect(hwnd, NULL, TRUE);
//                        ShowScrollBar(hwnd, SB_HORZ, FALSE);
//                        ShowScrollBar(hwnd, SB_VERT, FALSE);
//                    }
//                        break;
//                    case MENU_FILE_EXIT:
//                        SendMessage(hwnd, WM_CLOSE, 0, 0);
//                        break;
//                    case MENU_CHANGE_MODE: {
//                        int mode = GetMenuState(GetMenu(hwnd), MENU_CHANGE_MODE, MF_BYCOMMAND);
//                        if (mode & MF_CHECKED) {
//                            CheckMenuItem(GetMenu(hwnd), MENU_CHANGE_MODE, MF_BYCOMMAND | MF_UNCHECKED);
//                            global->screenBuffer.hasHScroll = TRUE;
//                        } else {
//                            CheckMenuItem(GetMenu(hwnd), MENU_CHANGE_MODE, MF_BYCOMMAND | MF_CHECKED);
//                            global->screenBuffer.hasHScroll = FALSE;
//                        }
//                        ResizeScreenBuffer(hwnd, &global->screenBuffer, &global->fileBuffer, &global->fontInfo);
//                    }
//                        break;
//                }
//            }
//            break;
//        }

        case WM_SIZE:
            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
            iVscrollMax = max(0, myData.countStrings + 2 - cyClient / cyChar);
            iVscrollPos = min(iVscrollPos, iVscrollMax);
            SetScrollRange(hwnd, SB_VERT, 0, iVscrollMax, FALSE);
            SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
            iHscrollMax = max(0, 2 + (iMaxWidth - cxClient) / cxChar);
            iHscrollPos = min(iHscrollPos, iHscrollMax);
            SetScrollRange(hwnd, SB_HORZ, 0, iHscrollMax, FALSE);
            SetScrollPos(hwnd, SB_HORZ, iHscrollPos, TRUE);
            return 0;

        case WM_CREATE: {
            hdc = GetDC(hwnd);
            CREATESTRUCT *myStruct = (CREATESTRUCT *) lParam;
            FILE *myFile = (FILE *) fopen((char *) myStruct->lpCreateParams, "rb");
            if (myFile == NULL)
                return 1;
            size_t fileSize = 0;
            fseek(myFile, 0, SEEK_END);
            fileSize = ftell(myFile);
            fseek(myFile, 0, SEEK_SET);
            myData.text = (char *) malloc(fileSize * sizeof(char));
            myData.lenStrings = (int *) malloc(fileSize * sizeof(int));
            myData.curSize = (int *) malloc(fileSize * sizeof(int));
            myData.countStrings = 0;
            if (myData.text == NULL) {
                return 1;
            }
            fread(myData.text, sizeof(char), fileSize, myFile);
            myData.text[fileSize] = '\n';
            int prevI = 0;
            myData.Size = fileSize;
            for (int i = 0; i < myData.Size; i++) {
                if (myData.text[i] == '\n') {
                    myData.lenStrings[myData.countStrings] = i - prevI;
                    myData.maxStrLen = max(myData.maxStrLen, myData.lenStrings[myData.countStrings]);
                    if (myData.countStrings != 0) {
                        myData.curSize[myData.countStrings] =
                                myData.curSize[myData.countStrings - 1] + myData.lenStrings[myData.countStrings - 1];
                    } else
                        myData.curSize[myData.countStrings] = 0;

                    myData.countStrings++;
                    prevI = i;

                }
            }
            GetTextMetrics(hdc, &tm);
            cxChar = tm.tmAveCharWidth;
            cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
            cyChar = tm.tmHeight + tm.tmExternalLeading;
            ReleaseDC(hwnd, hdc);
            iMaxWidth = 40 * cxChar + 22 * cxCaps;
            fclose(myFile);

        }
            break;
        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);
            BeginPaint(hwnd, &ps);
            iPaintBeg = max(long(0), iVscrollPos + ps.rcPaint.top / cyChar - 1);
            iPaintEnd = min(myData.countStrings, iVscrollPos + cyClient);


            if (1) {
                char *buffer = (char *) malloc(sizeof(char) * myData.maxStrLen);
                for (int i = iPaintBeg; i < iPaintEnd; i++) {

                    y = cyChar * (1 - iVscrollPos + i);
//                auto sysmetrics= GetSystemMetrics();
                    int num = 0;
                    for (int j = iHscrollPos; j < myData.lenStrings[i]; j++) {
                        buffer[num] = myData.text[myData.curSize[i] + j];
                        num++;
                    }
                    cerr << "buffer:    " << buffer;
                    TextOut(hdc, 0, y, buffer, num);
                }
                delete (buffer);
            } else {
                char *buffer = (char *) malloc(sizeof(char) * cxClient);
                int pos = myData.curSize[iPaintBeg];
                for (int i = iPaintBeg; i < iPaintEnd; i++) {
                    y = cyChar * (1 - iVscrollPos + i);
//
                    int num = 0;
                    while (num <= cxClient / cxChar && pos + num <= myData.Size) {
                        buffer[num] = myData.text[pos + num];
                        num++;
                    }
                    pos += num;
                    if (pos + num <= myData.Size && check(myData.text[pos]) &&
                        check(myData.text[pos + 1] && check(myData.text[pos - 1]))) {
                        buffer[num] = '-';
                        num++;
                    }

                    TextOut(hdc, 0, y, buffer, num);

                }
                delete (buffer);

            }
//                cerr<<count<<"  "<<myData.curSize[i]<<"  "<<myData.lenStrings[i]<<endl;
//                cerr<<x<<"  "<<y<<"  "<<iPaintBeg<<" "<<i<<" "<<(i-iPaintBeg)*cyChar <<endl;
//                cerr << buffer;

//                SetTextAlign(hdc, TA_RIGHT | TA_TOP);

//                TextOut( hdc, 10, y, buffer,myData.lenStrings[i]);
//                SetTextAlign(hdc, TA_LEFT | TA_TOP);
//                66

//            cerr << x << "  " << y << "  " << iPaintBeg << endl<< endl<< endl<< endl;
            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_VSCROLL :
            switch (LOWORD(wParam)) {
                case SB_TOP :
                    iVscrollInc = -iVscrollPos;
                    break;
                case SB_BOTTOM :
                    iVscrollInc = iVscrollMax - iVscrollPos;
                    break;
                case SB_LINEUP :
                    iVscrollInc = -1;
                    break;
                case SB_LINEDOWN :
                    iVscrollInc = 1;
                    break;
                case SB_PAGEUP :
                    iVscrollInc = min(-1, -cyClient / cyChar);
                    break;
                case SB_PAGEDOWN :
                    iVscrollInc = max(1, cyClient / cyChar);
                    break;
                case SB_THUMBTRACK :
                    iVscrollInc = HIWORD(wParam) - iVscrollPos;
                    break;
                default :
                    iVscrollInc = 0;
            }
            iVscrollInc = max(
                    -iVscrollPos,
                    min(iVscrollInc, iVscrollMax - iVscrollPos)
            );
            if (iVscrollInc != 0) {
                iVscrollPos += iVscrollInc;
                ScrollWindow(hwnd, 0, -cyChar * iVscrollInc, NULL, NULL);
                SetScrollPos(hwnd, SB_VERT, iVscrollPos, TRUE);
//                InvalidateRect(hwnd, NULL, TRUE);
//                UpdateWindow(hwnd);
            }
            return 0;
        case WM_HSCROLL :
            switch (LOWORD(wParam)) {
                case SB_LINEUP :
                    iHscrollInc = -1;
                    break;
                case SB_LINEDOWN :
                    iHscrollInc = 1;
                    break;
                case SB_PAGEUP :
                    iHscrollInc = -8;
                    break;
                case SB_PAGEDOWN :
                    iHscrollInc = 8;
                    break;
                case SB_THUMBPOSITION :
                    iHscrollInc = HIWORD(wParam) - iHscrollPos;
                    break;
                default :
                    iHscrollInc = 0;
            }
            iHscrollInc = max(
                    -iHscrollPos,
                    min(iHscrollInc, iHscrollMax - iHscrollPos)
            );
            if (iHscrollInc != 0) {
                iHscrollPos += iHscrollInc;
                ScrollWindow(hwnd, -cxChar * iHscrollInc, 0, NULL, NULL);
                SetScrollPos(hwnd, SB_HORZ, iHscrollPos, TRUE);
                InvalidateRect(hwnd, NULL, TRUE);
                UpdateWindow(hwnd);
            }
            return 0;
        case WM_DESTROY:
            free(arr);
            PostQuitMessage(0);/* send a WM_QUIT to the message queue */
            break;
        default:
            return DefWindowProc(hwnd, message, wParam, lParam);


    }


    return 0;
}
