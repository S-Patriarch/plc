/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : tcpip.c
 */

#include <plc/tcpip.h>
#include <plc/plcdef.h>

#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>

#include <errno.h>
#include <stdio.h>

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

