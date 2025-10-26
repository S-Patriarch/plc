/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : logger.c
 */

#include <plc/logger.h>
#include <plc/dt.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/types.h>

logger_s *p_logger_create(const char *file_name)
{
        /* выделяем память под структуру логгера */
        logger_s *log = (logger_s *)malloc(sizeof(logger_s));
        if (!log) return NULL;

        /* открываем файл в режиме "добавление + чтение" (a+) */
        log->file = fopen(file_name, "a+");
        if (!log->file) {
                free(log);
                return NULL;
        }

        /* инициализируем мьютекс для записи */
        if (pthread_mutex_init(&log->lck_write, NULL) != 0) {
                fclose(log->file);
                free(log);
                return NULL;
        }

        /* инициализируем RW-lock для чтения */
        if (pthread_rwlock_init(&log->lck_read, NULL) != 0) {
                pthread_mutex_destroy(&log->lck_write);
                fclose(log->file);
                free(log);
                return NULL;
        }

        return log;
}

void p_logger_destroy(logger_s *log)
{
        if (!log) return;

        if (log->file)
                fclose(log->file);

        pthread_mutex_destroy(&log->lck_write);
        pthread_rwlock_destroy(&log->lck_read);

        free(log);
}

void p_logger_write(logger_s *log, const char *msg)
{
        if (!log || !msg) return;

        char s_date[11], s_time[9];
        p_getdate(s_date, sizeof(s_date));
        p_gettime(s_time, sizeof(s_time));

        pthread_mutex_lock(&log->lck_write);

        fprintf(log->file, "[%s %s] %s\n", s_date, s_time, msg);
        fflush(log->file);

        pthread_mutex_unlock(&log->lck_write);
}

char *p_logger_read(logger_s *log)
{
        if (!log) return NULL;

        char    *line = NULL;
        size_t   len = 0;
        ssize_t  read;

        pthread_rwlock_rdlock(&log->lck_read);

        read = getline(&line, &len, log->file);

        pthread_rwlock_unlock(&log->lck_read);

        if (read == -1) {
                free(line);
                return NULL;
        }
        
        /*
         * ВНИМАНИЕ: вызывающий код должен освободить эту память!
         */
        return line;
}

