/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <net.h>
 */

#ifndef __PLC_NET_H
#define __PLC_NET_H     1

/* Современный TCP/IP интерфейс для UNIX-like систем.  
   
   Дизайн:
   - прозрачная типизация (скрываем int fd за struct);
   - RAII-подобное управление (p_net_free / p_net_close);
   - поддержка IPv4, IPv6 и getaddrinfo;
   - безопасная буферизация с struct net_buf_s;
   - TTL, keepalive, nodelay как флаги, а не магия setsockopt.
 
   Основная идея - хранить sockaddr_storage внутри net_ctx_s, чтобы
   bind/connect/accept работали без повторных вызовов getaddrinfo.  */

#define _POSIX_C_SOURCE 200112L

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include <plc/plcdef.h>

P_BEGIN_DECLS

/* 
 * Непрозрачные типы.  
 */

typedef struct net_ctx_s net_ctx_s;  /* слушатель / клиент  */
typedef struct net_buf_s net_buf_s;  /* буфер сообщений  */

/*
 * Конфигурация сокета при создании.
 */

typedef enum {
        P_NET_TCP,  /* TCP по умолчанию  */
        P_NET_UDP   /* зарезервировано для будущего  */
} net_proto_e;

typedef enum {
        P_NET_IPV4,
        P_NET_IPV6,
        P_NET_ANY   /* AF_UNSPEC - пробуем IPv6, fallback IPv4  */
} net_family_e;

typedef struct {
        net_family_e    family;  /* NET_IPV4 / NET_IPV6 / NET_ANY  */
        net_proto_e     proto;   /* NET_TCP  */
        const char     *node;    /* "example.com" или "192.168.1.1"  */
        const char     *port;    /* "http", "443", "8080"  */
        int             flags;   /* побитовое OR флагов ниже  */
} net_opts_s;

/* Флаги для net_opts.flags  */
#define P_NET_NONBLOCK          (1 << 0)  /* неблокирующий сокет  */
#define P_NET_REUSEADDR         (1 << 1)  /* SO_REUSEADDR  */
#define P_NET_REUSEPORT         (1 << 2)  /* SO_REUSEPORT (где доступен)  */
#define P_NET_KEEPALIVE         (1 << 3)  /* TCP keepalive  */
#define P_NET_NODELAY           (1 << 4)  /* TCP_NODELAY (откл. Nagle)  */
#define P_NET_V6ONLY            (1 << 5)  /* IPV6_V6ONLY для IPv6 сокетов  */

/* TTL для многоадресной или общей рассылки (по умолчанию системный).  */
#define P_NET_TTL_DEFAULT       0

/*
 * Создание и уничтожение.
 */

/* Создать TCP-сокет согласно опциям (клиент или серверный listen).
   Возвращает NULL при ошибке, детали в errno.  */
extern net_ctx_s *p_net_create (const net_opts_s *opts, int do_bind) P_NOEXCEPT;

/* bind + listen для сервера.
   backlog - размер очереди ожидающих соединений.
   Возвращает 0 при успехе, -1 при ошибке (errno).  */
extern int p_net_listen (net_ctx_s *ctx, int backlog) P_NOEXCEPT;

/* Принять входящее соединение (блокирующий или нет - зависит от флага).
   Возвращает нового net_ctx_s * или NULL + errno.  */
extern net_ctx_s *p_net_accept (net_ctx_s *ctx) P_NOEXCEPT;

/* Подключиться к серверу (клиент).
   Возвращает 0 при успехе, -1 при ошибке (errno).  */
extern int p_net_connect (net_ctx_s *ctx) P_NOEXCEPT;

/* Закрыть и освободить net_ctx_s (безопасно для NULL).  */
extern void p_net_free (net_ctx_s *ctx) P_NOEXCEPT;

/*
 * Чтение и запись.
 */

/* Принять сообщение.  
   Возвращает net_buf_s * (даже для 0 байт - FIN).  NULL означает ошибку 
   (проверяйте errno, EAGAIN для неблокирующего I/O).  */
extern net_buf_s *p_net_recv (net_ctx_s *ctx) P_NOEXCEPT;

/* Отправить len байт из data.
   Возвращает количество отправленных байт или -1 при ошибке.
   Для неблокирующих сокетов может вернуть < len.  */
extern int p_net_send (net_ctx_s *ctx, const void *data, size_t len) P_NOEXCEPT;

/* Удобная обертка: отправить строку (без завершающего нуля).
   Возвращает то же, что p_net_send.  */
extern int p_net_send_str (net_ctx_s *ctx, const char *str) P_NOEXCEPT;

/*
 * Управление буфером.
 */

/* Получить указатель на данные в буфере.  */
extern const void *p_net_buf_data (const net_buf_s *buf) P_NOEXCEPT;

/* Размер данных в буфере.  */
extern size_t p_net_buf_size (const net_buf_s *buf) P_NOEXCEPT;

/* Освободить буфер (безопасно для NULL).  */
extern void p_net_buf_free (net_buf_s *buf) P_NOEXCEPT;

/*
 * Дополнительно.
 */

/* Установить таймаут приема/передачи в миллисекундах.
   0 - отключить таймаут (вечная блокировка).
   Возвращает 0 при успехе, -1 при ошибке.  */
extern int p_net_set_timeout (net_ctx_s *ctx, int recv_ms, int send_ms) 
        P_NOEXCEPT;

/* Установить TTL (Time-To-Live) для сокета.
   Возвращает 0 при успехе, -1 при ошибке.  */
extern int p_net_set_ttl (net_ctx_s *ctx, int ttl) P_NOEXCEPT;

/* Получить файловый дескриптор для продвинутых операций (poll/epoll/select). 
   Возвращает файловый дескриптор при успехе, -1 при ошибке.  */
extern int p_net_get_fd (const net_ctx_s *ctx) P_NOEXCEPT;

P_END_DECLS

#endif /* net.h  */
