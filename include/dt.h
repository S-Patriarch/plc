/* Copyright (C) 2025-2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <dt.h>
 */

#ifndef __PLC_DT_H
#define __PLC_DT_H      1

#include <stddef.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

enum dcf {
        DCF_dBY,        /* формат конвертации "%d %B %Y"  */
        DCF_BdY         /* формат конвертации "%B %d, %Y"  */
};

/* Получить текущую дату.  */
extern int p_getdate (char *s, size_t size) P_NOEXCEPT;

/* Получить текущее время.  */
extern int p_gettime (char *s, size_t size) P_NOEXCEPT;

/* Осуществляет задержку в миллисекундах.  */
extern void p_delay_ms (unsigned int ms) P_NOEXCEPT;

/* Преобразовать строку даты в иной вид строки.  */
extern int p_date_convert (const char *s_in, size_t size_s_in, 
                           char *s_out, size_t size_s_out, 
                           enum dcf dcf) P_NOEXCEPT;

P_END_DECLS

#endif /* dt.h  */
