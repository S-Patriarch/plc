/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            crypto.c
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <plc/crypto.h>
#include <plc/plcdef.h>

unsigned int 
p_ptr_list_hash(void **in) P_NOEXCEPT 
{
        unsigned int hash = 5381;
        void *c;
        while ((c = *in++)) hash = hash*33 + (uintptr_t)c;
        return (hash);
}

unsigned int 
p_strhash32(const char *s) P_NOEXCEPT
{
        unsigned int    hash = 5381;
        unsigned char   c;

        while ((c = (unsigned char)*s++)) 
                hash = hash * 33 + c;

        return (hash);
}

unsigned long long
p_strhash64(const char *s) P_NOEXCEPT
{
        unsigned long long      hash = 14695981039346656037ULL;
        unsigned long long      prime = 1099511628211ULL;
        unsigned char           c;

        while ((c = (unsigned char)*s++)) {
                hash ^= c;
                hash *= prime;

                /* дополнительное перемешивание для защиты от коллизий  */
                hash ^= hash >> 31;
                hash ^= hash << 13;
        }

        return (hash);
}

int
p_crle(const char *s, char *out) P_NOEXCEPT
{
        int     len = strlen(s);
        int     i, cnt, written, out_pos;
        char    c;

        if (s == NULL || *s == '\0' || out == NULL)
                return (P_ERROR);

        out_pos = 0;

        for (i = 0; i < len;) {
                c = i[s];
                cnt = 1;

                /* подсчет количества повторяющихся символов  */
                while ((i+cnt) < len && (i+cnt)[s] == c)
                        ++cnt;

                /* проверка на переполнение буфера 
                   (каждая группа: число + символ + возможно завершающий ноль)
                   число cnt может занимать до 10 цифр (для 32-битного int)
                   10 цифр + 1 символ = 11 байт на группу, плюс место для 
                   завершающего нуля  */
                if ((out_pos + 12) > (len * 2 + 1))
                        /* недостаточно места в выходном буфере  */
                        return (P_ERROR);

                /* формируем число в строку прямо в out  */
                written = snprintf(out + out_pos, 12, "%d", cnt);
                if (written < 0)
                        return (P_ERROR); /* ошибка форматирования  */
                out_pos += written;

                /* добавляем символ  */                
                out_pos++[out] = c;
                
                i += cnt;
        }

        /* добавляем завершающий нуль-терминатор  */
        out_pos[out] = '\0';

        return (P_SUCCESS);
}

int
p_drle(const char *s, char *out) P_NOEXCEPT
{
        int     i, j, cnt, out_pos;
        char    c;

        if (s == NULL || *s == '\0' || out == NULL)
                return (P_ERROR);

        out_pos = 0;

        for (i = 0; i[s] != '\0';) {
                cnt = 0;

                /* парсим число  */
                while (isdigit(i[s])) {
                        cnt = cnt * 10 + (i[s] - '0');
                        i++;
                }

                /* проверка на ошибки: нет числа или нет символа после числа  */
                if (cnt == 0 || i[s] == '\0')
                        return (P_ERROR);
                
                c = i[s];
                i++; 
                
                /* добавляем символ cnt раз  */
                for (j = 0; j < cnt; j++)
                        out_pos++[out] = c;
        }

        /* добавляем завершающий нуль-терминатор  */
        out_pos[out] = '\0';

        return (P_SUCCESS);
}

