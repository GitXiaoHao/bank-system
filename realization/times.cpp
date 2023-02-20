//
// Created by yuhao on 2023/2/18.
//
#include "../include/times.h"

static time_t tmp_cal_ptr;
static struct tm *tmp_ptr;

//先获取时间
void init() {
    time(&tmp_cal_ptr);
    tmp_ptr = gmtime(&tmp_cal_ptr);
}

int get_now_month() {
    init();
    //当前月份
    return tmp_ptr->tm_mon + 1;
}

int get_now_day() {
    init();
    //当前是这个月的第几天
    return tmp_ptr->tm_mday;
}

int get_now_year() {
    init();
    //当前年
    return tmp_ptr->tm_year + 1900;
}

int get_now_hour() {
    init();
    //当前时
    return tmp_ptr->tm_hour + 8;
}

int get_now_minute() {
    init();
    //当前分
    int min = tmp_ptr->tm_min;

    return min;
}

int get_now_second() {
    init();
    //当前秒
    return tmp_ptr->tm_sec;
}