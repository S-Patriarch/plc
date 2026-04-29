/*
 * (C) 2025-26, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : io.h
 */

#ifndef __PLC_IO_H
#define __PLC_IO_H      1

#include <plc/plcdef.h>
#include <stddef.h>

enum mess_format {
        mess_n,         /* нет формата - ""  */
        mess_w,         /* предупреждение - "W: "  */
        mess_e          /* ошибка - "E: "  */
};

/* Вывод сообщения в stdout.  */
int p_mout (const char *message, enum mess_format format, 
            const char *attr_m, const char *attr_f) P_NOEXCEPT;

/* Вывод строки в stdout.  */
int p_cout (const char *s) P_NOEXCEPT;

/* Функция принимает ввод.
   Функция циклически считывает по одному символу. Цикл прерывается при 
   чтении либо EOF, либо символа перевода строки '\n', либо достижения 
   границы массива len. Когда ввод достигает максимального количества
   символов, функция добавляет в dst символ '\0'.  */
void p_gets (char *dst, size_t len) P_NOEXCEPT;

#endif  /* __PLC_IO_H  */
