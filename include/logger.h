/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : logger.h
 */

#ifndef __PLC_LOGGER_H
#define __PLC_LOGGER_H  1

#include <stdio.h>
#include <omp.h>

typedef struct logger {
        FILE            *file;           /* указатель на файл логов */
        omp_lock_t       lck_write;      /* блокировка для записи (эксклюзивный доступ) */
        omp_nest_lock_t  lck_read;       /* блокировка для чтения (разделяемый доступ) */
} logger_s;

logger_s *p_logger_create(const char *file_name);     /* создание нового логгера */
void      p_logger_destroy(logger_s *log);            /* уничтожение логгера и освобождение ресурсов */

void  p_logger_write(logger_s *log, const char *msg); /* запись сообщений в лог (потокобезопасная) */
char *p_logger_read(logger_s *log);                   /* чтение строки из лога (потокобезопасное) */

#endif  /* __PLC_LOGGER_H */
