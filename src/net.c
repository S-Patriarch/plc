/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            net.c
 */

#include <plc/net.h>

struct net_ctx_s {
        int               fd;
        int               family;      /* AF_INET / AF_INET6  */
        int               bound;       /* 1 если bind уже был вызван  */
        struct sockaddr_storage addr;  /* локальный или удаленный адрес  */
        socklen_t         addr_len;
        struct net_opts_s opts;
};

struct net_buf_s {
        char    data[P_BUFSIZ_65K];
        size_t  len;
};

/*
 * Вспомогательные функции.
 */

static int
_net_family_to_af(enum net_family_e f) P_NOEXCEPT
{
        switch (f) {
        case P_NET_IPV4:
                return(AF_INET);
        case P_NET_IPV6:
                return(AF_INET6);
        case P_NET_ANY:
                return(AF_UNSPEC);
        default:
                return(AF_UNSPEC);
        }
}

static int
_set_nonblocking(int fd, int enable) P_NOEXCEPT
{
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags == -1)
                return(-1);

        flags = enable ? (flags | O_NONBLOCK) : (flags & -O_NONBLOCK);

        return(fcntl(fd, F_SETFL, flags));
}

static int
_apply_sock_opts(int fd, int sock_flags) P_NOEXCEPT
{
        if (sock_flags & P_NET_REUSEADDR) {
                int val = 1;
                if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, 
                               &val, sizeof(val)) == -1)
                        return(-1);
        }

        if (sock_flags & P_NET_REUSEPORT) {
#ifdef SO_REUSEPORT
                int val = 1;
                if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, 
                               &val, sizeof(val)) == -1) {
                        /* На некоторых системах SO_REUSEPORT может быть
                           определен, но требовать прав root или особых
                           настроек ядра.
                           Считаем ошибку нефатальной.  */
                }
#else
                /* Ядро не поддерживает SO_REUSEPORT.  Игнорируем флаг
                   без ошибки, чтобы пользовательский код оставался
                   переносимым.  */
        (void)fd; /* подавляем предупреждение о неиспользуемой переменной  */
#endif
        }

        if (sock_flags & P_NET_KEEPALIVE) {
                int val = 1;
                if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, 
                               &val, sizeof(val)) == -1)
                        return(-1);
        }

        if (sock_flags & P_NET_NODELAY) {
                int val = 1;
                if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, 
                               &val, sizeof(val)) == -1)
                        return(-1);
        }

        return(0);
}

/*
 * Реализация функций интерфейса.
 */

struct net_ctx_s *
p_net_create(const struct net_opts_s *opts, int do_bind) P_NOEXCEPT 
{
        if (!opts || !opts->port) {
                errno = EINVAL;
                return(NULL);
        }

        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = _net_family_to_af(opts->family);
        hints.ai_socktype = (opts->proto == P_NET_UDP) ? SOCK_DGRAM : SOCK_STREAM;
        hints.ai_flags = (opts->node == NULL && do_bind) ? AI_PASSIVE : 0;

        struct addrinfo *res = NULL;
        int gai_err = getaddrinfo(opts->node, opts->port, &hints, &res);
        if (gai_err != 0) {
                errno = (gai_err == EAI_SYSTEM) ? errno : EADDRNOTAVAIL;
                return(NULL);
        }

        int fd = -1;
        struct addrinfo *rp;
        struct sockaddr_storage chosen_addr;
        socklen_t chosen_len = 0;

        for (rp = res; rp != NULL; rp = rp->ai_next) {
                fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
                if (fd == -1)
                        continue;

                if (_apply_sock_opts(fd, opts->flags) == -1) {
                        close(fd);
                        fd = -1;
                        continue;
                }

                if (opts->flags & P_NET_NONBLOCK) {
                        if (_set_nonblocking(fd, 1) == -1) {
                                close(fd);
                                fd = -1;
                                continue;
                        }
                }

                if ((opts->flags & P_NET_V6ONLY) && rp->ai_family == AF_INET6) {
                        int val = 1;
                        if (setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY,
                                       &val, sizeof(val)) == -1) {
                                close(fd);
                                fd = -1;
                                continue;
                        }
                }

                if (do_bind) {
                        if (bind(fd, rp->ai_addr, rp->ai_addrlen) == -1) {
                                close(fd);
                                fd = -1;
                                continue;
                        }
                }

                /* сохраняем адрес, который сработал  */
                memcpy(&chosen_addr, rp->ai_addr, rp->ai_addrlen);
                chosen_len = rp->ai_addrlen;
                break;
        }

        freeaddrinfo(res);

        if (fd == -1)
                return(NULL);

        struct net_ctx_s *ctx = (struct net_ctx_s *)calloc(1, sizeof(*ctx));
        if (!ctx) {
                close(fd);
                errno = ENOMEM;
                return(NULL);
        }

        ctx->fd = fd;
        ctx->family = chosen_addr.ss_family;
        ctx->bound = do_bind ? 1 : 0;
        ctx->addr_len = chosen_len;
        memcpy(&ctx->addr, &chosen_addr, chosen_len);
        ctx->opts = *opts;

        return(ctx);
}

int 
p_net_listen(struct net_ctx_s *ctx, int backlog) P_NOEXCEPT 
{
        if (!ctx || ctx->fd < 0) {
                errno = EINVAL;
                return(-1);
        }

        if (!ctx->bound) {
                errno = EINVAL;  /* нужно сначала bind  */
                return(-1);
        }

        if (listen(ctx->fd, backlog) == -1)
                return(-1);

        return(0);
}

struct net_ctx_s *
p_net_accept(struct net_ctx_s *ctx) P_NOEXCEPT 
{
        if (!ctx || ctx->fd < 0) {
                errno = EINVAL;
                return(NULL);
        }

        struct sockaddr_storage client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_fd = accept(ctx->fd,
                        (struct sockaddr *)&client_addr,
                        &client_len);
        if (client_fd == -1)
                return(NULL);

        /* применяем флаги сервера к клиентскому сокету  */
        if (_apply_sock_opts(client_fd, ctx->opts.flags) == -1) {
                close(client_fd);
                return(NULL);
        }

        if (ctx->opts.flags & P_NET_NONBLOCK) {
                if (_set_nonblocking(client_fd, 1) == -1) {
                        close(client_fd);
                        return(NULL);
                }
        }

        struct net_ctx_s *client_ctx = (struct net_ctx_s *)calloc(1, 
                                        sizeof(*client_ctx));
        if (!client_ctx) {
                close(client_fd);
                errno = ENOMEM;
                return(NULL);
        }

        client_ctx->fd = client_fd;
        client_ctx->family = client_addr.ss_family;
        client_ctx->bound = 0;  /* клиентский сокет не bind'ится  */
        client_ctx->addr_len = client_len;
        memcpy(&client_ctx->addr, &client_addr, client_len);
        client_ctx->opts = ctx->opts;

        return(client_ctx);
}

int 
p_net_connect(struct net_ctx_s *ctx) P_NOEXCEPT 
{
        if (!ctx || ctx->fd < 0) {
                errno = EINVAL;
                return(-1);
        }
        if (ctx->bound) {
                errno = EISCONN;
                return(-1);
        }

        int ret = connect(ctx->fd,
                          (const struct sockaddr *)&ctx->addr,
                          ctx->addr_len);
        if (ret == -1 && errno == EINPROGRESS) {
                /* неблокирующий сокет: соединение в процессе  */
                return(-1);
        }

        return(ret);
}

void 
p_net_free(struct net_ctx_s *ctx) P_NOEXCEPT 
{
        if (!ctx)
                return;

        if (ctx->fd >=0 ) {
                shutdown(ctx->fd, SHUT_RDWR);
                close(ctx->fd);
        }

        free(ctx);
}

struct net_buf_s *
p_net_recv(struct net_ctx_s *ctx) P_NOEXCEPT 
{
        if (!ctx || ctx->fd < 0) {
                errno = EINVAL;
                return(NULL);
        }

        struct net_buf_s *buf = (struct net_buf_s *)calloc(1, sizeof(*buf));
        if (!buf) {
                errno = ENOMEM;
                return(NULL);
        }

        ssize_t n = recv(ctx->fd, buf->data, P_BUFSIZ_65K, 0);
        if (n == -1) {
                int saved_errno = errno;
                free(buf);
                errno = saved_errno;
                return(NULL);
        }

        buf->len = (size_t)n;
        return(buf);
}

int 
p_net_send(struct net_ctx_s *ctx, const void *data, size_t len) P_NOEXCEPT 
{
        if (!ctx || ctx->fd < 0 || !data || len == 0) {
                errno = EINVAL;
                return(-1);
        }

        size_t total_sent = 0;
        const char *ptr = (const char *)data;

        while (total_sent < len) {
                ssize_t n = send(ctx->fd,
                                ptr + total_sent,
                                len - total_sent,
                                MSG_NOSIGNAL);  /* не отправлять SIGPIPE  */
                if (n == -1) {
                        if (errno == EINTR)
                                continue;  /* прервано сигналом - повторяем  */
                        if (total_sent > 0)
                                return((int)total_sent);  /* частичная отправка  */
                        return(-1);
                }
                if (n == 0) {
                        /* соединение закрыто  */
                        if (total_sent > 0)
                                return((int)total_sent);
                        errno = EPIPE;
                        return(-1);
                }
                total_sent += (size_t)n;

                /* для неблокирующих сокетов не зацикливаемся  */
                if (ctx->opts.flags & P_NET_NONBLOCK)
                        break;
        }

        return((int)total_sent);
}

int 
p_net_send_str(struct net_ctx_s *ctx, const char *str) P_NOEXCEPT 
{
        if (!str)
                return(0);

        return(p_net_send(ctx, str, strlen(str)));
}

const void *
p_net_buf_data(const struct net_buf_s *buf) P_NOEXCEPT 
{
        return(buf ? buf->data : NULL);
}

size_t 
p_net_buf_size(const struct net_buf_s *buf) P_NOEXCEPT 
{
        return(buf ? buf->len : 0);
}

void 
p_net_buf_free(struct net_buf_s *buf) P_NOEXCEPT 
{
        free(buf);
}

int 
p_net_set_timeout(struct net_ctx_s *ctx, int recv_ms, int send_ms) P_NOEXCEPT 
{
        if (!ctx || ctx->fd < 0) {
                errno = EINVAL;
                return(-1);
        }

        if (recv_ms > 0) {
                struct timeval tv;
                tv.tv_sec = recv_ms / 1000;
                tv.tv_usec = (recv_ms % 1000) * 1000;
                if (setsockopt(ctx->fd, SOL_SOCKET, SO_RCVTIMEO,
                               &tv, sizeof(tv)) == -1)
                        return(-1);
        } else if (recv_ms == 0) {
                /* отключаем таймаут  */
                struct timeval tv = { 0, 0 };
                setsockopt(ctx->fd, SOL_SOCKET, SO_RCVTIMEO,
                           &tv, sizeof(tv));
        }

        if (send_ms > 0) {
                struct timeval tv;
                tv.tv_sec = send_ms / 1000;
                tv.tv_usec = (send_ms % 1000) * 1000;
                if (setsockopt(ctx->fd, SOL_SOCKET, SO_SNDTIMEO,
                               &tv, sizeof(tv)) == -1)
                        return(-1);
        } else if (send_ms == 0) {
                struct timeval tv = { 0, 0 };
                setsockopt(ctx->fd, SOL_SOCKET, SO_SNDTIMEO,
                           &tv, sizeof(tv));
        }

        return(0);
}

int 
p_net_set_ttl(struct net_ctx_s *ctx, int ttl) P_NOEXCEPT 
{
        if (!ctx || ctx->fd < 0) {
                errno = EINVAL;
                return(-1);
        }
        if (ttl < 0 || ttl > 255) {
                errno = EINVAL;
                return(-1);
        }

        int level, optname;
        if (ctx->family == AF_INET6) {
                level = IPPROTO_IPV6;
                optname = IPV6_UNICAST_HOPS;
        } else {
                level = IPPROTO_IP;
                optname = IP_TTL;
        }

        if (setsockopt(ctx->fd, level, optname, &ttl, sizeof(ttl)) == -1)
                return(-1);

        return(0);
}

int 
p_net_get_fd(const struct net_ctx_s *ctx) P_NOEXCEPT 
{
        return(ctx ? ctx->fd : -1);
}

