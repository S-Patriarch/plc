/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <inet.h>
 */

#ifndef __PLC_INET_H
#define __PLC_INET_H    1

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

#define P_MAXSLEEP      128

/* Проверка доступности и отзывчивости домена (хоста).
   hostname - имя домена (хоста - mail.ru)
   port - порт (80 - http, 443 - https)
   Возвращает 1 при доступности хоста, 0 при не доступности хоста.  */
extern unsigned char p_dcheck (const char *hostname, const char *port) 
        P_NOEXCEPT;

/* Переносимый способ соединения с повторением.
   Эта функция демонстрирует известный алгоритм с экспоненциальной задержкой.
   Если функция connect терпит неудачу, процесс приостанавливается на некоторое
   время и затем повторяет попытку, всякий раз увеличивая время задержки, пока
   оно не достигнет максимума - около 2 минут.  Из-за того что может 
   потребоваться создать новый сокет, нет смысла передавать дескриптор сокета
   функции p_connect_retry.
   Возвращает не признак успеха, а дескриптор сокета, подключенного к 
   удаленной стороне, -1 - в случае ошибки.  */
extern int p_connect_retry (int domain, int type, int protocol,
                            const struct sockaddr *addr, socklen_t addr_len) 
        P_NOEXCEPT;

P_END_DECLS

#endif /* inet.h  */
