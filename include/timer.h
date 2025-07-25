/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : timer.h
 */

#ifndef __PLC_TIMER_H
#define __PLC_TIMER_H   1

void p_start_timer(void); /* начало замера */
void p_stop_timer(void);  /* завершение замера */

double p_duration_seconds(void); /* результат замера в секундах */
double p_duration_minutes(void); /* результат замера в минутах */
double p_duration_hours(void);   /* результат замера в часах */

#endif  /* __PLC_TIMER_H */
