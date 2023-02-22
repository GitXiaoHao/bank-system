//
// Created by yuhao on 2023/2/18.
//
#include "../include/sys.h"



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
