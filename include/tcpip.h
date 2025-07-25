/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : tcpip.h
 */

#ifndef __PLC_TCPIP_H
#define __PLC_TCPIP_H   1

#include <sys/socket.h>
#include <sys/types.h>
#include <stddef.h>
#include <stdio.h>
#include <pthread.h>

typedef void Sigfunc(int); /* для обработчиков сигналов */

/* создание сокета */
int p_tcp_socket(int domain, int type, int protocol);

/* привязка сокета */
int p_tcp_bind(int fd, const struct sockaddr *addr, socklen_t len);

/* прослушивание подключений */
int p_tcp_listen(int fd, int n);

/* прием данных */
int p_tcp_accept(int fd, struct sockaddr *addr, socklen_t *len);

/* установка соединения */
int p_tcp_connect(int fd, const struct sockaddr *addr, socklen_t len);

/* закрытие созданного сокета */
int p_tcp_close(int fd);

/* чтение данных из сокета */
ssize_t p_tcp_recv(int fd, void *ptr, size_t nbytes, int flags);

/* запись данных в сокет */
int p_tcp_send(int fd, const void *ptr, size_t nbytes, int flags);

/* чтение данных из потока */
ssize_t p_tcp_read(int fd, void *ptr, size_t nbytes);
ssize_t p_tcp_readline(int fd, void *ptr, size_t maxlen);

/* запись данных в поток */
int p_tcp_write(int fd, const void *ptr, size_t nbytes);
int p_tcp_writen(int fd, const void *ptr, size_t nbytes);

/* порождение дочернего процесса */
pid_t p_tcp_fork(void);

/* обработчик сигналов */
Sigfunc *p_tcp_signal(int signo, Sigfunc *func);

/* установка признака конца файла в стандартном потоке ввода
   отправка сегмента FIN */
int p_tcp_shutdown(int fd, int how);

/* ввод из потока */
char *p_tcp_fgets(char *str, int count, FILE *stream);

/* вывод в поток */
int p_tcp_fputs(const char *str, FILE *stream);

/* создание потока */
int p_tcp_pthread_create(pthread_t *tid, 
                         const pthread_attr_t *attr, 
                         void *(*func)(void*), 
                         void *arg);

/* завершение потока */
int p_tcp_pthread_join(pthread_t tid, void **status);

/* отсоединение потока */
int p_tcp_pthread_detach(pthread_t tid);

/* уничтожение потока */
int p_tcp_pthread_kill(pthread_t tid, int signo);

#endif  /* __PLC_TCPIP_H */
