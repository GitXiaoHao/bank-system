//
// Created by yuhao on 2023/2/18.
//

#include "stdio.h"
#include "times.h"
#include "stdlib.h"
#include <synchapi.h>
#include <graphics.h>
#include "sys.h"
//沉睡时间
#define SLEEP 1500
//上班时间
#define WORK_HOURS 9
//下班时间
#define CLOSING_TIME 24
//柜台数量
#define COUNTER_COUNT 4
//刷新时间
#define FLASH_SLEEP 300
//一天的人数
#define HUMAN_NUMBER 100
//欢迎界面
void welcome(bool *pBoolean);

//画主体
void graph_theme();

//人
typedef struct human {
    int x;
    int y;
    int radius;
    //开始办理业务时间 是按分钟算的
    int start_minute;
    //按小时算
    int start_hour;
    //完成的分钟
    int end_minute;
    //完成的小时
    int end_hour;
    //速度
    int speed = 0;
    //是当前柜台的第几个
    int now_counter_number;
    //第几个柜台
    //// 0 是第一个 1是第二个 2是第三个 3是第四个
    int counter_num;
    //处理时间
    int processing_time;
    //是否已经向上走
    bool line_up;
    ////如果是否已经办完业务
    bool is_success;
    //是否出了通道
    bool is_exit_passageway;
    //已经找到了柜台
    bool is_find_counter;
    //是否开始办理业务
    bool is_start;
    //目标的柜台
    struct counter *target_counter;
} human;

//柜台
typedef struct counter {
    //下面的距离
    int down_y;
    //左边的x
    int left_x;
    //右边的距离
    int right_x;
    //一个柜台多少人
    int number;
    struct human *humans[HUMAN_NUMBER / COUNTER_COUNT];
} counter;


