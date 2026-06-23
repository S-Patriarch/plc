/* Copyright (C) 2025-2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            io.c
 */

#include <plc/io.h>

int 
p_mout(const char *message, enum mess_format format, 
       const char *attr_m, const char *attr_f) P_NOEXCEPT
{
        if (message == NULL || *message == '\0') 
                return (P_ERROR);

        char sf[4];
        int res_m, res_f;

        if (format == mess_n)
                strcpy(sf, "");
        if (format == mess_w)
                strcpy(sf, "W: ");
        if (format == mess_e)
                strcpy(sf, "E: ");

        if (attr_f == NULL) 
                res_f = printf("%s", sf);
        else 
                res_f = printf("%s%s\033[0m", attr_f, sf);
        
        if (res_f == EOF) 
                return (P_ERROR);

        if (attr_m == NULL) 
                res_m = printf("%s", message);
        else 
                res_m = printf("%s%s\033[0m", attr_m, message);
        
        if (res_m == EOF) 
                return (P_ERROR);

        return (res_f + res_m);
}

int 
p_cout(const char *s) P_NOEXCEPT
{
        if (s == NULL || *s == '\0') return (P_ERROR);

        size_t count = 0;       
        while (*s != '\0') {
                int char_len = 1;
                if ((*s & 0xE0) == 0xC0) char_len = 2;
                else if ((*s & 0xF0) == 0xE0) char_len = 3;
                else if ((*s & 0xF8) == 0xF0) char_len = 4;

                for (int i = 0; i < char_len && *s != '\0'; ++i)
                        putchar(*s++);

                fflush(stdout);
                ++count;
        }

        return (count);
}

int
p_puts(const char *s) P_NOEXCEPT
{
        if (s == NULL || *s == '\0') return (-1);

        size_t len = 0;
        while (len[s] != '\0') len++;

        if (write(STDOUT_FILENO, s, len) == -1) return (-1);
        if (write(STDOUT_FILENO, "\n", 1) == -1) return (-1);

        fflush(stdout);
        return (0);
}

void 
p_gets(char *dst, size_t len) P_NOEXCEPT 
{
        int c;
        while ((c = getchar()) != EOF && c != '\n' && len != 1) {
                *dst++ = c;
                --len;
        }
        *dst = '\0';
}

ssize_t
p_readn(int fd, void *buf, size_t nbytes) P_NOEXCEPT 
{
        size_t          nleft;
        ssize_t         nread;

        nleft = nbytes;
        while (nleft > 0) {
                if ((nread = read(fd, buf, nleft)) < 0) {
                        if (nleft == nbytes)
                                return(-1);  /* ошибка, вернуть -1  */
                        else
                                break;  /* ошибка, вернуть количество 
                                           прочитанных байтов  */
                } else if (nread == 0) {
                        break;  /* конец файла  */
                }
                nleft -= nread;
                buf += nread;
        }

        return(nbytes - nleft);  /* возвращаем значение >= 0  */
}

ssize_t
p_writen(int fd, const void *buf, size_t nbytes) P_NOEXCEPT 
{
        size_t          nleft;
        ssize_t         nwritten;

        nleft = nbytes;
        while (nleft > 0) {
                if ((nwritten = write(fd, buf, nleft)) < 0) {
                        if (nleft == nbytes)
                                return(-1);  /* ошибка, вернуть -1  */
                        else
                                break;  /* ошибка, вернуть количество 
                                           записанных байтов  */
                } else if (nwritten == 0) {
                        break;
                }
                nleft -= nwritten;
                buf += nwritten;
        }

        return(nbytes - nleft);  /* возвращаем значение >= 0  */
}

