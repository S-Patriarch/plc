/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : error.c
 */

#include <plc/error.h>
#include <plc/plcdef.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void p_err_doit(int, int, const char *, va_list); 

void
p_err_ret(const char *fmt, ...) 
{
        va_list         ap;

        va_start(ap, fmt);
        p_err_doit(1, errno, fmt, ap);
        va_end(ap);
} 

void
p_err_sys(const char *fmt, ...) 
{
        va_list         ap;

        va_start(ap, fmt);
        p_err_doit(1, errno, fmt, ap);
        exit(1);
}

void
p_err_cont(int error, const char *fmt, ...) 
{
        va_list         ap;

        va_start(ap, fmt);
        p_err_doit(1, error, fmt, ap);
        va_end(ap);
} 

void
p_err_exit(int error, const char *fmt, ...) 
{
        va_list         ap;

        va_start(ap, fmt);
        p_err_doit(1, error, fmt, ap);
        va_end(ap);
        exit(1);
} 

void
p_err_dump(const char *fmt, ...) 
{
        va_list         ap;

        va_start(ap, fmt);
        p_err_doit(1, errno, fmt, ap);
        va_end(ap);
        abort();
        exit(1);
} 

void
p_err_msg(const char *fmt, ...) 
{
        va_list         ap;

        va_start(ap, fmt);
        p_err_doit(0, 0, fmt, ap);
        va_end(ap);
} 

void
p_err_quit(const char *fmt, ...) 
{
        va_list         ap;

        va_start(ap, fmt);
        p_err_doit(0, 0, fmt, ap);
        va_end(ap);
        exit(1);
}

/*
 * Выводит сообщение и возвращает управление в вызывающую функцию.
 * Вызывающая функция определяет значение флага "errno_flag".
 */
static void
p_err_doit(int errno_flag, int error, const char *fmt, va_list ap) 
{
        char    buf[P_BUFSIZ_4K];

        vsnprintf(buf, P_BUFSIZ_4K-1, fmt, ap);
        if (errno_flag)
                snprintf(buf+strlen(buf), P_BUFSIZ_4K-strlen(buf)-1, ": %s",
                         strerror(error));
        strcat(buf, "\n");
        fflush(stdout);
        fputs(buf, stderr);
        fflush(NULL);
}

