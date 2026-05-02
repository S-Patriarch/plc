/*
 * (C) 2025-26, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : io.c
 */

#include <plc/io.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int 
p_mout(const char *message, enum mess_format format, 
       const char *attr_m, const char *attr_f) P_NOEXCEPT
{
        if (message == NULL || *message == '\0') 
                return(P_ERROR);

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
                return(P_ERROR);

        if (attr_m == NULL) 
                res_m = printf("%s", message);
        else 
                res_m = printf("%s%s\033[0m", attr_m, message);
        
        if (res_m == EOF) 
                return(P_ERROR);

        return(res_f + res_m);
}

int 
p_cout(const char *s) P_NOEXCEPT
{
        if (s == NULL || *s == '\0') return(P_ERROR);

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

        return(count);
}

int
p_puts(const char *s) P_NOEXCEPT
{
        if (s == NULL || *s == '\0') return(-1);

        size_t len = 0;
        while (len[s] != '\0') len++;

        if (write(STDOUT_FILENO, s, len) == -1) return(-1);
        if (write(STDOUT_FILENO, "\n", 1) == -1) return(-1);

        fflush(stdout);
        return(0);
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

