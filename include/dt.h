/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : dt.h
 */

#ifndef __PLC_DT_H
#define __PLC_DT_H      1

#include <stddef.h>

enum dcf {
        DCF_dBY, /* формат конвертации "%d %B %Y" */
        DCF_BdY  /* формат конвертации "%B %d, %Y" */
};

int getdate(char *, size_t); /* получить текущую дату */
int gettime(char *, size_t); /* получить текущее время */

/* преобразует строку даты в иной вид строки */
int date_convert(const char *, size_t, char *, size_t, enum dcf);

/* осуществляет задержку в миллисекундах */
void delay_ms(unsigned int);

#endif  /* __PLC_DT_H */
