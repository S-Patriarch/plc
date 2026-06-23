/* Copyright (C) 2025-2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <io.h>
 */

#ifndef __PLC_IO_H
#define __PLC_IO_H      1

#include <stdio.h>
#include <string.h>
#include <unistd.h>
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

/* Функции p_readn и p_writen читают или пишут определенное количество байтов в
   дескриптор.  Они сами позаботятся об обслуживании ситуаций, когда операции 
   чтония или записи выполняются лишь частично: они будут вызывать функции read 
   или write столько раз, сколько потребуется для чтения или записи заданного
   количества байтов.  Функцию p_writen всегда можно использовать для 
   большенства типов файлов, но функция p_readn должна вызываться, только
   когда заранее известно,что из данного файла можно прочитать заданное
   количество байтов.
   Обе возвращают количество прочитанных или записанных байтов, -1 - в случае
   ошибки.  */

/* Читает nbytes байт из дескриптора.  */
extern ssize_t p_readn (int fd, void *buf, size_t nbytes) P_NOEXCEPT;

/* Записывает nbytes байт в дескриптор.  */
extern ssize_t p_writen (int fd, const void *buf, size_t nbytes) P_NOEXCEPT;

P_END_DECLS

#endif /* io.h  */
