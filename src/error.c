/* Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            error.c
 */

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <plc/error.h>
#include <plc/plcdef.h>

static void p_error_doit(int, int, const char *, va_list) P_NOEXCEPT; 

void
p_error_ret(const char *fmt, ...) P_NOEXCEPT
{
        va_list ap;

        va_start(ap, fmt);
        p_error_doit(1, errno, fmt, ap);
        va_end(ap);
} 

void
p_error_sys(const char *fmt, ...) P_NOEXCEPT
{
        va_list ap;

        va_start(ap, fmt);
        p_error_doit(1, errno, fmt, ap);
        exit(1);
}

void
p_error_cont(int error, const char *fmt, ...) P_NOEXCEPT 
{
        va_list ap;

        va_start(ap, fmt);
        p_error_doit(1, error, fmt, ap);
        va_end(ap);
} 

void
p_error_exit(int error, const char *fmt, ...) P_NOEXCEPT
{
        va_list ap;

        va_start(ap, fmt);
        p_error_doit(1, error, fmt, ap);
        va_end(ap);
        exit(1);
} 

void
p_error_dump(const char *fmt, ...) P_NOEXCEPT
{
        va_list ap;

        va_start(ap, fmt);
        p_error_doit(1, errno, fmt, ap);
        va_end(ap);
        abort();
        exit(1);
} 

void
p_error_msg(const char *fmt, ...) P_NOEXCEPT
{
        va_list ap;

        va_start(ap, fmt);
        p_error_doit(0, 0, fmt, ap);
        va_end(ap);
} 

void
p_error_quit(const char *fmt, ...) P_NOEXCEPT
{
        va_list ap;

        va_start(ap, fmt);
        p_error_doit(0, 0, fmt, ap);
        va_end(ap);
        exit(1);
}

/* Выводит сообщение и возвращает управление в вызывающую функцию.
   Вызывающая функция определяет значение флага "errno_flag".  */
static void
p_error_doit(int errno_flag, int error, const char *fmt, va_list ap) P_NOEXCEPT
{
        char buf[P_BUFSIZ_4K];

        vsnprintf(buf, P_BUFSIZ_4K-1, fmt, ap);
        if (errno_flag)
                snprintf(buf+strlen(buf), P_BUFSIZ_4K-strlen(buf)-1, ": %s",
                         strerror(error));
        strcat(buf, "\n");
        fflush(stdout);
        fputs(buf, stderr);
        fflush(NULL);
}

