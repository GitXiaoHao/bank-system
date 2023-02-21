//
// Created by yuhao on 2023/2/18.
//
#include "../include/sys.h"

void set_size(unsigned uCol, unsigned uLine) {
    char cmd[64];
    sprintf(cmd, "mode con cols=%d lines=%d", uCol, uLine);
    system(cmd);
}

void hide_console_cursor() {
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                         &cursor_info);
}


void change_system() {
    //改变控制台颜色
    system("color 70");
    //改变标题
    SetConsoleTitle(TITLE);
    //改变大小
    set_size(WIDTH, HEIGHT);
    //让鼠标无效化
    hide_console_cursor();
    //屏蔽控制台最小按钮和关闭按钮
    HWND hwnd = GetConsoleWindow();
    HMENU hmenu = GetSystemMenu(hwnd, false);
    RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
    LONG style = GetWindowLong(hwnd, GWL_STYLE);
    style &= ~(WS_MINIMIZEBOX);
    SetWindowLong(hwnd, GWL_STYLE, style);
    SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    ShowWindow(hwnd, SW_SHOWNORMAL);
    DestroyMenu(hmenu);
    ReleaseDC(hwnd, NULL);
}

void change_graph() {
    //初始化画布
    /*
    *   值             含义
        EX_DBLCLKS   在绘图窗口中支持鼠标双击事件。
        EX_NOCLOSE   禁用绘图窗口的关闭按钮。
        EX_NOMINIMIZE   禁用绘图窗口的最小化按钮。
        EX_SHOWCONSOLE   显示控制台窗口。
    */
    initgraph(GRAPH_WIDTH, GRAPH_HEIGHT, EX_NOCLOSE + EX_NOMINIMIZE);
    //设置标题
    SetWindowText(GetHWnd(), "银行业务模拟与离散事件模拟");//SetConsleTitle
    cleardevice();
    //设置背景色
    setbkcolor(WHITE);
}
