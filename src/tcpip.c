/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : tcpip.c
 */

#include "p_tcpip.h"
#include <plc/tcpip.h>
#include <plc/plcdef.h>

#include <stddef.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#include <errno.h>
#include <stdio.h>

Sigfunc *_signal(int signo, Sigfunc *func) {}
ssize_t _my_read(int fd, char *ptr) {}
ssize_t _readline(int fd, void *vptr, size_t maxlen) {}
ssize_t _readlinebuf(void **vptrptr) {}
ssize_t _writen(int fd, const void *vptr, size_t n) {}

int tcp_socket(int domain, int type, int protocol) 
{
        int n = 0;
        if ((n = socket(domain, type, protocol)) < 0)
                return ERROR;
        return n;
}

int tcp_bind(int fd, const struct sockaddr *addr, socklen_t len) 
{
        if (bind(fd, addr, len) < 0)
                return ERROR;
        return SUCCESS;
}

int tcp_listen(int fd, int n) 
{
        if (listen(fd, n) < 0)
                return ERROR;
        return SUCCESS;
}

int tcp_accept(int fd, struct sockaddr *addr, socklen_t *len) 
{
        int n = 0;
        if ((n = accept(fd, addr, len)) < 0)
                return ERROR;
        return n;
}

int tcp_connect(int fd, const struct sockaddr *addr, socklen_t len) 
{
        if (connect(fd, addr, len) < 0)
                return ERROR;
        return SUCCESS;
}

int tcp_close(int fd) 
{
        if (close(fd) < 0)
                return ERROR;
        return SUCCESS;
}

ssize_t tcp_recv(int fd, void *ptr, size_t nbytes, int flags) 
{
        ssize_t n = 0;
        if ((n = recv(fd, ptr, nbytes, flags)) < 0)
                return ERROR;
        return n;
}

int tcp_send(int fd, const void *ptr, size_t nbytes, int flags) 
{
        if (send(fd, ptr, nbytes, flags) < 0)
                return ERROR;
        return SUCCESS;
}

ssize_t tcp_read(int fd, void *ptr, size_t nbytes) 
{
        ssize_t n = 0;
        if ((n = read(fd,ptr,nbytes)) < 0)
                return ERROR;
        return n;
}

ssize_t tcp_readline(int fd, void *ptr, size_t maxlen) 
{
        ssize_t n = 0;
        if ((n = _readline(fd, ptr, maxlen)) < 0)
                return ERROR;
        return n;
}

int tcp_write(int fd, const void *ptr, size_t nbytes) 
{
        if (write(fd, ptr, nbytes) < 0)
                return ERROR;
        return SUCCESS;
}

int tcp_writen(int fd, const void *ptr, size_t nbytes) 
{
        if (_writen(fd, ptr, nbytes) != nbytes)
                return ERROR;
        return SUCCESS;
}

pid_t tcp_fork(void) 
{
        pid_t pid = 0;
        if ((pid = fork()) < 0)
                return ERROR;
        return pid;
}

Sigfunc *tcp_signal(int signo, Sigfunc *func) 
{
        Sigfunc *sigfunc;
        if ((sigfunc = _signal(signo, func)) == SIG_ERR)
                return NULL;
        return sigfunc;
}

int tcp_shutdown(int fd, int how) 
{
        if (shutdown(fd, how) < 0)
                return ERROR;
        return SUCCESS;
}

char *tcp_fgets(char *str, int count, FILE *stream) 
{
        char *rstr; 
        if ((rstr = fgets(str, count, stream)) == NULL && ferror(stream))
                return NULL;
        return rstr;
}

int tcp_fputs(const char *str, FILE *stream) 
{
        if (fputs(str, stream) == feof(stream))
                return ERROR;
        return SUCCESS;
}

int tcp_pthread_create(pthread_t *tid, 
                       const pthread_attr_t *attr, 
                       void *(*func)(void*), 
                       void *arg) 
{
        int n = 0;
        if ((n = pthread_create(tid, attr, func, arg)) == 0) return SUCCESS;
        else return ERROR;
}

int tcp_pthread_join(pthread_t tid, void **status) 
{
        int n = 0;
        if ((n = pthread_join(tid, status)) == 0) return SUCCESS;
        else return ERROR;
}

int tcp_pthread_detach(pthread_t tid) 
{
        int n = 0;
        if ((n = pthread_detach(tid)) == 0) return SUCCESS;
        else return ERROR;
}

int tcp_pthread_kill(pthread_t tid, int signo) 
{
        int n = 0;
        if ((n = pthread_kill(tid, signo)) == 0) return SUCCESS;
        else return ERROR;
}

