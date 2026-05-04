/* Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <logger.h>
 */

#ifndef __PLC_LOGGER_H
#define __PLC_LOGGER_H  1

#include <stdio.h>
#include <pthread.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

typedef struct logger {
        FILE *file;                /* указатель на файл логов  */
        pthread_mutex_t lck_write; /* бло-ка для записи (экскл. доступ)  */
        pthread_rwlock_t lck_read; /* бло-ка для чтения (раздел. доступ)  */
} logger_s;

/* Создание нового логгера.  */
extern logger_s *p_logger_create (const char *file_name) P_NOEXCEPT;

/* Уничтожение логгера и освобождение ресурсов.  */
extern void p_logger_destroy (logger_s *log) P_NOEXCEPT;

/* Запись сообщений в лог (потокобезопасная).  */
extern void p_logger_write (logger_s *log, const char *msg) P_NOEXCEPT;

/* Чтение строки из лога (потокобезопасное).  */
extern char *p_logger_read (logger_s *log) P_NOEXCEPT;

P_END_DECLS

#endif /* logger.h  */
