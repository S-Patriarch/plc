/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : tcpip.c
 */

#define _GNU_SOURCE

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

tcpip_s t;

Sigfunc *_signal(int signo, Sigfunc *func) 
{
        struct sigaction act;
        struct sigaction oact;
         
        act.sa_handler = func;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;

        if (signo == SIGALRM) {
                #ifdef SA_INTERRUPT
                act.sa_flags |= SA_INTERRUPT; // SunOS 4.x
                #endif
        } else {
                #ifdef SA_RESTART
                act.sa_flags |= SA_RESTART; // SVR4, 44BSD
                #endif
        }
        if (sigaction(signo, &act, &oact) < 0) return SIG_ERR;
        return oact.sa_handler;
}

ssize_t _my_read(int fd, char *ptr) 
{
        if (t._read_cnt <= 0) {
                again:
                if ((t._read_cnt = read(fd, t._read_buf, sizeof(t._read_buf))) < 0) {
                        if (errno == EINTR) goto again;
                        return -1;
                } else if (t._read_cnt == 0) 
                        return 0;
                t._read_ptr = t._read_buf;
        }
        t._read_cnt--;
        *ptr = *t._read_ptr++;
        return 1;
}

ssize_t _readline(int fd, void *vptr, size_t maxlen) 
{
        char    c;
        char    *ptr = (char *)vptr;
        ssize_t n = 0;
        ssize_t rc = 0;

        for (n = 1; n < maxlen; n++) {
                if ((rc = _my_read(fd, &c)) == 1) {
                        *ptr++ = c;
                        if (c == '\n') break;
                } else if (rc == 0) {
                        *ptr = 0;
                        return n - 1;
                } else return -1;
        }
        *ptr = 0;
        return n;
}

ssize_t _readlinebuf(void **vptrptr) 
{
        if (t._read_cnt) *vptrptr = t._read_ptr;
        return t._read_cnt;
}

ssize_t _writen(int fd, const void *vptr, size_t n) 
{
        const char *ptr = (const char *)vptr;
        size_t     nleft = n;
        ssize_t    nwritten = 0;

        while (nleft > 0) {
                if ((nwritten = write(fd, ptr, nleft)) <= 0) {
                        if (nwritten < 0 && errno == EINTR) nwritten = 0;
                        else return -1;
                }
                nleft -= nwritten;
                ptr += nwritten;
        }
        return n;
}

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

