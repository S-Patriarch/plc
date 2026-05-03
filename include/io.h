/* Copyright (C) 2025-2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <io.h>
 */

#ifndef __PLC_IO_H
#define __PLC_IO_H      1

#include <stddef.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

enum mess_format {
        mess_n,         /* нет формата - ""  */
        mess_w,         /* предупреждение - "W: "  */
        mess_e          /* ошибка - "E: "  */
};

/* Вывод сообщения в stdout.  */
extern int p_mout (const char *message, enum mess_format format, 
                   const char *attr_m, const char *attr_f) P_NOEXCEPT;

/* Вывод строки в stdout.  */
extern int p_cout (const char *s) P_NOEXCEPT;

/* Функция записывает строку в стандартный поток вывода stdout,
   автоматически добавляя в конце символ новой строки '\n'.
   Возвращает 0 при успехе, -1 при ошибке.
   Особенности работы:
   - Выводит все символы строки до '\0', но сам нуль-терминатор не выводит.
   - После строки всегда добавляет символ перевода строки '\n'.
   - Использует stdout как поток вывода.
   - При успехе сбрасывает буфер stdout.  */
extern int p_puts (const char *s) P_NOEXCEPT;

/* Функция принимает ввод.
   Функция циклически считывает по одному символу. Цикл прерывается при 
   чтении либо EOF, либо символа перевода строки '\n', либо достижения 
   границы массива len. Когда ввод достигает максимального количества
   символов, функция добавляет в dst символ '\0'.  */
extern void p_gets (char *dst, size_t len) P_NOEXCEPT;

P_END_DECLS

#endif /* io.h  */
