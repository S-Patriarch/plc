/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : p_tcpip.h
 */

#ifndef __PLC_P_TCPIP_H
#define __PLC_P_TCPIP_H         2

const int LISTENQ     = 1024; // максимальное количество клиентских соединений 
const int MAXLINE     = 4096; // максимальная длина текстовой строки
const int BUFFSIZE    = 8192; // размер буфера для чтения и записи
const int MAXSOCKADDR = 128;  // максимальный размер структуры адреса сокета
const int SERV_PORT   = 9877; // клиент-серверы TCP и UDP

typedef struct tcpip {
        int   _read_cnt;
        char *_read_ptr;
        char  _read_buf[MAXLINE];
} tcpip_s;

#endif  /* __PLC_P_TCPIP_H */
