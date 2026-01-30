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
#include <plc/plcdef.h>

typedef void Sigfunc(int); /* для обработчиков сигналов  */

/* создание сокета  */
int     p_tcp_socket(int domain, int type, int protocol) P_NOEXCEPT;

/* привязка сокета  */
int     p_tcp_bind(int fd, const struct sockaddr *addr, socklen_t len) P_NOEXCEPT;

/* прослушивание подключений  */
int     p_tcp_listen(int fd, int n) P_NOEXCEPT;

/* прием данных  */
int     p_tcp_accept(int fd, struct sockaddr *addr, socklen_t *len) P_NOEXCEPT;

/* установка соединения  */
int     p_tcp_connect(int fd, const struct sockaddr *addr, socklen_t len) P_NOEXCEPT;

/* закрытие созданного сокета  */
int     p_tcp_close(int fd) P_NOEXCEPT;

/* чтение данных из сокета  */
ssize_t p_tcp_recv(int fd, void *ptr, size_t nbytes, int flags) P_NOEXCEPT;

/* запись данных в сокет  */
int     p_tcp_send(int fd, const void *ptr, size_t nbytes, int flags) P_NOEXCEPT;

/* чтение данных из потока  */
ssize_t p_tcp_read(int fd, void *ptr, size_t nbytes) P_NOEXCEPT;
ssize_t p_tcp_readline(int fd, void *ptr, size_t maxlen) P_NOEXCEPT;

/* запись данных в поток  */
int     p_tcp_write(int fd, const void *ptr, size_t nbytes) P_NOEXCEPT;
int     p_tcp_writen(int fd, const void *ptr, size_t nbytes) P_NOEXCEPT;

/* порождение дочернего процесса  */
pid_t   p_tcp_fork(void) P_NOEXCEPT;

/* обработчик сигналов  */
Sigfunc *p_tcp_signal(int signo, Sigfunc *func) P_NOEXCEPT;

/* установка признака конца файла в стандартном потоке ввода
 * отправка сегмента FIN 
 */
int     p_tcp_shutdown(int fd, int how) P_NOEXCEPT;

/* ввод из потока  */
char   *p_tcp_fgets(char *str, int count, FILE *stream) P_NOEXCEPT;

/* вывод в поток  */
int     p_tcp_fputs(const char *str, FILE *stream) P_NOEXCEPT;

/* создание потока  */
int     p_tcp_pthread_create(pthread_t *tid, 
                             const pthread_attr_t *attr, 
                             void *(*func)(void*), 
                             void *arg) P_NOEXCEPT;

/* завершение потока  */
int     p_tcp_pthread_join(pthread_t tid, void **status) P_NOEXCEPT;

/* отсоединение потока  */
int     p_tcp_pthread_detach(pthread_t tid) P_NOEXCEPT;

/* уничтожение потока  */
int     p_tcp_pthread_kill(pthread_t tid, int signo) P_NOEXCEPT;

#endif  /* __PLC_TCPIP_H  */
