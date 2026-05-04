/* Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <error.h>
 */

/* Функции обработки ошибок, которые выводят сообщения в
   стандартное устройство вывода сообщений об ошибках.  */

#ifndef __PLC_ERROR_H
#define __PLC_ERROR_H   1

#include <plc/plcdef.h>

P_BEGIN_DECLS

/* Обрабатывает нефатальные ошибки, связанные с системными вызовами.
   Выводит сообщение и возвращает управление.  */
extern void p_err_ret (const char *fmt, ...) P_NOEXCEPT; 

/* Обрабатывает фатальные ошибки, связанные с системными вызовами.
   Выводит сообщение и завершает работу процесса.  */
extern void p_err_sys (const char *fmt, ...) P_NOEXCEPT;

/* Обрабатывает нефатальные ошибки, не связанные с системными вызовами.
   Код ошибки передается в аргументе.
   Выводит сообщение и возвращает управление.  */
extern void p_err_cont (int error, const char *fmt, ...) P_NOEXCEPT; 

/* Обрабатывает фатальные ошибки, не связанные с системными вызовами.
   Код ошибки передается в аргументе.
   Выводит сообщение и завершает работу процесса.  */
extern void p_err_exit (int error, const char *fmt, ...) P_NOEXCEPT; 

/* Обрабатывает фатальные ошибки, связанные с системными вызовами.
   Выводит сообщение, создает файл core и завершает работу процесса.  */
extern void p_err_dump (const char *fmt, ...) P_NOEXCEPT; 

/* Обрабатывает нефатальные ошибки, не связанные с системными вызовами.
   Выводит сообщение и возвращает управление.  */
extern void p_err_msg (const char *fmt, ...) P_NOEXCEPT; 

/* Обрабатывает фатальные ошибки, не связанные с системными вызовами.
   Выводит сообщение и завершает работу процесса.  */
extern void p_err_quit (const char *fmt, ...) P_NOEXCEPT;

P_END_DECLS

#endif /* error.h  */
