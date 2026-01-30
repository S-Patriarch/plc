/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : async.h
 */

#ifndef __PLC_ASYNC_H
#define __PLC_ASYNC_H   1

#include <pthread.h>
#include <stdatomic.h>
#include <plc/plcdef.h>

typedef struct {
        pthread_t       thread;             /* идентификатор потока  */
        void           *result;             /* указатель на результат  */
        atomic_bool     completed;          /* флаг завершения задачи  */
        void           *(*function)(void*); /* функция для выполнения  */
        void           *argument;           /* аргумент для функции  */
} future_s;

future_s *p_async_create(void *(*func)(void*), void *arg) P_NOEXCEPT; /* создание задачи  */
int       p_async_launch(future_s *future) P_NOEXCEPT;                /* запуск задачи  */
void     *p_async_get(future_s *future) P_NOEXCEPT;                   /* получение результата  */
void      p_async_free(future_s *future) P_NOEXCEPT;                  /* освобождение ресурсов  */

#endif  /* __PLC_ASYNC_H  */ 
