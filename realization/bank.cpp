//
// Created by yuhao on 2023/2/18.
//


#include "../include/bank.h"

//判断是否暂停
static bool is_stop = false;
//目前柜台数量
static int counter_number = 0;
//存放柜台的结构体数组
static counter *counters[COUNTER_COUNT];
//排队离的距离
static const int distance = 10;
//当前的速度
static const int speed = 10;
//产生人的速度
static const int produce_human_minute = 30;
//人的半径
static const int human_radius = 20;
//目前到了多少人
static int now_human_number = 0;
//人
static human *humans[HUMAN_NUMBER];
//白板左边x
static const int left_x = 200;
//刷新次数
static long long int flush_number = 0;
//判断是否是第一次添加柜台
static bool is_first = true;
////判断在柜台办理业务的人已经增加了或者减少了
static bool handle_human_add_lower = false;

void welcome(bool *pBoolean) {
    //获取当前时间
    int minute = get_now_hour();
    printf("                    ***-------------------------------------------------------------------***\n");
    printf("                                                                                               \n");
    printf("						欢迎来到					\n");
    //Sleep(SLEEP);//沉睡1.5秒
    printf("						我的银行					\n");
    printf("                                                                                               \n");
    printf("                    ***-------------------------------------------------------------------***\n");
    //Sleep(SLEEP);//沉睡1.5秒
    printf("                    =========================================================================\n");
    printf("                    *                                                                       *\n");
    printf("                    *                       本银行营业时间为早上9点-下午17点                *\n");
    printf("                    *                       节假日不放假                                    *\n");
    if (minute > CLOSING_TIME || minute < WORK_HOURS) {
        //不在营业时间
        printf("                    *                       当前为休息时间                                  *\n");
        printf("                    *                       请点击屏幕退出银行   	  		    *\n");
    } else {
        *pBoolean = true;
        printf("                    *                       当前为营业时间                                  *\n");
        printf("                    *                       请点击屏幕进入银行   	  		    *\n");
    }
    printf("                    *                                                                       *\n");
    printf("                    =========================================================================\n");
    //Sleep(SLEEP);//1.5秒后进入程序
}


//银行名字
void graph_typeface() {
    //设置字体颜色
    settextcolor(BLACK);//设置字体颜色
    settextstyle(60, 20, _T("华文新魏"));//设置字号、字体
    outtextxy(380, 20, _T("China Bank"));//设置要输出的内容
}

//矩形
void graph_rectangle() {
    //设置划线颜色
    setlinecolor(BLACK);
    //画矩形
    rectangle(left_x, 1, 800, 150);
}

//柜台
void graph_counter() {
    //设置划线颜色
    setlinecolor(BLACK);
    //画矩形
    int i;
    for (i = 0; i < COUNTER_COUNT; ++i) {
        int x = left_x + ((i + 1) * 40) + (i * 100);
        int down_y = 150;
        int right_x = x + 80;
        //画柜台
        rectangle(x, 100, right_x, down_y);
        if (is_first) {
            counter *counter1;
            counter1 = (struct counter *) malloc(sizeof(struct counter));
            counter1->left_x = x;
            counter1->down_y = down_y;
            counter1->number = 0;
            counter1->right_x = right_x;
            counters[(counter_number)] = (struct counter *) malloc(sizeof(struct counter));
            counters[(counter_number)++] = counter1;
            if (counter_number == COUNTER_COUNT) {
                is_first = false;
            }
        }
        //写字
        settextstyle(20, 0, _T("华文新魏"));//设置字号、字体
        char counter[10];
        //拼接字符串
        sprintf(counter, "柜台%d", i + 1);
        //输出
        outtextxy(x + 15, 110, _T(counter));//设置要输出的内容
    }
}

//返回当前柜台的人数最少的柜台 从0开始
int now_counter_human_number_least() {
    int i;
    int min = HUMAN_NUMBER;
    int least = 0;
    for (i = 0; i < COUNTER_COUNT; ++i) {
        //遍历柜台
        //看哪个柜台的人少;
        if (min > counters[i]->number) {
            min = counters[i]->number;
            least = i;
        }
    }
    return least;
}

//下面的一条线
static int height1 = GRAPH_HEIGHT - 20;
//上面的一条线
static int height2 = GRAPH_HEIGHT - 90;
//左边通道最右边的部分
static int width = 160;
//下面一条线的最右边的部分
static int longer = width + 80;
//高度
static int level = GRAPH_HEIGHT - 200;
///退出的通道
//通道距离
static int export_distance = 80;
//左边通道
static int export_left = 20;
//右边通道的左边
static int export_right = GRAPH_WIDTH - (GRAPH_WIDTH / 6);
//高度
static int high = 100;

//通道
void graph_passageway() {
    //设置划线颜色
    setlinecolor(BLACK);
    //划线
    ////左边通道
    line(0, height1, longer, height1);
    line(0, height2, width, height2);

    line(width, height2, width, level);
    line(longer, height1, longer, level);
    ////右边通道
    //下面的一条线
    line(GRAPH_WIDTH - (GRAPH_WIDTH / 5), height1, GRAPH_WIDTH, height1);
    //上面的一条线
    line(GRAPH_WIDTH - (GRAPH_WIDTH / 5), height2, GRAPH_WIDTH, height2);
    ///两个出口 左边一个右边一个
    //左边
    line(export_left, 0, export_left, high);
    line(export_left + export_distance, 0, export_left + export_distance, high);
    //右边
    line(export_right, 0, export_right, high);
    line(export_right + export_distance, 0, export_right + export_distance, high);
    //高度
    ////上面一条线
    //line(GRAPH_WIDTH - (GRAPH_WIDTH / 4), height1, GRAPH_WIDTH - (GRAPH_WIDTH / 4), level);
    ////下面一条线
    //line(GRAPH_WIDTH - (GRAPH_WIDTH / 6), height2, GRAPH_WIDTH - (GRAPH_WIDTH / 6), level);
}

//判断人是否到通道顶端了 需要往上走
bool is_top(human *pHuman) {
    //左边
    if (pHuman->x + pHuman->radius >= longer - 20) {
        return true;
    }
    return false;
}

//判断是否已经出了通道
bool is_exit_passageway(human *pHuman) {
    //他的底部要出去
    if (pHuman->y + pHuman->radius <= level) {
        return true;
    }
    return false;
}

void is_now_counter_first(human *pHuman) {
    if (pHuman->now_counter_number == 0) {
        pHuman->is_start = true;
        //设置办理业务时间
        pHuman->start_minute = get_now_minute();
        pHuman->start_hour = get_now_hour();
    }
}

void find_counter(human *pHuman) {
    counter *target_counter = pHuman->target_counter;
    //先从x开始寻找
    int x = ((target_counter->right_x) + (target_counter->left_x)) / 2;
    if (x > pHuman->x) {
        //x轴还没有走完
        pHuman->x = pHuman->x + pHuman->speed;
        return;
    }
    //判断y轴
    int y = (target_counter->down_y) + (distance * target_counter->number) +
            (target_counter->number * (pHuman->radius * 2));
    if (pHuman->y - pHuman->radius > y) {
        pHuman->y = pHuman->y - pHuman->speed;
        return;
    }
    if (!pHuman->is_find_counter) {
        pHuman->is_find_counter = true;
        //你是第几个
        pHuman->now_counter_number = target_counter->number;
        //目标柜台数量+1
        target_counter->humans[target_counter->number] = (struct human *) malloc(sizeof(struct human));
        target_counter->humans[target_counter->number++] = pHuman;
        //改变了
        handle_human_add_lower = true;
        //判断自己是不是第一个
        is_now_counter_first(pHuman);
    }

}

void exit_bank(human *pHuman) {
    int target_x = 0;
    int counter_num = pHuman->counter_num;
    //判断当前是哪个柜台
    switch (counter_num) {
        case 0:
            //柜台1
            //先判断x
            target_x = (export_left + (distance / 2));
            if (pHuman->x > target_x) {
                pHuman->x = pHuman->x - pHuman->speed;
            } else {
                //x到了 直接y
                pHuman->y = pHuman->y - pHuman->speed;
            }
            break;
        case 1:
        case 2:
            //柜台2
            //以窗口中间为目标
            target_x = GRAPH_WIDTH / 2;
            if (pHuman->x < target_x) {
                //看是柜台1还是柜台2
                if(counter_num == 1){
                    pHuman->x = pHuman->x + pHuman->speed;
                }else if(counter_num == 2){
                    //柜台3
                    pHuman->x = pHuman->x - pHuman->speed;
                }
            } else {
                //y
                if ((height2 + height1) / 2 > pHuman->y) {
                    pHuman->y = pHuman->y + pHuman->speed;
                } else {
                    pHuman->x = pHuman->x + pHuman->speed;
                }
            }
            break;
        case 3:
        default:
            //柜台4
            //先判断x
            target_x = (export_right + (distance / 2));
            if (pHuman->x < target_x) {
                pHuman->x = pHuman->x + pHuman->speed;
            } else {
                //y
                pHuman->y = pHuman->y - pHuman->speed;
            }
            break;
    }
}

//已经完成了业务处理
void complete_business(human *pHuman) {
    //设置为已经办完
    pHuman->is_success = true;
    //要从柜台删除它 设置完成时间
    pHuman->end_minute = get_now_minute();
    pHuman->end_hour = get_now_hour();
    //从柜台删除他
    counter *now_counter = counters[pHuman->counter_num];
    //让每一个人现在的排队数都--
    for (int i = 0; i < now_counter->number; ++i) {
        --((now_counter->humans[i])->now_counter_number);
    }
    //当前柜台的排队人数的数量--
    --(now_counter->number);
    //改变
    handle_human_add_lower = true;
}

//办理业务
void handle_the_business(human *pHuman) {
    //处理时间
    if (pHuman->start_hour == get_now_hour()) {
        //如果小时相同
        //判断分钟
        if (pHuman->start_minute + pHuman->processing_time < get_now_minute()) {
            //已经办完业务
            complete_business(pHuman);
        }
    } else {
        //如果到了下一个小时时刻
        if (pHuman->start_minute + pHuman->processing_time - 60 < get_now_minute()) {
            //已经办完业务
            complete_business(pHuman);
        }
    }
}

void change_human() {
    int i;
    for (i = 0; i < now_human_number; ++i) {
        //画笔改成黑色
        setfillcolor(BLUE);
        human *now_human = humans[i];
        //以画圆为准
        solidcircle(now_human->x, now_human->y, now_human->radius);
        if(handle_human_add_lower){
            ////开始寻找柜台
            find_counter(now_human);
        }
        if(i +1 == now_human_number){
            handle_human_add_lower = false;
        }
        //判断是否已经成功
        if (now_human->is_success) {
            //已经成功办完
            ///退出银行
            exit_bank(now_human);
            continue;
        }
        //判断是否已经找到了柜台
        if (now_human->is_find_counter) {
            if (now_human->is_start) {
                handle_the_business(now_human);
            } else {
                //判断自己是不是第一个
                is_now_counter_first(now_human);
            }
            continue;
        }
        //判断是否已经出了通道
        if (now_human->is_exit_passageway) {
            //出了通道
            //设置目标柜台
            if (!now_human->target_counter) {
                int least = now_counter_human_number_least();
                now_human->target_counter = counters[least];
                //看是哪个柜台
                now_human->counter_num = least;
            }
            ////开始寻找柜台
            find_counter(now_human);
            ///必须continue 目前还在循环中
            continue;
        }
        //判断是否已经往上走了
        if (now_human->line_up) {
            //判断是否已经出了通道
            if (is_exit_passageway(now_human)) {
                //已经出了通道
                now_human->is_exit_passageway = true;
                now_human->line_up = false;
            }
            now_human->y = now_human->y - now_human->speed;
            continue;
        }
        //判断是否到通道顶端了
        if (is_top(now_human) && !now_human->is_exit_passageway) {
            //如果到了
            now_human->line_up = true;
        } else {
            //如果是左边 就x ++
            now_human->x = now_human->x + now_human->speed;
        }
    }
}

//添加人
void graph_human() {
    change_human();
    //判断当前的人数
    if (now_human_number < HUMAN_NUMBER) {
        //生成随机数
        int num = produce_human_minute;
        if (flush_number % num == 0) {
            human *human1;
            human1 = (struct human *) malloc(sizeof(struct human));
            human1->x =
            human1->start_minute =
            human1->start_hour =
            human1->now_counter_number =
            human1->counter_num =
            human1->end_hour =
            human1->end_minute = 0;
            human1->speed = speed;
            human1->y = (height2 + height1) / 2;
            human1->radius = human_radius;
            human1->line_up =
            human1->is_start =
            human1->is_exit_passageway =
            human1->is_success =
            human1->is_find_counter = false;
            human1->target_counter = NULL;
            //处理时间
            int processing_time = rand() % 2 + 1;
            human1->processing_time = processing_time;
            //加入数组中
            humans[now_human_number++] = human1;
        }
    }
}

void graph_time() {
    //设置字体颜色
    settextcolor(BLACK);//设置字体颜色
    settextstyle(20, 10, _T("宋体"));//设置字号、字体
    char now_time[50];
    if (flush_number % 2 == 0) {
        sprintf(now_time, "%02d   %02d", get_now_hour(), get_now_minute());
    } else {
        sprintf(now_time, "%02d : %02d", get_now_hour(), get_now_minute());
    }
    outtextxy(720, 70, _T(now_time));//设置要输出的内容
}

void graph_main() {
    //画框
    graph_rectangle();
    //写字
    graph_typeface();
    //画时间
    graph_time();
    //画柜台
    graph_counter();
    //画通道
    graph_passageway();
    //画人
    graph_human();
}


void graph_theme() {
    //改变画框
    change_graph();
    //开始绘画，解决频闪
    BeginBatchDraw();
    while (!is_stop) {
        flush_number = flush_number + 1;
        //long long int 范围2147483647
        if (flush_number >= 2100000000) {
            flush_number = 1;
        }
        //魔鬼细节： 这里必须先清屏再去画图，因为BeginBatchDraw();是吧每一次作图内容存入进去
        //先cleardevice();清屏，然后再去作图，最后当遇到FlushBatchDraw();或者EndBatchDraw();的时候
        //就可以把帧图显示到屏幕上，如果反过来的话最后显示的是 cleardevice(); 清屏后的黑屏幕
        cleardevice();
        //画主题板
        graph_main();
        //开始
        Sleep(FLASH_SLEEP);
        FlushBatchDraw();//这里也可以用EndBatchDraw();
    }
    EndBatchDraw();
    //关闭拦截
    getchar();
    //关闭画布
    closegraph();
}

