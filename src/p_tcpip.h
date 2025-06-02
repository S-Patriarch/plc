/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : p_tcpip.h
 */

#ifndef __PLC_P_TCPIP_H
#define __PLC_P_TCPIP_H         2

#include <stddef.h>
#include <sys/types.h>

#define LISTENQ         1024  // максимальное количество клиентских соединений 
#define MAXLINE         4096  // максимальная длина текстовой строки
#define BUFFSIZE        8192  // размер буфера для чтения и записи
#define MAXSOCKADDR     128   // максимальный размер структуры адреса сокета
#define SERV_PORT       9877  // клиент-серверы TCP и UDP

typedef void Sigfunc(int); // для обработчиков сигналов

typedef struct tcpip {
        int   _read_cnt;
        char *_read_ptr;
        char  _read_buf[MAXLINE];
} tcpip_s;

Sigfunc *_signal(int signo, Sigfunc *func);
ssize_t _my_read(int fd, char *ptr);
ssize_t _readline(int fd, void *vptr, size_t maxlen);
ssize_t _readlinebuf(void **vptrptr);
ssize_t _writen(int fd, const void *vptr, size_t n);

#endif  /* __PLC_P_TCPIP_H */
