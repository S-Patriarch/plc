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
int mout(const char *, enum mess_format, const char *, const char *);

#endif  /* __PLC_IO_H */
