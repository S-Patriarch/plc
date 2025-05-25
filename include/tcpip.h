/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : tcpip.h
 */

#ifndef __PLC_TCPIP_H
#define __PLC_TCPIP_H   1

#include <sys/socket.h>
#include <stddef.h>

const int LISTENQ     = 1024; // максимальное количество клиентских соединений 
const int MAXLINE     = 4096; // максимальная длина текстовой строки
const int BUFFSIZE    = 8192; // размер буфера для чтения и записи
const int MAXSOCKADDR = 128;  // максимальный размер структуры адреса сокета
const int SERV_PORT   = 9877; // клиент-серверы TCP и UDP

typedef void Sigfunc(int); // для обработчиков сигналов

typedef struct tcpip {
        int   _read_cnt;
        char *_read_ptr;
        char  _read_buf[MAXLINE];
} tcpip_s;

/* создание сокета */
int tcp_socket(int domain, int type, int protocol);

/* привязка сокета */
int tcp_bind(int fd, const struct sockaddr *addr, socklen_t len);

/* прослушивание подключений */
int tcp_listen(int fd, int n);

/* прием данных */
int tcp_accept(int fd, struct sockaddr *addr, socklen_t *len);

/* установка соединения */
int tcp_connect(int fd, const struct sockaddr *addr, socklen_t len);

/* закрытие созданного сокета */
int tcp_close(int fd);

/* чтение данных из сокета */
ssize_t tcp_recv(int fd, void *ptr, size_t nbytes, int flags);

/* запись данных в сокет */
int tcp_send(int fd, const void *ptr, size_t nbytes, int flags);

/* чтение данных из потока */
ssize_t tcp_read(int fd, void *ptr, size_t nbytes);
ssize_t tcp_readline(int fd, void *ptr, size_t maxlen);

/* запись данных в поток */
int tcp_write(int fd, const void *ptr, size_t nbytes);
int tcp_writen(int fd, const void *ptr, size_t nbytes);

#endif  /* __PLC_TCPIP_H */
