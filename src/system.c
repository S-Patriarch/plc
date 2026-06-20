/* Copyright (C) 2025-2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            system.c
 */

#define _POSIX_C_SOURCE 200809L

#include <plc/system.h>

int 
p_locale_available(const char *locale) P_NOEXCEPT
{
        char *old_locale = setlocale(LC_ALL, NULL);
        char *res = setlocale(LC_ALL, locale);

        if (res == NULL) 
                return(P_FALSE);

        setlocale(LC_ALL, old_locale);
        return (P_TRUE);
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
                return (NULL);

        /* находим позицию первой точки в env_val  */
        dot_pos = strchr(env_val, '.');

        /* если точки нет, возвращаем копию всей строки  */
        if (dot_pos == NULL) {
                env_res = strdup(env_val);
                return (env_res);
        }

        /* вычисляем длину подстроки до точки  */
        len = dot_pos - env_val;

        /* выделяем память для подстроки (+1 для нулевого терминатора)  */
        env_res = (char *)malloc(len + 1);
        if (env_res == NULL)
                return (NULL);

        /* копируем подстроку  */
        memcpy(env_res, env_val, len);
        env_res[len] = '\0';

        return (env_res);
}

void
p_free(void **ptr) P_NOEXCEPT
{
        if (ptr != NULL && *ptr != NULL) {
                free(*ptr);
                *ptr = NULL;
        }
}

void
p_fclose(FILE **file) P_NOEXCEPT
{
        if (file != NULL && *file != NULL) {
                fclose(*file);
                *file = NULL;
        }
}

int
p_make_timeout(struct timespec *tsp, long minutes) P_NOEXCEPT 
{
        struct timeval  now;

        if (gettimeofday(&now, NULL) != 0) /* получить текущее время  */
                return (P_ERROR);

        tsp->tv_sec = now.tv_sec;
        tsp->tv_nsec = now.tv_usec * 1000; /* микросекунды в наносекунды  */

        /* добавить величину тайм-аута  */
        tsp->tv_sec += minutes * 60;

        return 0;
}

p_sigfunc * 
p_signal(int signo, p_sigfunc *func) P_NOEXCEPT 
{
        struct sigaction act, oact;

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
                return (SIG_ERR);

        return (oact.sa_handler);
}

int
p_daemonize(const char *cmd) P_NOEXCEPT 
{
        int                     i, fd0, fd1, fd2;
        pid_t                   pid;
        struct rlimit           rl;
        struct sigaction        sa;

        /* сбросить маску режима создания файла  */
        umask(0);

        /* получить максимально возможный номер дескриптора файла  */
        if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
                return(-1);

        /* стать лидером нового сеанса, чтобы утратить управляющий терминал  */
        if ((pid = fork()) < 0)
                return(-1);
        else if (pid != 0) /* родительский процесс  */
                exit(0);
        setsid();

        /* обеспечить невозможность обретения управляющего терминала в будущем  */
        sa.sa_handler = SIG_IGN;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        if (sigaction(SIGHUP,&sa, NULL) < 0)
                return(-1);
        if ((pid = fork()) < 0)
                return(-1);
        else if (pid != 0 ) /* родительский процесс  */
                exit(0);

        /* назначить корневой каталог текущим рабочим каталогом,
           чтобы впоследствии можно было отмонтировать файловую систему  */
        if (chdir("/") < 0)
                return(-1);

        /* закрыть все открытые файловые дескрипторы  */
        if (rl.rlim_max == RLIM_INFINITY)
                rl.rlim_max = 1024;
        for (i = 0; i < rl.rlim_max; i++)
                close(i);

        /* присоединить файловые дескрипторы 0, 1 и 2 к /dev/null  */
        fd0 = open("/dev/null", O_RDWR);
        fd1 = dup(0);
        fd2 = dup(0);

        /* инициализировать файл журнала  */
        openlog(cmd, LOG_CONS, LOG_DAEMON);
        if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
                syslog(LOG_ERR, "ошибочные файловые дескрипторы %d %d %d",
                       fd0, fd1, fd2);
                return(-1);
        }

        return(0);
}

