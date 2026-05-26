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

/* Проверка доступности и отзывчивости домена (хоста).
   hostname - имя домена (хоста - mail.ru)
   port - порт (80 - http, 443 - https)
   Возвращает 1 при доступности хоста, 0 при не доступности хоста.  */
unsigned char p_dcheck (const char *hostname, const char *port) P_NOEXCEPT;

P_END_DECLS

#endif /* inet.h  */
