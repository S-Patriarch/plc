/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : logger.h
 */

#ifndef __PLC_LOGGER_H
#define __PLC_LOGGER_H  1

#include <stdio.h>
#include <pthread.h>
#include <plc/plcdef.h>

typedef struct logger {
        FILE             *file;                                      /* указатель на файл логов  */
        pthread_mutex_t   lck_write;                                 /* блокировка для записи (эксклюзивный доступ)  */
        pthread_rwlock_t  lck_read;                                  /* блокировка для чтения (разделяемый доступ)  */
} logger_s;

logger_s *p_logger_create(const char *file_name) P_NOEXCEPT;         /* создание нового логгера  */
void      p_logger_destroy(logger_s *log) P_NOEXCEPT;                /* уничтожение логгера и освобождение ресурсов  */
void      p_logger_write(logger_s *log, const char *msg) P_NOEXCEPT; /* запись сообщений в лог (потокобезопасная)  */
char     *p_logger_read(logger_s *log) P_NOEXCEPT;                   /* чтение строки из лога (потокобезопасное)  */

#endif  /* __PLC_LOGGER_H  */
