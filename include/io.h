/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : io.h
 */

#ifndef __PLC_IO_H
#define __PLC_IO_H      1

enum mess_format {
        mess_n,         /* нет формата - "" */
        mess_w,         /* предупреждение - "W: " */
        mess_e          /* ошибка - "E: " */
};

/* вывод сообщения в stdout*/
int p_mout(const char       *message, 
           enum mess_format  format, 
           const char       *attr_m, 
           const char       *attr_f);

/* вывод строки в stdout */
int p_cout(const char *s);

#endif  /* __PLC_IO_H */
