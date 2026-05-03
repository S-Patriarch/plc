/* Copyright (C) 2025-2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <timer.h>
 */

#ifndef __PLC_TIMER_H
#define __PLC_TIMER_H   1

#include <plc/plcdef.h>

P_BEGIN_DECLS

extern void p_start_timer (void) P_NOEXCEPT;
extern void p_stop_timer (void) P_NOEXCEPT;

/* Результат замера в секундах.  */
extern double p_duration_seconds (void) P_NOEXCEPT; 

/* Результат замера в минутах.  */
extern double p_duration_minutes (void) P_NOEXCEPT; 

/* Результат замера в часах.  */
extern double p_duration_hours (void) P_NOEXCEPT;

P_END_DECLS

#endif /* timer.h  */
