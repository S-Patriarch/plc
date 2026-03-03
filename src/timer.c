/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : timer.c
 */

#include "p_timer.h"
#include <plc/timer.h>

struct timer timer;

void p_start_timer(void) P_NOEXCEPT
{
        time(&timer.begin_time);
}

void p_stop_timer(void) P_NOEXCEPT
{
        time(&timer.end_time);
}

double p_duration_seconds(void) P_NOEXCEPT
{
        return (difftime(timer.end_time, timer.begin_time));
}

double p_duration_minutes(void) P_NOEXCEPT
{
        double diff_sec = difftime(timer.end_time, timer.begin_time);
        return (diff_sec / 60);
}

double p_duration_hours(void) P_NOEXCEPT
{
        double diff_sec = difftime(timer.end_time, timer.begin_time);
        double diff_min = diff_sec / 60;
        return (diff_min / 60);
}

