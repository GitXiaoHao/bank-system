//
// Created by yuhao on 2023/2/18.
//


#include "../include/bank.h"

//文件读写
static FILE *file;
//写的东西
static char information[1000];
//目前柜台数量
static int counter_number = 0;
//一共到了多少人
static int now_human_number = 0;
//剩余多少人
static int leave_human_number = 0;

//当前的速度
static const int human_speed = 4;
//白板左边x
static const int left_x = 200;
//排队离的距离
static const int distance = 10;
//人的半径
static const int human_radius = 20;
//处理时间
static const int processing_time = 10;
//产生人的速度
///当前已在函数中转为了随机数
static int produce_human_minute = 200 * processing_time;

//刷新次数
static long long int flush_number = 0;

//左边通道的下面一条线
static int get_into_down_line = GRAPH_HEIGHT - 20;
//上面的一条线
static int get_into_up_line = GRAPH_HEIGHT - 90;
//左边通道最右边的部分
static int get_into_up_right_line = 160;
//下面一条线的最右边的部分
static int get_into_down_right_line = get_into_up_right_line + 80;
//高度
static int get_into_level = GRAPH_HEIGHT - 200;
///退出的通道
//通道距离
static int export_distance = 80;
//上面的左边通道
static int export_left = 20;
//右边通道的左边
static int export_right = GRAPH_WIDTH - (GRAPH_WIDTH / 6);
//上面通道的高度
static int get_out_up_high = 100;

//判断是否是第一次添加柜台
static bool is_first = true;
//判断是否暂停
static bool is_stop = false;
//判断是否开始
static bool is_start = false;
//是否开启了线程
static bool is_thread = false;

//人
static human *humans[HUMAN_NUMBER];
//存放柜台的结构体数组
static counter *counters[COUNTER_COUNT];
//进行记录
static human *record_humans[HUMAN_NUMBER];

//按钮的宽度
static const int button_width = 120;
//按钮的高度
static const int button_height = 50;
//按钮的y坐标
static const int button_y = GRAPH_HEIGHT - (button_height * 4);
//暂停按钮和结束按钮的距离
static const int stop_start_button_distance = button_height + 20;


void graph_main() {
    //画框
    graph_rectangle();
    //写字
    graph_typeface();
    //画柜台
    graph_counter();
    //画时间
    graph_time();
    //画通道
    graph_passageway();
    //画人
    graph_human();
    //画按钮
    graph_button();
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
            counter1->count_time =
            counter1->count_number =
            counter1->count_all_time =
            counter1->number = 0;
            counter1->right_x = right_x;
            sprintf(counter1->name, "柜台%d\t", i + 1);
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

//通道
void graph_passageway() {
    //设置划线颜色
    setlinecolor(BLACK);
    //划线
    ////左边通道
    line(0, get_into_down_line, get_into_down_right_line, get_into_down_line);
    line(0, get_into_up_line, get_into_up_right_line, get_into_up_line);

    line(get_into_up_right_line, get_into_up_line, get_into_up_right_line, get_into_level);
    line(get_into_down_right_line, get_into_down_line, get_into_down_right_line, get_into_level);
    ////右边通道
    setlinecolor(RED);
    //下面的一条线
    line(GRAPH_WIDTH - (GRAPH_WIDTH / 5), get_into_down_line, GRAPH_WIDTH, get_into_down_line);
    //上面的一条线
    line(GRAPH_WIDTH - (GRAPH_WIDTH / 5), get_into_up_line, GRAPH_WIDTH, get_into_up_line);
    ///两个出口 左边一个右边一个
    //左边
    line(export_left, 0, export_left, get_out_up_high);
    line(export_left + export_distance, 0, export_left + export_distance, get_out_up_high);
    //右边
    line(export_right, 0, export_right, get_out_up_high);
    line(export_right + export_distance, 0, export_right + export_distance, get_out_up_high);
    //高度
    ////上面一条线
    //line(GRAPH_WIDTH - (GRAPH_WIDTH / 4), height1, GRAPH_WIDTH - (GRAPH_WIDTH / 4), level);
    ////下面一条线
    //line(GRAPH_WIDTH - (GRAPH_WIDTH / 6), height2, GRAPH_WIDTH - (GRAPH_WIDTH / 6), level);
}

//画按钮
void graph_button() {
    char stop[50] = "暂停";
    char start[50] = "开始";
    char exit[50] = "退出";
    button(0, button_y, button_width, button_height, stop);
    button(0, button_y - stop_start_button_distance, button_width, button_height, start);
    button(GRAPH_WIDTH - button_width, button_y, button_width, button_height, exit);
}

//添加人
void graph_human() {
    change_human();
    //判断当前的人数
    if (now_human_number < HUMAN_NUMBER) {
        //转为随机数
        int produce_human_time = rand() % produce_human_minute + 1;
        //随机生成人
        if (flush_number % produce_human_time == 0) {
            human *human1;
            human1 = (struct human *) malloc(sizeof(struct human));
            human1->x =
            human1->start_minute =
            human1->start_hour =
            human1->start_second =
            human1->now_counter_number =
            human1->counter_num =
            human1->end_hour =
            human1->end_second =
            human1->end_minute = 0;
            human1->is_line_up =
            human1->is_start =
            human1->is_exit_passageway =
            human1->is_success =
            human1->is_find_counter = false;
            human1->target_counter = NULL;
            human1->speed = human_speed;
            human1->y = (get_into_up_line + get_into_down_line) / 2;
            human1->radius = human_radius;
            sprintf(human1->name, "客户%-2d\t", now_human_number + 1);
            sprintf(human1->leave_time, "");
            //产生时间
            sprintf(human1->produce_time, "%02d:%02d:%02d\t", get_now_hour(), get_now_minute(), get_now_second());
            //处理时间
            human1->processing_time = rand() % processing_time + 1;
            //加入数组中
            humans[leave_human_number++] = human1;
            record_humans[now_human_number++] = human1;
        }
    }
}

void graph_time() {
    //设置字体颜色
    settextcolor(BLACK);//设置字体颜色
    settextstyle(20, 10, _T("宋体"));//设置字号、字体
    char now_time[50];
    //if (flush_number % 2 == 0) {
    //    sprintf(now_time, "%02d   %02d", get_now_hour(), get_now_minute());
    //} else {
    sprintf(now_time, "%02d : %02d", get_now_hour(), get_now_minute());
    //}
    outtextxy(720, 70, _T(now_time));//设置要输出的内容
}

//画主体
[[noreturn]] void *graph_theme(void *pVoid) {
    srand((int) time(NULL));  //用时间作为种子对随机数进行操作
    //初始化文件
    if ((file = fopen("E://bank.txt", "a+")) == NULL) {
        file = fopen("C://bank.txt", "a+");
    }
    //改变画框
    change_graph();
    //开始绘画，解决频闪
    BeginBatchDraw();
    sprintf(information, "\n\n本次运行从%d年 %d月 %d日 %d点 %d分 %d秒 开始\n", get_now_year(), get_now_month(),
            get_now_day(),
            get_now_hour(), get_now_minute(), get_now_second());
    flush:
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
        //开线程
        if (!is_thread) {
            is_thread = true;
            //开个线程
            pthread_t button_listen;
            pthread_create(&button_listen, NULL, add_button_listen, NULL);
            //pthread_join(th2,NULL);
        }
    }
    //关闭拦截
    while (true) {
        if (is_start) {
            goto flush;
        }
    }
    //EndBatchDraw();
    ////关闭画布
    //closegraph();
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

//判断人是否到通道顶端了 需要往上走
bool is_top(human *pHuman) {
    //左边
    if (pHuman->x + pHuman->radius >= get_into_down_right_line - 20) {
        return true;
    }
    return false;
}

//判断是否已经出了通道
bool is_exit_passageway(human *pHuman) {
    //他的底部要出去
    if (pHuman->y + pHuman->radius <= get_into_level) {
        return true;
    }
    return false;
}

//判断在当前柜台 这个人是不是第一个
void is_now_counter_first(human *pHuman) {
    if (pHuman->now_counter_number == 0) {
        pHuman->is_start = true;
        //设置办理业务时间
        pHuman->start_minute = get_now_minute();
        pHuman->start_hour = get_now_hour();
        pHuman->start_second = get_now_second();
    }
}

//当前的人找柜台
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
    int y = (target_counter->down_y) + (distance * pHuman->now_counter_number) +
            (pHuman->now_counter_number * (pHuman->radius * 2));
    if (pHuman->y - pHuman->radius > y) {
        pHuman->y = pHuman->y - pHuman->speed;
        return;
    }
    if (!pHuman->is_find_counter) {
        pHuman->is_find_counter = true;
        //一共处理多少个人
        ++(target_counter->count_number);
        //一共处理的多少时间
        target_counter->count_all_time += pHuman->processing_time;
        //当前需要处理多少时间
        target_counter->count_time += pHuman->processing_time;
        //判断自己是不是第一个
        is_now_counter_first(pHuman);
    }
}

//判断是否出界
bool is_out(human *pHuman) {
    if ((pHuman->y + pHuman->radius) < 0 ||
        (pHuman->x - pHuman->radius) > GRAPH_WIDTH) {
        //从上面走的 从右面出去的
        return true;
    }
    return false;
}

//删除人
void remove_human(human *pHuman) {
    int i;
    for (i = 0; i < leave_human_number; ++i) {
        if (pHuman == humans[i]) {
            int j;
            for (j = i; j < leave_human_number - 1; ++j) {
                humans[j] = humans[j + 1];
            }
            break;
        }
    }
    //当前的人数--
    --leave_human_number;
}

void exit_bank(human *pHuman) {
    int target_x;
    int counter_num = pHuman->counter_num;
    //判断是否已经出界
    if (is_out(pHuman)) {
        //出界了
        ///删除
        remove_human(pHuman);
        //记录
        sprintf(pHuman->leave_time, "%2d:%2d:%2d\t", get_now_hour(), get_now_minute(), get_now_second());
        return;
    }
    //判断当前是哪个柜台
    switch (counter_num) {
        case 0:
            //柜台1
            //先判断x
            target_x = (export_left + (export_distance / 2));
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
            //如果到了通道
            if ((get_into_up_line + get_into_down_line) / 2 == pHuman->y) {
                pHuman->x = pHuman->x + pHuman->speed;
            } else if ((get_into_up_line + get_into_down_line) / 2 > pHuman->y) {
                //还没有到通道
                if (pHuman->x < target_x) {
                    //柜台2
                    pHuman->x = pHuman->x + pHuman->speed;
                } else if (pHuman->x > target_x) {
                    //柜台三
                    pHuman->x = pHuman->x - pHuman->speed;
                } else {
                    //x平行时，就移动y
                    pHuman->y = pHuman->y + pHuman->speed;
                }
            }
            break;
        case 3:
        default:
            //柜台4
            //先判断x
            target_x = (export_right + (export_distance / 2));
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
    pHuman->end_second = get_now_second();
    //从柜台删除他
    counter *now_counter = counters[pHuman->counter_num];
    int i;
    //从柜台删除
    for (i = 0; i < now_counter->number; ++i) {
        if (pHuman == now_counter->humans[i]) {
            int j;
            for (j = i; j < now_counter->number - 1; ++j) {
                now_counter->humans[j] = now_counter->humans[j + 1];
            }
            break;
        }
    }
    //当前柜台的排队人数的数量--
    --(now_counter->number);
    //让每一个人现在的排队数都--
    for (i = 0; i < now_counter->number; ++i) {
        --((now_counter->humans[i])->now_counter_number);
        //把这个柜台都设为没有找到
        (now_counter->humans[i])->is_find_counter = false;
    }
    now_counter->count_time -= pHuman->processing_time;
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

//改变人的路线或者到柜台
void change_human() {
    int i;
    //画笔改成黑色
    setfillcolor(BLUE);
    for (i = 0; i < leave_human_number; ++i) {
        human *now_human = humans[i];
        //以画圆为准
        solidcircle(now_human->x, now_human->y, now_human->radius);
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
                counter *target_counter = counters[least];
                //你是第几个
                now_human->now_counter_number = target_counter->number;
                //目标柜台数量+1
                target_counter->humans[target_counter->number] = (struct human *) malloc(sizeof(struct human));
                //将人加进去
                target_counter->humans[target_counter->number++] = now_human;
                now_human->target_counter = target_counter;
                //看是哪个柜台
                now_human->counter_num = least;
            }
            ////开始寻找柜台
            find_counter(now_human);
            ///必须continue 目前还在循环中
            continue;
        }
        //判断是否已经往上走了
        if (now_human->is_line_up) {
            //判断是否已经出了通道
            if (is_exit_passageway(now_human)) {
                //已经出了通道
                now_human->is_exit_passageway = true;
                now_human->is_line_up = false;
            }
            now_human->y = now_human->y - now_human->speed;
            continue;
        }
        //判断是否到通道顶端了
        if (is_top(now_human) && !now_human->is_exit_passageway) {
            //如果到了
            now_human->is_line_up = true;
        } else {
            //如果是左边 就x ++
            now_human->x = now_human->x + now_human->speed;
        }
    }
}

//关闭时进行记录进行记录
void record_all() {
    if (now_human_number == 0) {
        strcat(information, "本次无客户\n");
    }else {
        record_human();
        //要刷新
        my_str_splice(NULL);
        record_counter();
    }
    my_str_splice(NULL);
    sprintf(information, "本次运行从%d年 %d月 %d日 %d点 %d分 %d秒 结束\n", get_now_year(),
            get_now_month(), get_now_day(),
            get_now_hour(), get_now_minute(), get_now_second());
}

//记录人
void record_human() {
    int i;
    strcat(information, "--------------------------------客户--------------------------------\n");
    strcat(information,
           "姓名\t\t进入时间\t\t离开时间\t\t是否开始办理业务\t\t办理业务柜台\t\t开始办理业务时间\t\t处理时间\t\t结束办理业务时间\n");
    my_str_splice(information);
    for (i = 0; i < now_human_number; ++i) {
        human *now_human = record_humans[i];
        //姓名
        my_str_splice(now_human->name);
        //进入时间
        my_str_splice(now_human->produce_time);
        //离开时间
        if (strcmp("", now_human->leave_time) == 0) {
            //如果还没有开始
            sprintf(information, "\t\t");
            my_str_splice(information);
        }
        my_str_splice(now_human->leave_time);
        //是否开始办理业务
        if (now_human->is_start) {
            //开始办理了
            strcat(information, "\t是\t\t");
            my_str_splice(information);
            //办理业务的柜台
            sprintf(information, "\t%s\t\t", now_human->target_counter->name);
            my_str_splice(information);
            //开始办理业务时间
            sprintf(information, "%02d:%02d:%02d\t", now_human->start_hour, now_human->start_minute,
                    now_human->start_second);
            my_str_splice(information);
            //处理时间
            sprintf(information, "\t\t%d分钟\t", now_human->processing_time);
            my_str_splice(information);
            if (now_human->is_success) {
                //已经成功办理
                sprintf(information, "\t\t%02d:%02d:%02d\t",now_human->end_hour, now_human->end_minute,
                        now_human->end_second);
                my_str_splice(information);
            }
        } else {
            //没有开始办理 直接撤
            strcat(information, "\t否");
        }
        strcat(information, "\n");
        my_str_splice(information);

    }
    my_str_splice(information);
}

//记录柜台
void record_counter() {
    strcat(information, "--------------------------------柜台--------------------------------\n");
    strcat(information,
           "名称\t\t共计办理业务人数\t\t共计办理时间\t\t平均办理业务时间\n");
    my_str_splice(information);
    int i;
    for (i = 0; i < COUNTER_COUNT; ++i) {
        counter *now_counter = counters[i];
        my_str_splice(now_counter->name);
        if (now_human_number == 0 || now_counter->count_number == 0) {
            //当前没有人
            strcat(information,"\n");
        }else {
            sprintf(information, "\t%d人\t\t\t\t%d分钟\t\t\t%d分钟\n", now_counter->count_number, now_counter->count_all_time,
                    (now_counter->count_all_time / now_counter->count_number));
        }
        my_str_splice(information);
    }
}

void *add_button_listen(void *pVoid) {
    ExMessage msg{};
    while (true) {
        if (peekmessage(&msg, EM_MOUSE)) {
            switch (msg.message) {
                case WM_LBUTTONDOWN:
                    if (msg.x >= 0 && msg.x <= 0 + button_width && msg.y >= button_y &&
                        msg.y <= button_y + button_height) {
                        //暂停按钮
                        is_stop = true;
                    }
                    if (msg.x >= 0 && msg.x <= 0 + button_width && msg.y >= (button_y - stop_start_button_distance) &&
                        msg.y <= (button_y - stop_start_button_distance) + button_height) {
                        //暂停按钮
                        is_stop = false;
                        //开始
                        is_start = true;
                    }
                    if (msg.x >= (GRAPH_WIDTH - button_width) && msg.x <= GRAPH_WIDTH && msg.y >= button_y &&
                        msg.y <= button_y + button_height) {
                        //退出按钮
                        record_all();
                        fputs(information, file);
                        fclose(file);
                        exit(0);
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void button(int x, int y, int w, int h, TCHAR *text) {
    setbkmode(TRANSPARENT);
    setfillcolor(GREEN);
    fillroundrect(x, y, x + w, y + h, 10, 10);
    // 输出字符串（MBCS 字符集）
    TCHAR s1[] = "黑体";
    settextstyle(30, 0, s1);
    int tx = x + (w - textwidth(text)) / 2;
    int ty = y + (h - textheight(text)) / 2;
    outtextxy(tx, ty, text);
}

//拼接字符串
void my_str_splice(char msg[]) {
    if (msg != NULL) {
        if (information != msg) {
            strcat(msg, "\t");
            strcat(information, msg);
        }
    }else{
        strcat(information,"\n");
    }
    //
    fputs(information, file);
    sprintf(information, "");
}
