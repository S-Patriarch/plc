/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <in.h>
 */

#ifndef __PLC_IN_H
#define __PLC_IN_H      1

P_BEGIN_DECLS

/* Максимальная длина строки адреса IPv4:
   "ddd.ddd.ddd.ddd\0"
    1234567890123456  */
#define P_INET_ADDRSTRLEN       16 

/* Максимальная длина строки адреса IPv6:
   "xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx" или
   "xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:ddd.ddd.ddd.ddd\0"
    1234567890123456789012345678901234567890123456  */
#define P_INET6_ADDRSTRLEN      46   

/* 
 * Определение номера порта, который может быть использован для 
 * взаимодействия клиент-сервер.  
 */
#define P_SERV_PORT     9877    /* клиент и сервер TCP и UDP  */
#define P_SERV_PORT_STR "9877"  /* клиент и сервер TCP и UDP  */

/* Потоковые клиенты и серверы домена Unix.  */
#define P_UNIXSTR_PATH  "/tmp/unix.str"

/* Клиенты и серверы протокола дейтаграмм домена Unix.  */
#define P_UNIXXDG_PATH  "tmp/unix.dg"

P_END_DECLS

#endif /* in.h  */
