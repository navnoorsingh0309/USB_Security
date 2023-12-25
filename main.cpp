#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <dbt.h>
#include <stdio.h>
#include <gdiplus.h>
#include <iostream>
#define ID_TIMER 1001
#define ID_TIMER_TOPMOST 1003
#define ID_SP_STATIC 12
#define ID_EP_EDIT 13
#define ID_OK_BUTTON 15
#define ID_DENY_BTN 31
#define ID_ALLOW_BTN 21789
#define ID_FLASH_FOUND 19
#define ID_SP_HALT 34
#define ID_SP_TURNS 23
#define WM_SHOWEVERYTHING 78907
#define WM_HIDEEVERYTHING 12345
using namespace std;
using namespace Gdiplus;
HWND Window, Static, Edit, okButton, StaticTrs, StaticH;
HWND Denyb, Allowb, Flashs;
long style;
MSG Msg;
HMENU Hmenu;
int con = 0, len = 0, turns = 3, keynum = 0, timer = 0, found = 0, halt = 0;
int ex = 0, width = 0, height = 0;
string passa = "", sapass =  "ADMIN";
char* text, adminpass[100] = "ADMIN";
char apass[100]="";
HFONT font;
string stext;
HHOOK hook;
RECT rec;
HDC hdc, hdc1, hdc2;
KBDLLHOOKSTRUCT* KeyBoard;
NOTIFYICONDATA Data;
WNDPROC oeditk;
LOGFONTW lfontw;
void Access(HDC hdc, int i)
{
    GetWindowRect(GetDesktopWindow(), &rec);
    width = rec.right - rec.left;
    height = rec.bottom - rec.top;
    RectF recf(width / 2.67, height / 1.8, width / 2, height / 15);
    RectF recf1(width / 2.61, height / 1.8, width / 2, height / 15);
    StringFormat format;
    Graphics g(hdc);
    WCHAR ad[] = L"Access Denied";
    WCHAR ad1[] = L"Access Granted";
    Pen pen(Color::Red, 3.0);
    Pen pen1(Color::Green, 3.0);
    SolidBrush brush(Color::Red);
    SolidBrush brush1(Color::Green);
    Font f(L"Arial", height / 30);
    if(i == 0)
    {
     //   g.DrawRectangle(&pen, width / 2.7, height / 1.85, width / 3.8, height / 15);
        g.DrawString(ad, 13, &f, recf1, &format, &brush);
    }
    if(i == 1)
    {
       // g.DrawRectangle(&pen1, width / 2.7, height / 1.85, width / 3.8, height / 15);
        g.DrawString(ad1, 14, &f, recf, &format, &brush1);
    }
}
float GetDriverTotalSpace(const char* disk)
{
    __int64 totalf = 0;
    float total = 0;
    long int l = 0;
    DWORD fsys=0;
    GetVolumeInformation(disk, 0, 0, 0, 0, &fsys, 0, 0);
    if(fsys!=0)
        GetDiskFreeSpaceEx(disk,NULL,(PULARGE_INTEGER)&totalf,NULL);
    total = totalf;
    total = total/1024/1024;
    l = total;
    return l;
}
LRESULT CALLBACK EditKeys(HWND wnd, UINT Msge, WPARAM WParam, LPARAM LParam)
{
    switch(Msge)
    {
        case WM_KEYDOWN:
            {
                switch(WParam)
                {
                    case VK_RETURN:
                        {
                            SendMessage(okButton, BM_CLICK, BM_CLICK, 0);
                        }
                        break;
                }
            }
            break;
        default:
            CallWindowProc(oeditk, wnd, Msge, WParam, LParam);
    }
}
LRESULT CALLBACK Keys(int nCode,WPARAM wParam, LPARAM lPasram)
{
    KeyBoard=(KBDLLHOOKSTRUCT*)lPasram;
    BOOL bControlKeyDown = 0;
    switch (nCode)
    {
        case HC_ACTION:
        {
            if(con == 1)
            {
            // Check to see if the CTRL key is pressed
            bControlKeyDown = GetAsyncKeyState (VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1);
            // Disable CTRL+ESC
            if (KeyBoard->vkCode == VK_ESCAPE && bControlKeyDown)
                return 1;
            // Disable ALT+TAB
            if (KeyBoard->vkCode == VK_TAB && KeyBoard->flags & LLKHF_ALTDOWN)
                return 1;
            // Disable ALT+ESC
            if (KeyBoard->vkCode == VK_ESCAPE && KeyBoard->flags & LLKHF_ALTDOWN)
                return 1;
            //to stop opening my PC
            if (KeyBoard->vkCode == 91 && 'E' || KeyBoard->vkCode == 92 && 'E')
                return 1;
            //to Stop Log off Function
            if (KeyBoard->vkCode == 91 && 'L' || KeyBoard->vkCode == 92 && 'L')
                return 1;
            //Stop Minimizing of application
            if (KeyBoard->vkCode == 91 && 'S' || KeyBoard->vkCode == 92 && 'S')
                return 1;
            if(KeyBoard->vkCode == VK_DELETE && bControlKeyDown && KeyBoard->flags & LLKHF_ALTDOWN)
                Beep(1000, 1000);
            }
            break;
        }
        default:
            break;
    }
    return CallNextHookEx(hook,nCode,wParam,lPasram);
}
int MiddleCheck()
{
    Sleep(1000);
    int dr_type1=99;
    char dr_avail1[256];
    char *temp1=dr_avail1;
    int i1=0, res1=0, rr = 0;
    float size1 = 0;
    /* 1st we fill the buffer */
    GetLogicalDriveStrings(256,dr_avail1);
    while(*temp1!=NULL)
    { // Split the buffer by null
        dr_type1 = GetDriveType(temp1);
        if(dr_type1 == 2)
        {
            size1 = GetDriverTotalSpace(temp1);
            if(size1 <= 0)
            {

            }
            else
            {
                rr = 123;
                return rr;
            }
        }
        else if(dr_type1 == 5)
        {
            size1 = GetDriverTotalSpace(temp1);
            if(size1 <= 0)
                {

                }
            else
            {
                rr = 123;
                return rr;
                break;
            }
        }
        temp1 += lstrlen(temp1) +1; // incriment the buffer
    }
}
void StartCheck()
{
    timer = 2;
    ShowWindow(Window, SW_HIDE);
    con = 0;
    PostMessage(Window, WM_HIDEEVERYTHING, 0, 0);
    int dr_type=99;
    char dr_avail[256];
    char *temp=dr_avail;
    int i=0, res=0;
    int size = 0;
    /* 1st we fill the buffer */
    GetLogicalDriveStrings(256,dr_avail);
    while(*temp!=NULL)
    { // Split the buffer by null
        dr_type = GetDriveType(temp);
        if(dr_type == 2)
        {
            size = GetDriverTotalSpace(temp);
            if(size <= 0)
            {
                con = 0;
                timer = 2;
                found = 0;
                ShowWindow(Window, SW_HIDE);
            }
            else
            {
                timer = 1;
                ShowWindow(Window, SW_MAXIMIZE);
                ShowWindow(Window, SW_SHOW);
                ShowWindow(Denyb, SW_SHOW);
                ShowWindow(Flashs, SW_SHOW);
                ShowWindow(Allowb, SW_SHOW);
                found = 1;
                con = 1;
                Beep(3000, 900);
                break;
            }
          /*  res = MessageBox(Window, "Flash Drive Found, Allow?", "Portable Security Message", MB_YESNO|MB_TOPMOST);
            if(res == 6)
            {
                con = 0;
                ShowWindow(Window, SW_MINIMIZE);
                PostMessage(Window, WM_SHOWEVERYTHING, 0, 0);
                timer = 2;
            }
            else
            {
                con = 1;
                PostMessage(Window, WM_SHOWEVERYTHING, 0, 0);
                timer = 1;
            }*/
        }
        else if(dr_type == 5)
        {
            try
            {
                size = GetDriverTotalSpace(temp);
            }
            catch(int e)
            {

            }
            if(size <= 0)
            {
                con = 0;
                timer = 2;
                found = 0;
                ShowWindow(Window, SW_HIDE);
            }
            else
            {
                timer = 1;
            ShowWindow(Window, SW_MAXIMIZE);
            ShowWindow(Window, SW_SHOW);
            ShowWindow(Denyb, SW_SHOW);
            ShowWindow(Flashs, SW_SHOW);
            ShowWindow(Allowb, SW_SHOW);
            found = 1;
            con = 1;
            Beep(3000, 900);
            break;
            }
        }
        temp += lstrlen(temp) +1; // incriment the buffer
    }
    }
    /*if(res == 6 || res ==0)
    {
        con = 0;
        ShowWindow(Window, SW_MINIMIZE);
        PostMessage(Window, WM_SHOWEVERYTHING, 0, 0);
        timer = 1;
    }*/
void SysTrayIcon()
{
    CHAR* cc = "Portable Security";
    Data.cbSize = sizeof(NOTIFYICONDATA);
    Data.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO1));
    Data.hWnd = Window;
    Data.uCallbackMessage = WM_SYSICON;
    Data.uFlags = NIF_ICON|NIF_TIP|NIF_MESSAGE;
    Data.uID = ID_TRAY_APP_ICON;
    strcpy(Data.szTip, cc);
}
void AllowFF()
{
    con = 0;
    ShowWindow(Window, SW_HIDE);
    PostMessage(Window, WM_SHOWEVERYTHING, 0, 0);
    timer = 2;
    found = 0;
}
void DenyFF()
{
    con = 1;
    PostMessage(Window, WM_SHOWEVERYTHING, 0, 0);
    ShowWindow(Denyb, SW_HIDE);
    ShowWindow(Allowb, SW_HIDE);
    timer = 1;
    found = 0;
}
void ClearGraphics(HDC hdc)
{
    GetWindowRect(Window, &rec);
    width = rec.right - rec.left;
    height = rec.bottom - rec.top;
    Graphics gg(hdc);
    SolidBrush brush(Color::Black);
    gg.FillRectangle(&brush, width / 2.8, height / 1.86, width / 3, height / 13);
}
void SetCtlsSize()
{
    GetWindowRect(GetDesktopWindow(), &rec);
    width = rec.right - rec.left;
    height = rec.bottom - rec.top;
    MoveWindow(Allowb, width / 2.4, height / 2, width / 14.4, height / 36, 1);
    MoveWindow(Denyb,width / 1.9, height / 2, width / 14.4, height / 36, 1);
    MoveWindow(Flashs, width / 3.2, height / 2.72, 750, 50, 1);
    MoveWindow(StaticH, width / 4.8, height / 9, width / 1.8, height / 9, 1);
    MoveWindow(Static, width / 2.62, height / 2.25, 200, height / 36, 1);
    if(width == 800)
        MoveWindow(Static, width / 3, height / 2.25, 200, height / 36, 1);
    MoveWindow(Edit, width / 2.04, height / 2.25, width / 11.076923, height / 50, 1);
    MoveWindow(okButton, width / 2.2, height / 2, width / 14.4, height / 36, 1);
    MoveWindow(StaticTrs, width / 3.8, height / 1.2857142, width / 1.6, height / 9, 1);
    font = CreateFont(width / 28.5, height / 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
    SendMessage(StaticTrs, WM_SETFONT, (WPARAM)font, 0);
    SendMessage(StaticH, WM_SETFONT, (WPARAM)font, 0);
    font = CreateFont(width / 48, height / 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
    SendMessage(Flashs, WM_SETFONT, (WPARAM)font, 0);
}
LRESULT CALLBACK WinFunc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_ACTIVATE:
            {
                Shell_NotifyIcon(NIM_ADD, &Data);
            }
            break;
        case WM_CREATE:
            {
                GetWindowRect(GetDesktopWindow(), &rec);
                width = rec.right - rec.left;
                height = rec.bottom - rec.top;
                MoveWindow(Allowb, width / 2.4, height / 2, width / 14.4, height / 36, 1);
                MoveWindow(Denyb,width / 1.9, height / 2, width / 14.4, height / 36, 1);
                MoveWindow(Flashs, width / 3.2, height / 2.72, 750, 50, 1);
                MoveWindow(StaticH, width / 4.8, height / 9, width / 1.8, height / 9, 1);
                MoveWindow(Static, width / 2.62, height / 2.25, 200, height / 36, 1);
                MoveWindow(Edit, width / 2.04, height / 2.25, width / 11.076923, height / 50, 1);
                MoveWindow(okButton, width / 2.2, height / 2, width / 14.4, height / 36, 1);
                MoveWindow(StaticTrs, width / 3.8, height / 1.2857142, width / 1.6, height / 9, 1);
                Allowb = CreateWindow("Button", "A&llow", WS_CHILD, width / 2.4, height / 2, width / 14.4, height / 36, hWnd, (HMENU)ID_ALLOW_BTN, NULL, NULL);
                Denyb = CreateWindow("Button", "Deny", WS_CHILD, width / 1.9, height / 2, width / 14.4, height / 36, hWnd, (HMENU)ID_DENY_BTN, NULL, NULL);
                Flashs = CreateWindow("Static", "Flash Drive Found!!!", WS_CHILD, width / 3.2, height / 2.72, 750, 50, hWnd, (HMENU)ID_FLASH_FOUND, NULL, NULL);
                StaticH = CreateWindow("Static", "System Halt!!", WS_CHILD, width / 4.8, height / 9, width / 1.8, height / 9, hWnd, (HMENU)ID_SP_HALT, NULL, NULL);
                Static = CreateWindow("Static", "User Password:-", WS_CHILD,width / 2.62, height / 2.25, 200, height / 36, hWnd, (HMENU)ID_SP_STATIC, NULL, NULL);
                if(width == 800)
                    MoveWindow(Static, width / 3, height / 2.25, 200, height / 36, 1);
                Edit = CreateWindow("Edit", "", WS_CHILD, width / 2.04, height / 2.25, width / 11.076923, height / 50, hWnd, (HMENU)ID_EP_EDIT, NULL, NULL);
                okButton = CreateWindow("Button", "OK", WS_CHILD, width / 2.2, height / 2, width / 14.4, height / 36, hWnd, (HMENU)ID_OK_BUTTON, NULL, NULL);
                StaticTrs = CreateWindow("Static", "3 Turns Left!!!", WS_CHILD,  width / 3.8, height / 1.2857142, 100 , height / 9, hWnd, (HMENU)ID_SP_TURNS, NULL, NULL);
                oeditk = (WNDPROC)SetWindowLongPtr(Edit, GWLP_WNDPROC, (LONG_PTR)EditKeys);
                SetFocus(Edit);
                Hmenu = CreatePopupMenu();
                AppendMenu(Hmenu, MF_STRING, ID_TRAY_EXIT, TEXT("Exit"));
                font = CreateFont(width / 28.5, height / 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
                SendMessage(StaticTrs, WM_SETFONT, (WPARAM)font, 0);
                SendMessage(StaticH, WM_SETFONT, (WPARAM)font, 0);
                font = CreateFont(width / 48, height / 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Arial");
                SendMessage(Flashs, WM_SETFONT, (WPARAM)font, 0);
            }
            break;
        case WM_COMMAND:
            {
                if(LOWORD(wParam) == ID_ALLOW_BTN)
                {
                    AllowFF();
                }
                if(LOWORD(wParam) == ID_DENY_BTN)
                {
                    DenyFF();
                    SetCtlsSize();
                }
                if(LOWORD(wParam) == ID_OK_BUTTON)
                {
                    len = GetWindowTextLength(Edit)+1;
                    text = new char[len];
                    GetWindowText(Edit, text, len);
                    stext = text;
                    if(stext == "LightYagami")
                    {
                        hdc2 = GetDC(Window);
                        Access(hdc2, 1);
                        Sleep(3000);
                        con = 0;
                        timer = 2;
                        turns = 3;
                        SetWindowText(StaticTrs, "3 Turns Left!!!");
                        ShowWindow(Window, SW_HIDE);
                    }
                    else
                    {
                        hdc2 = GetDC(Window);
                        timer = 2;
                        Access(hdc2, 0);
                        Beep(3000, 900);
                        Sleep(1000);
                        ClearGraphics(hdc2);
                        //MessageBox(Window, "Wrong Password", "Portable Security Message", MB_ICONERROR|MB_TOPMOST);
                        timer = 1;
                        SetWindowText(Edit, "");
                        SetFocus(Edit);
                        turns--;
                        if(turns == 2)
                        {
                            SetWindowText(StaticTrs, "2 Turns Left!!!");
                            ShowWindow(StaticTrs, SW_HIDE);
                            Sleep(100);
                            ShowWindow(StaticTrs, SW_SHOW);
                            Sleep(100);
                            ShowWindow(StaticTrs, SW_HIDE);
                            Sleep(100);
                            ShowWindow(StaticTrs, SW_SHOW);
                            SetCtlsSize();
                        }
                        else if(turns == 1)
                        {
                            SetWindowText(StaticTrs, "Last Chance!!!");
                            ShowWindow(StaticTrs, SW_HIDE);
                            Sleep(100);
                            ShowWindow(StaticTrs, SW_SHOW);
                            Sleep(100);
                            ShowWindow(StaticTrs, SW_HIDE);
                            Sleep(100);
                            ShowWindow(StaticTrs, SW_SHOW);
                            SetCtlsSize();
                        }
                        else if(turns == 0)
                        {
                            ShowWindow(okButton, SW_HIDE);
                            ShowWindow(Static, SW_HIDE);
                            ShowWindow(StaticH, SW_SHOW);
                            ShowWindow(Flashs, SW_HIDE);
                            ShowWindow(Edit, SW_HIDE);
                            ShowWindow(okButton, SW_HIDE);
                            halt = 1;
                            SetWindowText(StaticTrs, "Calling Admin");
                            Sleep(100);
                            Beep(3000, 3000);
                            SetFocus(Window);
                            turns = 0;
                            SetCtlsSize();
                        }
                    }
                }
            }
            break;
        case WM_CTLCOLORSTATIC:
            {
                if(StaticH == (HWND)wParam)
                {
                    hdc1 = (HDC)wParam;
                    SetBkColor(hdc1, RGB(0, 0, 0));
                    SetTextColor(hdc1, RGB(255, 255, 255));
                    return (LRESULT)CreateSolidBrush(RGB(0,0,0));
                }
                hdc = (HDC)wParam;
                SetBkColor(hdc, RGB(0, 0, 0));
                SetTextColor(hdc, RGB(255, 255, 255));
                return (LRESULT)CreateSolidBrush(RGB(0,0,0));
            }
            break;
        case WM_DEVICECHANGE:
            {
                if(halt == 0)
                {
                PDEV_BROADCAST_HDR pHdr = (PDEV_BROADCAST_HDR) lParam;
                switch (wParam)
                {
                    case DBT_DEVICEARRIVAL:
                        ShowWindow(Window, SW_MAXIMIZE);
                        ShowWindow(Window, SW_SHOW);
                        SetFocus(Edit);
                        ShowWindow(Denyb, SW_HIDE);
                        ShowWindow(Allowb, SW_HIDE);
                        PostMessage(Window, WM_SHOWEVERYTHING, 0, 0);
                        con = 1;
                        timer = 1;
                        SetCtlsSize();
                        /*Sleep(1000);
                        Beep(3000, 900);*/
                        break;
                    case DBT_DEVICEREMOVECOMPLETE:
                        ex = MiddleCheck();
                        if(ex != 123)
                        {
                        if(turns != 0)
                        {
                            timer = 2;
                            ShowWindow(Window, SW_HIDE);
                        }
                        }
                        con = 0;
                        passa = "";
                        keynum = 0;
                        break;
                }
                }
            }
            break;
        case WM_KEYDOWN:
            {
                if(turns == 0)
                {
                apass[keynum] = wParam;
                if(apass[keynum] == adminpass[keynum])
                {
                    passa+=apass[keynum];
                    keynum++;
                }
                else
                {
                    passa = "";
                    keynum = 0;
                }
                if(passa == sapass)
                {
                    turns = 3;
                    SetWindowText(StaticTrs, "3 Turns Left!!!");
                    halt = 0;
                    ShowWindow(Window, SW_HIDE);
                    timer = 2;
                }
                }
                if(found == 1)
                {
                    if(LOWORD(wParam) == 65 || LOWORD(wParam) == 97)
                    {
                        AllowFF();
                    }
                    else if(LOWORD(wParam)==68 || LOWORD(wParam) == 100)
                    {
                        DenyFF();
                    }
                }
            }
            break;
        case WM_SHOWEVERYTHING:
            {
                ShowWindow(Static, SW_SHOW);
                ShowWindow(Edit, SW_SHOW);
                ShowWindow(okButton, SW_SHOW);
                ShowWindow(StaticTrs, SW_SHOW);
                ShowWindow(StaticH, SW_HIDE);
                ShowWindow(Flashs, SW_SHOW);
                SetCtlsSize();
                EnableWindow(Edit, 1);
                EnableWindow(okButton, 1);
                SetFocus(Edit);
                SetWindowText(Edit, "");
            }
            break;
        case WM_HIDEEVERYTHING:
            {
                ShowWindow(Static, SW_HIDE);
                ShowWindow(Edit, SW_HIDE);
                ShowWindow(okButton, SW_HIDE);
                ShowWindow(StaticTrs, SW_HIDE);
                ShowWindow(StaticH, SW_HIDE);
                SetCtlsSize();
            }
            break;
        case WM_SYSICON:
	    {

	        switch(wParam)
	        {
	            case ID_TRAY_APP_ICON:
	                SetForegroundWindow(Window);
                    break;
	        }
	        if (lParam == WM_RBUTTONDOWN)
	        {
	            // Get current mouse position.
	            POINT curPoint ;
	            GetCursorPos( &curPoint ) ;
	            SetForegroundWindow(Window);
	            // TrackPopupMenu blocks the app until TrackPopupMenu returns
	            UINT clicked = TrackPopupMenu(Hmenu,TPM_RETURNCMD | TPM_NONOTIFY,curPoint.x,curPoint.y,0,hWnd,NULL);
	            SendMessage(hWnd, WM_NULL, 0, 0); // send benign message to window to make sure the menu goes away.
	            if (clicked == ID_TRAY_EXIT)
	            {
	                // quit the application.
	                Shell_NotifyIcon(NIM_DELETE, &Data);
	                exit(0);
	            }
	        }
	    }
	    break;
        case WM_TIMER:
            {
                KillTimer(Window, ID_TIMER);
                if(timer == 0)
                {
                    StartCheck();
                }
                else if(timer == 1)
                    SetWindowPos(Window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
            }
            break;
        case WM_CLOSE:
            {
                exit(0);
            }
            break;
        default:
            DefWindowProc(hWnd, Message, wParam, lParam);
    }
}
ULONG_PTR ptr;
GdiplusStartupInput input;
int WINAPI WinMain(HINSTANCE Hin, HINSTANCE Hin1, LPSTR Args, int WinMode)
{
    GdiplusStartup(&ptr, &input, NULL);
    WNDCLASSEX WinClass = {0};
    WinClass.cbSize = sizeof(WNDCLASSEX);
    WinClass.lpszClassName = "SecurityWindow";
    WinClass.lpfnWndProc = WinFunc;
    WinClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    WinClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    WinClass.style = CS_NOCLOSE;
    if(!RegisterClassEx(&WinClass))
        MessageBox(NULL, "Error While Registering Window", "ERROR", NULL);
    if(!(Window = CreateWindow(WinClass.lpszClassName, "", WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, NULL, NULL, NULL, NULL)))
        MessageBox(NULL, "Error While Creating Window", "ERROR", NULL);
    ShowWindow(Window, SW_NORMAL);
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, &Keys, NULL, NULL);
    SetWindowLongPtr(Window, GWLP_HWNDPARENT, (LONG)GetDesktopWindow());
    SysTrayIcon();
    SetTimer(Window, ID_TIMER, 0, NULL);
    SetTimer(Window, ID_TIMER_TOPMOST, 0, NULL);
    while(GetMessage(&Msg, NULL, NULL, NULL))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
        if(Msg.message == WM_KEYDOWN)
        {
            SendMessage(Static, WM_KEYDOWN, Msg.wParam, Msg.lParam);
        }
    }
}

