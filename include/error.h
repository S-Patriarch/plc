/* Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <error.h>
 */

/* Функции обработки ошибок, которые выводят сообщения в
   стандартное устройство вывода сообщений об ошибках.  */

#ifndef __PLC_ERROR_H
#define __PLC_ERROR_H   1

#include <errno.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

/* Обрабатывает нефатальные ошибки, связанные с системными вызовами.
   Выводит сообщение и возвращает управление.  */
extern void p_error_ret (const char *fmt, ...) P_NOEXCEPT; 

/* Обрабатывает фатальные ошибки, связанные с системными вызовами.
   Выводит сообщение и завершает работу процесса.  */
extern void p_error_sys (const char *fmt, ...) P_NOEXCEPT;

/* Обрабатывает нефатальные ошибки, не связанные с системными вызовами.
   Код ошибки передается в аргументе.
   Выводит сообщение и возвращает управление.  */
extern void p_error_cont (int error, const char *fmt, ...) P_NOEXCEPT; 

/* Обрабатывает фатальные ошибки, не связанные с системными вызовами.
   Код ошибки передается в аргументе.
   Выводит сообщение и завершает работу процесса.  */
extern void p_error_exit (int error, const char *fmt, ...) P_NOEXCEPT; 

/* Обрабатывает фатальные ошибки, связанные с системными вызовами.
   Выводит сообщение, создает файл core и завершает работу процесса.  */
extern void p_error_dump (const char *fmt, ...) P_NOEXCEPT; 

/* Обрабатывает нефатальные ошибки, не связанные с системными вызовами.
   Выводит сообщение и возвращает управление.  */
extern void p_error_msg (const char *fmt, ...) P_NOEXCEPT; 

/* Обрабатывает фатальные ошибки, не связанные с системными вызовами.
   Выводит сообщение и завершает работу процесса.  */
extern void p_error_quit (const char *fmt, ...) P_NOEXCEPT;

/* Возвращает строку с сообщением, которая соответствует номеру
   ошибки, переданному ей в качестве аргумента. В противном случае 
   возвращает NULL.
   ВНИМАНИЕ - вызывающий освобождает free(). 
   Пример использования:
        char *err_msg = p_error_get(errno);
        if (err_msg != NULL) {
                fprintf(stderr, "E: %s\n", err_msg);
                free(err_msg); 
        }  */
extern char *p_error_get (int errnum) P_NOEXCEPT;

P_END_DECLS

#endif /* error.h  */
