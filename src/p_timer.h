/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : p_timer.h
 */

#ifndef __PLC_P_TIMER_H
#define __PLC_P_TIMER_H         2

#include <time.h>

struct timer {
        time_t  begin_time;
        time_t  end_time;
};

#endif  /* __PLC_P_TIMER_H */
