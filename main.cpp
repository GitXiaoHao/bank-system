
#include "include/bank.h"
bool is_work = false;
int main() {
    srand((int)time(NULL));  //用时间作为种子对随机数进行操作
    ////改变控制台
    change_system();
    ////欢迎界面
    welcome(&is_work);
    system("pause");
    //判断是否在上班
    if (is_work) {
        graph_theme();
    }
    return 0;
}
