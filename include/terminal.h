/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <terminal.h>
 */

#ifndef __PLC_TERMINAL_H
#define __PLC_TERMINAL_H        1

#define _XOPEN_SOURCE 600

#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#if defined(SOLARIS)
#include <stropts.h>
#endif
#include <plc/plcdef.h>

P_BEGIN_DECLS

/* Открывает следующее доступное ведущее устройство псевдотерминала.

   Вызывающий процесс должен разместить в памяти буфер для имени ведущего
   или ведомого устройства; в случае успеха в этом буфере (аргумент pts_name)
   будет возвращено имя ведомого PTY.  Это имя затем передается функции
   p_ptys_open, которая открывает ведомое устройство.  Размер буфера в байтах
   передается в аргументе pts_namesz, чтобы функция p_ptym_open не создавала
   копию строки длиннее, чем размер буфера.

   Возвращает дескриптор ведущего устройства PTY в случае успеха,
   -1 - в случае ошибки.  */
extern int p_ptym_open (char *pts_name, int pts_namesz) P_NOEXCEPT;

/* Открывает следующее доступное ведомое устройство псевдотерминала.
   Возвращает дескриптор ведомого устройства PTY в случае успеха,
   -1 - в случае ошибки.  */
extern int p_ptys_open (char *pts_name) P_NOEXCEPT;

/* Перевод терминала в режим посимвольного ввода (cbreak).
   Возвращает 0 в случае успеха, -1 - в случае ошибки.  */
extern int p_tty_cbreak (int fd) P_NOEXCEPT;

/* Переводит терминал в режим прозрачного ввода (raw).
   Возвращает 0 в случае успеха, -1 - в случае ошибки.  */
extern int p_tty_raw (int fd) P_NOEXCEPT;

/* Востанавливает состояние терминала.
   Возвращает 0 в случае успеха, -1 - в случае ошибки.  */
extern int p_tty_reset (int fd) P_NOEXCEPT;

/* Может устанавливаться вызовом atexit(p_tty_atexit).  */
extern void p_tty_atexit (void) P_NOEXCEPT;

/* Позволяет вызывающему процессу узнать начальное состояние терминала.  */
extern struct termios *p_tty_termios (void) P_NOEXCEPT;

P_END_DECLS

#endif /* terminal.h  */
