//
// Created by yuhao on 2023/2/18.
//

#include "stdio.h"
#include "times.h"
#include "stdlib.h"
#include <synchapi.h>
#include <pthread.h>
#include "sys.h"
#include "math.h"
//柜台数量
#define COUNTER_COUNT 4
//刷新时间
#define FLASH_SLEEP 100
//一天的人数
#define HUMAN_NUMBER 100
//人
typedef struct human {
    char name[50] = "";
    int x = 0;
    int y = 0;
    int radius = 0;
    //开始办理业务时间 是按分钟算的
    int start_minute = 0;
    //按小时算
    int start_hour = 0;
    //按秒算
    int start_second = 0;
    //完成的分钟
    int end_minute = 0;
    //完成的小时
    int end_hour = 0;
    //完成的秒
    int end_second = 0;
    //速度
    int speed = 0;
    //是当前柜台的第几个
    int now_counter_number = 0;
    //第几个柜台
    //// 0 是第一个 1是第二个 2是第三个 3是第四个
    int counter_num = 0;
    //处理时间
    int processing_time = 0;
    //是否已经向上走
    bool is_line_up = false;
    ////如果是否已经办完业务
    bool is_success = false;
    //是否出了通道
    bool is_exit_passageway = false;
    //已经找到了柜台
    bool is_find_counter = false;
    //是否开始办理业务
    bool is_start = false;
    //产生时间
    char produce_time[50] = "";
    //离开时间
    char leave_time[50] = "";
    //目标的柜台
    struct counter *target_counter = NULL;
} human;

//柜台
typedef struct counter {
    //柜台名
    char name[50];
    //下面的距离
    int down_y;
    //左边的x
    int left_x;
    //右边的距离
    int right_x;
    //一个柜台多少人
    int number;
    //共处理的多少个人
    int count_number;
    //共处理的多少时间
    int count_all_time;
    //设置当前柜台需要处理的时间
    int count_time;
    struct human *humans[HUMAN_NUMBER / COUNTER_COUNT];
} counter;




//直接调用 进行绘图
static void graph_main();

//画时间
void graph_time();

//画人
void graph_human();

//画按钮
void graph_button();

//画柜台
void graph_counter();

//画银行名
void graph_typeface();

//画银行矩形
void graph_rectangle();

//画通道
void graph_passageway();

//按钮的绘制
void button(int x, int y, int w, int h, TCHAR *text);

//拼接字符串
void my_str_splice(char *msg);

//返回当前柜台的人数最少的柜台 从0开始
int now_counter_human_number_least();

//判断人是否到通道顶端了 需要往上走
bool is_top(human *pHuman);

//判断是否已经出了通道
bool is_exit_passageway(human *pHuman);

//判断在当前柜台 这个人是不是第一个
void is_now_counter_first(human *pHuman);

//当前的人找柜台
void find_counter(human *pHuman);

//判断已经处理完业务的人是否已经走完出口通道
bool is_out(human *pHuman);

//删除人
void remove_human(human *pHuman);

//在退出银行的路上
void exit_bank(human *pHuman);

//记录柜台
void record_counter();

//记录人
void record_human();

//关闭时进行记录进行记录
void record_all();

//已经完成了业务处理
void complete_business(human *pHuman);

//办理业务(现在是判断时间)
void handle_the_business(human *pHuman);

//改变人的路线或者到柜台
void change_human();

//增加按钮的监听效果
void *add_button_listen(void *pVoid1);

//画主体
[[noreturn]] void *graph_theme(void *pVoid);

