/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : dt.c
 */

#define _POSIX_C_SOURCE 200809L         /* для nanosleep() */

#include <plc/dt.h>
#include <plc/plcdef.h>
#include <time.h>
#include <stdio.h>

int getdate(char *s, size_t size) 
{
        time_t now;

        time(&now);
        struct tm *lt = localtime(&now);

        if (lt == NULL)
                return ERROR;

        strftime(s, size, "%d-%m-%Y", lt);
        return SUCCESS;
}

int gettime(char *s, size_t size) 
{
        time_t now;

        time(&now);
        struct tm *lt = localtime(&now);

        if (lt == NULL)
                return ERROR;

        strftime(s, size, "%H:%M:%S", lt);
        return SUCCESS;
}

int date_convert(const char *s_in, size_t size_s_in, 
                 char *s_out, size_t size_s_out,
                 enum dcf dcf) 
{
        if (s_in == NULL || *s_in == '\0')
                return ERROR;

        struct tm tm = {0};

        if (sscanf(s_in, "%d-%d-%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year) == 3) {
                tm.tm_mon -= 1;     /* коррекция месяца (январь = 0) */
                tm.tm_year -= 1900; /* коррекция года (отсчет с 1900) */
        } else
                return ERROR;

        switch (dcf) {
        case DCF_dBY :
                strftime(s_out, size_s_out, "%d %B %Y", &tm);
                break;
        case DCF_BdY :
                strftime(s_out, size_s_out, "%B %d, %Y", &tm);
                break;
        default :
                return ERROR;
        }

        return SUCCESS;
}

/*
 * Особенности nanosleep:
 * - 1 секунда = 1000 миллисекунд
 * - 1 миллисекунда = 1 000 000 наносекунд
 * - функция может быть прервана сигналом, поэтому нужен цикл
 */
void delay_ms(unsigned int ms) 
{
        struct timespec ts;
        ts.tv_sec = ms / 1000;              /* целые секунды */
        ts.tv_nsec = (ms % 1000) * 1000000; /* остаток в наносекундах */

        while (nanosleep(&ts, &ts) == -1)
                /* продолжаем спать, если осталось время после прерывания */
                continue;
}

