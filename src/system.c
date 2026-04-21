/*
 * (C) 2025-26, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : system.c
 */

#define _POSIX_C_SOURCE 200809L

#include <plc/system.h>
#include <plc/plcdef.h>
#include <time.h>
#include <sys/time.h>
#include <locale.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <signal.h>

int 
p_locale_available(const char *locale) P_NOEXCEPT
{
        char *old_locale = setlocale(LC_ALL, NULL);
        char *res = setlocale(LC_ALL, locale);

        if (res == NULL) 
                return(P_FALSE);

        setlocale(LC_ALL, old_locale);
        return(P_TRUE);
}

char *
p_getlocale(void) P_NOEXCEPT
{
        const char     *env_name = "LANG";
        char           *env_val;
        char           *env_res;
        char           *dot_pos;
        size_t          len;

        env_val = getenv(env_name);
        if (env_name == NULL)
                return(NULL);

        /* Находим позицию первой точки в env_val */
        dot_pos = strchr(env_val, '.');

        /* Если точки нет, возвращаем копию всей строки */
        if (dot_pos == NULL) {
                env_res = strdup(env_val);
                return(env_res);
        }

        /* Вычисляем длину подстроки до точки */
        len = dot_pos - env_val;

        /* Выделяем память для подстроки (+1 для нулевого терминатора) */
        env_res = (char *)malloc(len + 1);
        if (env_res == NULL)
                return(NULL);

        /* Копируем подстроку */
        memcpy(env_res, env_val, len);
        env_res[len] = '\0';

        return(env_res);
}

void
p_free_ptr(void **ptr) P_NOEXCEPT
{
        if (ptr != NULL && *ptr != NULL) {
                free(*ptr);
                *ptr = NULL;
        }
}

int
p_make_timeout(struct timespec *tsp, long minutes) P_NOEXCEPT 
{
        struct timeval  now;

        if (gettimeofday(&now, NULL) != 0) /* получить текущее время  */
                return(-1);

        tsp->tv_sec = now.tv_sec;
        tsp->tv_nsec = now.tv_usec * 1000; /* микросекунды в наносекунды  */

        /* добавить величину тайм-аута  */
        tsp->tv_sec += minutes * 60;

        return(0);
}

p_sigfunc * 
p_signal(int signo, p_sigfunc *func) P_NOEXCEPT 
{
        struct sigaction        act, oact;

        act.sa_handler = func;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;

        if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
                act.sa_flags |= SA_INTERRUPT;
#endif        
        } else {
                act.sa_flags |= SA_RESTART;
        }

        if (sigaction(signo, &act, &oact) < 0)
                return(SIG_ERR);

        return(oact.sa_handler);
}

