/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            inet.c
 */

#include <plc/inet.h>

unsigned char
p_dcheck(const char *hostname, const char *port) P_NOEXCEPT
{
        struct addrinfo hints;
        struct addrinfo *res;
        struct timeval tv;

        int sockfd = -1;
        unsigned char retstatus = P_TRUE;

        /* получаем информацию об адресе хоста  */
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;     /* поддерживаем IPv4 и IPv6  */
        hints.ai_socktype = SOCK_STREAM; /* используем TCP  */
        if (getaddrinfo(hostname, port, &hints, &res) < 0) {
                retstatus = P_FALSE;
                goto reterr;
        }

        /* создаем сокет  */
        sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sockfd < 0) {
                retstatus = P_FALSE;
                goto reterr;
        }

        /* устанавливаем таймауты  */
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
        setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

        /* пытаемся подключиться  */
        if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0)
                retstatus = P_FALSE;

reterr:
        freeaddrinfo(res);
        close(sockfd);
        return (retstatus);
}

int 
p_connect_retry(int domain, int type, int protocol,
                const struct sockaddr *addr, socklen_t addr_len) P_NOEXCEPT 
{
        int     numsec, fd;

        /* Попытаться установить соединение с экспоненциальной задержкой.  */
        for (numsec = 1; numsec <= P_MAXSLEEP; numsec <<= 1) {
                if ((fd = socket(domain, type, addr_len)) < 0)
                        return(-1);
                if (connect(fd, addr, addr_len) == 0) {
                        /* Соединение установлено.  */
                        return(fd);
                }
                close(fd);

                /* Задержка перед следующей попыткой.  */
                if (numsec <= P_MAXSLEEP/2)
                        sleep(numsec);
        }

        return(-1);
}

int 
p_init_server(int type, const struct sockaddr *addr, socklen_t addr_len, 
              int qlen) P_NOEXCEPT 
{
        int     fd, err;
        int     reuse = 1;

        if ((fd = socket(addr->sa_family, type, 0)) < 0)
                return(-1);
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) < 0)
                goto errout;
        if (bind(fd, addr, addr_len) < 0)
                goto errout;
        if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
                if (listen(fd, qlen) < 0)
                        goto errout;
        return(fd);

errout:
        err = errno;
        close(fd);
        errno = err;
        return(-1);
}

