/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : dt.h
 */

#ifndef __PLC_DT_H
#define __PLC_DT_H      1

#include <stddef.h>
#include <plc/plcdef.h>

enum dcf {
        DCF_dBY, /* формат конвертации "%d %B %Y"  */
        DCF_BdY  /* формат конвертации "%B %d, %Y"  */
};

int     p_getdate(char *s, size_t size) P_NOEXCEPT; /* получить текущую дату  */
int     p_gettime(char *s, size_t size) P_NOEXCEPT; /* получить текущее время  */

/* преобразует строку даты в иной вид строки  */
int     p_date_convert(const char *s_in,  size_t size_s_in, 
                       char       *s_out, size_t size_s_out, 
                       enum dcf    dcf) P_NOEXCEPT;

/* осуществляет задержку в миллисекундах  */
void    p_delay_ms(unsigned int ms) P_NOEXCEPT;

#endif  /* __PLC_DT_H  */
