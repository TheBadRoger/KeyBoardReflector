#include <fstream>
#include <iostream>
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
#include <thread>
#include <atomic>
#include <sstream>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment(lib, "gdi32.lib")

using namespace std;

// 窗口控件ID
#define ID_BTN_BROWSE 1001
#define ID_BTN_START 1002
#define ID_BTN_STOP 1003
#define ID_EDIT_PATH 1004
#define ID_PROGRESS 1005
#define ID_LABEL_STATUS 1006
#define ID_LABEL_COUNTDOWN 1007
#define ID_CHECK_SMART 1008
#define ID_SPIN_DELAY 1009
#define ID_EDIT_DELAY 1010

// 全局变量
HWND g_hWnd;
HWND g_hEditPath;
HWND g_hBtnStart;
HWND g_hBtnStop;
HWND g_hBtnBrowse;
HWND g_hProgress;
HWND g_hLabelStatus;
HWND g_hLabelCountdown;
HWND g_hCheckSmart;
HWND g_hEditDelay;
HWND g_hSpinDelay;

atomic<bool> g_isRunning(false);
atomic<bool> g_shouldStop(false);
string g_filePath;
int g_delayMs = 10;

// 原程序的变量和函数
string u_shiftspc = "-=[];',./\\`";
string shiftspc = "_+{}|:\"<>?~";
string numshiftspc = ")!@#$%^&*(";
int reflist[256];

void initializer() {
    reflist['-'] = reflist['_'] = VK_OEM_MINUS;
    reflist['='] = reflist['+'] = VK_OEM_PLUS;
    reflist['['] = reflist['{'] = VK_OEM_4;
    reflist[']'] = reflist['}'] = VK_OEM_6;
    reflist['\\'] = reflist['|'] = VK_OEM_5;
    reflist[';'] = reflist[':'] = VK_OEM_1;
    reflist['\''] = reflist['\"'] = VK_OEM_7;
    reflist[','] = reflist['<'] = VK_OEM_COMMA;
    reflist['.'] = reflist['>'] = VK_OEM_PERIOD;
    reflist['/'] = reflist['?'] = VK_OEM_2;
    reflist['`'] = reflist['~'] = VK_OEM_3;
    reflist[0] = '0';
    reflist[1] = '1';
    reflist[2] = '2';
    reflist[3] = '3';
    reflist[4] = '4';
    reflist[5] = '5';
    reflist[6] = '6';
    reflist[7] = '7';
    reflist[8] = '8';
    reflist[9] = '9';
}

bool isCapsLkOn() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

void switch_lower() {
    if (isCapsLkOn()) {
        keybd_event(VK_CAPITAL, 0, 0, 0);
        keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
    }
}

void switch_upper() {
    if (!isCapsLkOn()) {
        keybd_event(VK_CAPITAL, 0, 0, 0);
        keybd_event(VK_CAPITAL, 0, KEYEVENTF_KEYUP, 0);
    }
}

void input_space() {
    keybd_event(VK_SPACE, 0, 0, 0);
    keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);
}

void input_enter() {
    keybd_event(VK_RETURN, 0, 0, 0);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}

void input_escape() {
    keybd_event(VK_ESCAPE, 0, 0, 0);
    keybd_event(VK_ESCAPE, 0, KEYEVENTF_KEYUP, 0);
}

void input_special(char kch) {
    if (u_shiftspc.find(kch) != u_shiftspc.npos) {
        keybd_event(reflist[kch], 0, 0, 0);
        keybd_event(reflist[kch], 0, KEYEVENTF_KEYUP, 0);
    }
    else if (shiftspc.find(kch) != shiftspc.npos) {
        keybd_event(VK_SHIFT, 0, 0, 0);
        keybd_event(reflist[kch], 0, 0, 0);
        keybd_event(reflist[kch], 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
        if (kch == '>') input_escape();
    }
    else if (numshiftspc.find(kch) != numshiftspc.npos) {
        keybd_event(VK_SHIFT, 0, 0, 0);
        keybd_event(reflist[(int)numshiftspc.find(kch)], 0, 0, 0);
        keybd_event(reflist[(int)numshiftspc.find(kch)], 0, KEYEVENTF_KEYUP, 0);
        keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
    }
}

void input_alphakey(char kch) {
    int keyid = toupper(kch);
    keybd_event(keyid, 0, 0, 0);
    keybd_event(keyid, 0, KEYEVENTF_KEYUP, 0);
}

void input_digit(char kch) {
    keybd_event(kch, 0, 0, 0);
    keybd_event(kch, 0, KEYEVENTF_KEYUP, 0);
}

// 设置状态文本
void SetStatusText(const wstring& text) {
    SetWindowTextW(g_hLabelStatus, text.c_str());
}

// 文件输入线程
void InputThread() {
    g_isRunning = true;
    g_shouldStop = false;

    // 禁用开始按钮，启用停止按钮
    EnableWindow(g_hBtnStart, FALSE);
    EnableWindow(g_hBtnStop, TRUE);
    EnableWindow(g_hBtnBrowse, FALSE);
    EnableWindow(g_hEditPath, FALSE);
    EnableWindow(g_hEditDelay, FALSE);
    EnableWindow(g_hSpinDelay, FALSE);

    // 倒计时
    for (int i = 3; i >= 1 && !g_shouldStop; i--) {
        wstring countdown = L"倒计时: " + to_wstring(i) + L" 秒";
        SetWindowTextW(g_hLabelCountdown, countdown.c_str());
        Sleep(1000);
    }

    if (!g_shouldStop) {
        SetWindowTextW(g_hLabelCountdown, L"");
        SetStatusText(L"正在输入...");

        // 打开文件并计算总字符数
        fstream targetfile(g_filePath, ios::in);
        if (!targetfile.is_open()) {
            SetStatusText(L"错误: 无法打开文件");
            goto cleanup;
        }

        // 获取文件大小
        targetfile.seekg(0, ios::end);
        streampos fileSize = targetfile.tellg();
        targetfile.seekg(0, ios::beg);

        targetfile >> noskipws;

        // 设置进度条范围
        SendMessage(g_hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
        SendMessage(g_hProgress, PBM_SETPOS, 0, 0);

        char tkey;
        long long processedBytes = 0;
        long long totalSize = (long long)fileSize;

        while (targetfile >> tkey && !g_shouldStop) {
            if (isalpha(tkey)) {
                if (islower(tkey)) {
                    switch_lower();
                    input_alphakey(tkey);
                }
                else if (isupper(tkey)) {
                    switch_upper();
                    input_alphakey(tkey);
                }
            }
            else if (isdigit(tkey)) {
                input_digit(tkey);
            }
            else if (tkey == ' ') input_space();
            else if (tkey == '\n') input_enter();
            else input_special(tkey);

            // 更新进度
            processedBytes++;
            int progress = (int)((processedBytes * 100) / totalSize);
            SendMessage(g_hProgress, PBM_SETPOS, progress, 0);

            // 延迟
            Sleep(g_delayMs);
        }

        targetfile.close();

        if (g_shouldStop) {
            SetStatusText(L"已停止");
        }
        else {
            input_enter();
            SetStatusText(L"完成！");
            SendMessage(g_hProgress, PBM_SETPOS, 100, 0);
            MessageBoxW(g_hWnd, L"输入完成！\n请检查结果并进行必要的调整。", L"完成", MB_OK | MB_ICONINFORMATION);
        }
    }
    else {
        SetStatusText(L"已取消");
        SetWindowTextW(g_hLabelCountdown, L"");
    }

cleanup:
    // 重新启用控件
    EnableWindow(g_hBtnStart, TRUE);
    EnableWindow(g_hBtnStop, FALSE);
    EnableWindow(g_hBtnBrowse, TRUE);
    EnableWindow(g_hEditPath, TRUE);
    EnableWindow(g_hEditDelay, TRUE);
    EnableWindow(g_hSpinDelay, TRUE);
    SendMessage(g_hProgress, PBM_SETPOS, 0, 0);

    g_isRunning = false;
}

// 浏览文件
void BrowseFile() {
    OPENFILENAMEA ofn;
    char szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = g_hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "文本文件\0*.txt\0所有文件\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameA(&ofn)) {
        SetWindowTextA(g_hEditPath, szFile);
        g_filePath = szFile;
    }
}

// 窗口过程
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
    {
        // 创建字体
        HFONT hFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"微软雅黑");

        HFONT hTitleFont = CreateFontW(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"微软雅黑");

        // 标题
        HWND hTitle = CreateWindowW(L"STATIC", L"键盘输入模拟器",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            10, 10, 460, 30, hWnd, NULL, NULL, NULL);
        SendMessage(hTitle, WM_SETFONT, (WPARAM)hTitleFont, TRUE);

        // 文件选择区域
        CreateWindowW(L"STATIC", L"选择文件:",
            WS_CHILD | WS_VISIBLE,
            20, 60, 80, 25, hWnd, NULL, NULL, NULL);

        g_hEditPath = CreateWindowW(L"EDIT", L"",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_READONLY,
            100, 60, 270, 25, hWnd, (HMENU)ID_EDIT_PATH, NULL, NULL);

        g_hBtnBrowse = CreateWindowW(L"BUTTON", L"浏览...",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            380, 60, 80, 25, hWnd, (HMENU)ID_BTN_BROWSE, NULL, NULL);

        // 设置区域
        CreateWindowW(L"STATIC", L"输入延迟:",
            WS_CHILD | WS_VISIBLE,
            20, 100, 80, 25, hWnd, NULL, NULL, NULL);

        g_hEditDelay = CreateWindowW(L"EDIT", L"10",
            WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,
            100, 100, 60, 25, hWnd, (HMENU)ID_EDIT_DELAY, NULL, NULL);

        g_hSpinDelay = CreateWindowW(UPDOWN_CLASS, NULL,
            WS_CHILD | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS,
            0, 0, 0, 0, hWnd, (HMENU)ID_SPIN_DELAY, NULL, NULL);
        SendMessage(g_hSpinDelay, UDM_SETBUDDY, (WPARAM)g_hEditDelay, 0);
        SendMessage(g_hSpinDelay, UDM_SETRANGE, 0, MAKELPARAM(1000, 0));
        SendMessage(g_hSpinDelay, UDM_SETPOS, 0, 10);

        CreateWindowW(L"STATIC", L"毫秒",
            WS_CHILD | WS_VISIBLE,
            170, 100, 40, 25, hWnd, NULL, NULL, NULL);

        // 提示信息
        HWND hTip = CreateWindowW(L"STATIC",
            L"提示: 使用前请关闭目标软件的智能提示、自动补全和自动缩进功能",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            20, 140, 440, 20, hWnd, NULL, NULL, NULL);

        // 进度条
        g_hProgress = CreateWindowW(PROGRESS_CLASS, NULL,
            WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
            20, 170, 440, 25, hWnd, (HMENU)ID_PROGRESS, NULL, NULL);

        // 状态标签
        g_hLabelStatus = CreateWindowW(L"STATIC", L"就绪",
            WS_CHILD | WS_VISIBLE | SS_LEFT,
            20, 205, 200, 25, hWnd, (HMENU)ID_LABEL_STATUS, NULL, NULL);

        g_hLabelCountdown = CreateWindowW(L"STATIC", L"",
            WS_CHILD | WS_VISIBLE | SS_RIGHT,
            260, 205, 200, 25, hWnd, (HMENU)ID_LABEL_COUNTDOWN, NULL, NULL);

        // 控制按钮
        g_hBtnStart = CreateWindowW(L"BUTTON", L"开始输入",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            120, 245, 100, 35, hWnd, (HMENU)ID_BTN_START, NULL, NULL);

        g_hBtnStop = CreateWindowW(L"BUTTON", L"停止",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_DISABLED,
            260, 245, 100, 35, hWnd, (HMENU)ID_BTN_STOP, NULL, NULL);

        // 设置字体
        HWND hChild = GetWindow(hWnd, GW_CHILD);
        while (hChild) {
            SendMessage(hChild, WM_SETFONT, (WPARAM)hFont, TRUE);
            hChild = GetWindow(hChild, GW_HWNDNEXT);
        }

        SendMessage(hTitle, WM_SETFONT, (WPARAM)hTitleFont, TRUE);
    }
    break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId) {
        case ID_BTN_BROWSE:
            BrowseFile();
            break;

        case ID_BTN_START:
        {
            char path[260];
            GetWindowTextA(g_hEditPath, path, 260);
            if (strlen(path) == 0) {
                MessageBoxW(hWnd, L"请先选择文件！", L"提示", MB_OK | MB_ICONWARNING);
                break;
            }

            // 获取延迟设置
            char delayStr[10];
            GetWindowTextA(g_hEditDelay, delayStr, 10);
            g_delayMs = atoi(delayStr);
            if (g_delayMs < 0) g_delayMs = 0;
            if (g_delayMs > 1000) g_delayMs = 1000;

            if (!g_isRunning) {
                thread inputThread(InputThread);
                inputThread.detach();
            }
        }
        break;

        case ID_BTN_STOP:
            g_shouldStop = true;
            break;

        case ID_EDIT_DELAY:
            if (HIWORD(wParam) == EN_CHANGE) {
                char delayStr[10];
                GetWindowTextA(g_hEditDelay, delayStr, 10);
                int delay = atoi(delayStr);
                if (delay >= 0 && delay <= 1000) {
                    g_delayMs = delay;
                }
            }
            break;
        }
    }
    break;

    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        SetBkMode(hdcStatic, TRANSPARENT);
        return (LRESULT)GetStockObject(WHITE_BRUSH);
    }
    break;

    case WM_DESTROY:
        g_shouldStop = true;
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 控制台版本的主函数（保留原功能）
void ConsoleMode(int argc, char* argv[]) {
    if (argc == 2) {
        initializer();
        cout << "使用前提醒：\n由于该软件的工作原理是将文本内容1：1复制，故建议关闭工作软件所有的智能提示、自动补全和自动缩进" << endl;
        cout << "复制进行期间最好不要随意使用键盘或者鼠标，否则可能会出现预料之外的结果\n" << endl;
        cout << "确认后按任意键继续..." << endl;
        getchar();

        for (int i = 3; i >= 1; i--) {
            cout << i << " 秒后开始复制。" << endl;
            Sleep(1000);
        }

        fstream targetfile(argv[1], ios::in);
        targetfile >> noskipws;
        char tkey;
        while (targetfile >> tkey) {
            if (isalpha(tkey)) {
                if (islower(tkey)) {
                    switch_lower();
                    input_alphakey(tkey);
                }
                else if (isupper(tkey)) {
                    switch_upper();
                    input_alphakey(tkey);
                }
            }
            else if (isdigit(tkey)) {
                input_digit(tkey);
            }
            else if (tkey == ' ') input_space();
            else if (tkey == '\n') input_enter();
            else input_special(tkey);
        }

        input_enter();
        cout << "复制内容不能保证在工作区上百分百一致，但仅需稍作修改即可\n" << endl;
        cout << "按任意键继续..." << endl;
        getchar();
    }
}

// 主函数 - 支持控制台和GUI两种模式
int main(int argc, char* argv[]) {
    // 如果有命令行参数，使用控制台模式
    if (argc == 2) {
        ConsoleMode(argc, argv);
        return 0;
    }

    // 否则启动GUI模式
    HINSTANCE hInstance = GetModuleHandle(NULL);

    // 初始化
    initializer();
    InitCommonControls();

    // 注册窗口类
    WNDCLASSEXW wcex = { 0 };
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"KeyboardSimulator";
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassExW(&wcex);

    // 创建窗口
    g_hWnd = CreateWindowW(L"KeyboardSimulator", L"键盘输入模拟器",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 340,
        NULL, NULL, hInstance, NULL);

    if (!g_hWnd) {
        return FALSE;
    }

    // 居中窗口
    RECT rect;
    GetWindowRect(g_hWnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    SetWindowPos(g_hWnd, NULL, (screenWidth - width) / 2, (screenHeight - height) / 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    ShowWindow(g_hWnd, SW_SHOW);
    UpdateWindow(g_hWnd);

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
