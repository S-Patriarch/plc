/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <thread.h>
 */

#ifndef __PLC_THREAD_H
#define __PLC_THREAD_H  1

#include <pthread.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

/* Создание потока в обособленном состоянии.
   Атрибут потока detachstate устанавливается в PTHREAD_CREATE_DETACHED.
   Возвращает 0 в случае успеха, код ошибки - в случае неудачи.  */
extern int p_thread_detach (void *(*fn)(void *), void *arg) P_NOEXCEPT;

P_END_DECLS

#endif /* thread.h  */
