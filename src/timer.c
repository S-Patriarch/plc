/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : timer.c
 */

#include "p_timer.h"
#include <plc/timer.h>

struct timer timer;

void p_start_timer(void) 
{
        time(&timer.begin_time);
}

void p_stop_timer(void) 
{
        time(&timer.end_time);
}

double p_duration_seconds(void) 
{
        return (difftime(timer.end_time, timer.begin_time));
}

double p_duration_minutes(void) 
{
        double diff_sec = difftime(timer.end_time, timer.begin_time);
        return (diff_sec / 60);
}

double p_duration_hours(void) 
{
        double diff_sec = difftime(timer.end_time, timer.begin_time);
        double diff_min = diff_sec / 60;
        return (diff_min / 60);
}

