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

void no_big_small() {
    HWND hWnd = GetConsoleWindow(); //获取窗口句柄
    long long int sty = GetWindowLongPtrA(hWnd, GWL_STYLE); //获取窗口样式
    sty = sty & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX; //去除可变化大小,最大,最小化按钮,~是取反,&是与,这是位运算相关的知识了
    SetWindowLongPtrA(hWnd, GWL_STYLE, sty); //设置窗体不可更改大小,不可最大化
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
    //让控制台不能放大和缩小
    no_big_small();
}

void change_graph() {
    //初始化画布
    initgraph(GRAPH_WIDTH, GRAPH_HEIGHT);
    //设置标题
    SetWindowText(GetHWnd(), "银行业务模拟与离散事件模拟");//SetConsleTitle
    cleardevice();
    //设置背景色
    setbkcolor(WHITE);
}
