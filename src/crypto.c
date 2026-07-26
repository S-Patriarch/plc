/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            crypto.c
 */

#include <plc/crypto.h>

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

        return 0;
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

        return 0;
}

int 
p_crle_simple(const unsigned char *input, int input_size,
              unsigned char *output, int output_max_size) P_NOEXCEPT 
{
        int             ipos = 0, opos = 0;
        int             count = 0;
        unsigned char   current_byte;

        while (ipos < input_size) {
                /* Считаем сколько одинаковых байт подряд.  */
                current_byte = input[ipos]; 
                count = 1;
                while (ipos + count < input_size &&
                       input[ipos+count] == current_byte &&
                       count < 255) { /* ограничиваем длину серии 255
                                         (максимум в 1 байт)  */
                        count++;
                }

                /* Записываем пару: [количество][байт]  
                   Проверяем, хватит ли места в выходном буфере 
                   (нужно 2 байта).  */
                if (opos + 2 > output_max_size)
                        return(-1);   /* переполнение выходного буфера  */
                output[opos++] = (unsigned char)count;
                output[opos++] = current_byte;

                /* Перемещаем позицию во входном потоке.  */
                ipos += count;
        }
        
        return(opos);
}

int 
p_drle_simple(const unsigned char *input, int input_size,
              unsigned char *output, int output_max_size) P_NOEXCEPT 
{
        int             ipos = 0, opos = 0;
        int             count = 0;
        int             i;
        unsigned char   byte;

        while (ipos < input_size) {
                /* Проверяем, что можем прочитать пару [счетчик][байт].  */
                if (ipos + 2 > input_size)
                        return(-1); /* некорректные данные  */

                count = input[ipos++];
                byte = input[ipos++];

                /* Проверяем, не выйдем ли за пределы выходного буфера.  */
                if (opos + count > output_max_size)
                        return(-1); /* переполнение  */

                /* Записываем count раз byte.  */
                for (i = 0; i < count; i++)
                        output[opos++] = byte;
        }

        return(opos);
}

int 
p_crle_escape(const unsigned char *input, int input_size,
              unsigned char *output, int output_max_size) P_NOEXCEPT 
{
        int             ipos = 0, opos = 0;
        int             count = 0;
        int             i;
        unsigned char   current_byte;

        while (ipos < input_size) {
                current_byte = input[ipos];
                count = 1;
                /* Считаем длину серии (макс. 255, так как счетчик - 1 байт).  */
                while (ipos + count < input_size &&
                       input[ipos + count] == current_byte &&
                       count < 255) {
                        count++;
                }

                /* Если байтов >= 3 или это escape-байт, кодируем как серию.  */
                if (count >= 3 || current_byte == P_ESCAPE_BYTE) {
                        output[opos++] = P_ESCAPE_BYTE;
                        output[opos++] = (unsigned char)count;
                        output[opos++] = current_byte;
                } else {
                        /* Иначе записываем как сырые данные (1 или 2 байта).  */
                        for (i = 0; i < count; i++) {
                                if (opos + 1 > output_max_size)
                                        return(-1);
                                output[opos++] = current_byte;
                        }
                }
                ipos += count;
        }

        return(opos);
}

int 
p_drle_escape(const unsigned char *input, int input_size,
              unsigned char *output, int output_max_size) P_NOEXCEPT 
{
        int             ipos = 0, opos = 0;
        int             count = 0;
        int             i;
        unsigned char   byte, repeated_byte;

        while (ipos < input_size) {
                byte = input[ipos++];

                if (byte == P_ESCAPE_BYTE) {
                        /* если встретили escape-байт, читаем счетчик и 
                           сам байт.  */
                        if (ipos + 2 > input_size)
                                return(-1); /* битые данные  */

                        count = input[ipos++];
                        repeated_byte = input[ipos++];

                        if (opos + count > output_max_size)
                                return(-1);

                        /* Если count == 0, это был сам байт 0xFF в 
                           исходных данных.  */
                        if (count == 0) {
                                output[opos++] = P_ESCAPE_BYTE;
                        } else {
                                for (i = 0; i < count; i++)
                                        output[opos++] = repeated_byte;
                        }
                } else {
                        /* Обычный одиночный байт.  */
                        if (opos + 1 > output_max_size)
                                return(-1);
                        output[opos++] = byte;
                }
        }

        return(opos);
}

unsigned long long int 
p_popcnt64(unsigned long long int x) P_NOEXCEPT 
{
        x -= (x >> 1) & 0x5555555555555555ULL;
        x  = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
        x  = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
        return((x * 0x0101010101010101ULL) >> 56);
}

uint32_t 
p_adler32(const unsigned char *buf, size_t len) P_NOEXCEPT 
{
        uint32_t a = 1;
        uint32_t b = 0;
        size_t   i, chunk, end;

        for (i = 0; i < len;) {
                chunk = P_ADLER32_NMAX;
                if (chunk > len - i)
                        chunk = len - 1;

                end = i + chunk;
                for (; i < end; i++) {
                        a += buf[i];
                        b += a;
                }

                a %= P_ADLER32_MOD;
                b %= P_ADLER32_MOD;
        }

        return((b << 16) | a);
}

uint64_t 
p_adler64(const unsigned char *buf, size_t len) P_NOEXCEPT 
{
        uint64_t a = 1;
        uint64_t b = 0;
        size_t   i, chunk, end;

        for (i = 0; i < len;) {
                chunk = P_ADLER64_NMAX;
                if (chunk > len - i)
                        chunk = len - 1;

                end = i + chunk;
                for (; i < end; i++) {
                        a += buf[i];
                        b += a;
                }

                a %= P_ADLER64_MOD;
                b %= P_ADLER64_MOD;
        }

        return((b << 32) | a);
}

uint32_t 
p_djb2_hash32(const unsigned char *buf, size_t len) P_NOEXCEPT 
{
        uint32_t hash = 5381;
        size_t   i;

        if (buf == NULL || len == 0)
                return(hash);

        for (i = 0; i < len; i++)
                hash = ((hash << 5) + hash) + buf[i];

        return(hash);
}

uint64_t 
p_djb2_hash64(const unsigned char *buf, size_t len) P_NOEXCEPT 
{
        uint64_t hash = 5381;
        size_t   i;

        if (buf == NULL || len == 0)
                return(hash);

        for (i = 0; i < len; i++)
                hash = ((hash << 5) + hash) + buf[i];

        return(hash);
}

