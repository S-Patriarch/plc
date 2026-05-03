/* Copyright (C) 2025-2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <p_timer.h>
 */

#ifndef __PLC_P_TIMER_H
#define __PLC_P_TIMER_H         2

#include <plc/plcdef.h>
#include <time.h>

P_BEGIN_DECLS

struct timer {
        time_t begin_time;
        time_t end_time;
};

P_END_DECLS

#endif /* p_timer.h  */
