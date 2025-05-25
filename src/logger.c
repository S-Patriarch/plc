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
#include <sys/types.h>

logger_s *logger_create(const char *file_name)
{
        /* выделяем память под структуру логгера */
        logger_s *log = (logger_s *)malloc(sizeof(logger_s));
        if (!log) return NULL;

        /* инициализируем блокировки OpenMP */
        omp_init_lock(&log->lck_write);
        omp_init_nest_lock(&log->lck_read);

        /* открываем файл в режиме "добавление + чтение" (a+) */
        log->file = fopen(file_name, "a+");
        if (!log->file) return NULL;

        return log;
}

void logger_destroy(logger_s * log)
{
        if (log) {
                /* закрываем файл, если он открыт */
                if (log->file) fclose(log->file);

                /* уничтожаем блокировки */
                omp_destroy_lock(&log->lck_write);
                omp_destroy_nest_lock(&log->lck_read);

                /* освобождаем память */
                free(log);
        }
}

void logger_write(logger_s *log, const char *msg)
{
        if (!log || !msg) return;

        /* блокируем запись (эксклюзивный доступ) */
        omp_set_lock(&log->lck_write);

        /* блокируем чтение на время записи */
        omp_set_nest_lock(&log->lck_read);

        char s_date[11], s_time[9];
        getdate(s_date, sizeof(s_date));
        gettime(s_time, sizeof(s_time));

        /* если файл открыт, записываем сообщение */
        if (log->file) {
                fprintf(log->file, "[%s %s] %s\n", s_date, s_time, msg);
                fflush(log->file);
        }

        /* разблокируем в обратном порядке */
        omp_unset_nest_lock(&log->lck_read);
        omp_unset_lock(&log->lck_write);
}

char *logger_read(logger_s *log)
{
        if (!log) return NULL;

        char *line = NULL;
        size_t len = 0;
        ssize_t read;

        /* блокируем для чтения (разделяемый доступ) */
        omp_set_nest_lock(&log->lck_read);

        if (log->file) {
                /* читаем строку (getline сама выделяет память) */
                read = getline(&line, &len, log->file);

                if (read == -1) {
                        free(line);
                        line = NULL;
                }
        }

        /* разблокируем */
        omp_unset_nest_lock(&log->lck_read);

        /*
         * ВНИМАНИЕ: вызывающий код должен освободить эту память!
         */
        return line;
}
