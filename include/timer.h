/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : timer.h
 */

#ifndef __PLC_TIMER_H
#define __PLC_TIMER_H   1

void start_timer(void); /* начало замера */
void stop_timer(void);  /* завершение замера */

double duration_seconds(void); /* результат замера в секундах */
double duration_minutes(void); /* результат замера в минутах */
double duration_hours(void);   /* результат замера в часах */

#endif  /* __PLC_TIMER_H */
