/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            inet.c
 */

#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <plc/inet.h>
#include <plc/plcdef.h>

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

