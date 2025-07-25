/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : string.c
 */

#include <plc/string.h>
#include <plc/plcdef.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

int p_strrev(char *s)
{
        if (s == NULL || *s == '\0')
                return P_ERROR;

        size_t i = 0;
        size_t j = strlen(s) - 1;

        while (i < j) {
                char t = s[i];
                s[i++] = s[j];
                s[j--] = t;
        }

        return P_SUCCESS;
}

char **p_strspl(const char *s, size_t *w_count) 
{
        if (s == NULL || *s == '\0')
                return NULL;

        char *copy = strdup(s);
        if (copy == NULL)
                return NULL;

        *w_count = 0;
        char *temp = strtok(copy, " ");
        while (temp != NULL) {
                (*w_count)++;
                temp = strtok(NULL, " ");
        }

        char **words = (char **)malloc(*w_count * sizeof(char *));
        if (words == NULL) {
                free(copy);
                copy = NULL;
                return NULL;
        }

        strcpy(copy, s);
        size_t i = 0;
        temp = strtok(copy, " ");
        while (temp != NULL) {
                words[i] = strdup(temp);
                if (words[i] == NULL) {
                        for (size_t j = 0; j != i; j++)
                                free(words[j]);
                        free(words);
                        words = NULL;
                        free(copy);
                        copy = NULL;
                        return NULL;
                }
                i++;
                temp = strtok(NULL, " ");
        }

        free(copy);
        copy = NULL;

        return words;
}

void p_free_words(char **words, size_t w_count) 
{
        for (size_t i = 0; i != w_count; i++)
                free(words[i]);

        free(words);
        words = NULL;
}

size_t p_count_char(const char *s, char c) 
{
        if (s == NULL || *s == '\0')
                return 0;

        size_t count = 0;
        for (size_t i = 0; s[i] != '\0'; i++)
                if (s[i] == c)
                        count++;

        return count;
}

size_t p_count_words(const char *s) 
{
        if (s == NULL || *s == '\0')
                return 0;

        size_t count = 0;
        bool in_word = false;

        for (size_t i = 0; s[i] != '\0'; i++) {
                if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n') {
                        if (!in_word) {
                                count++;
                                in_word = true;
                        }
                } else
                        in_word = false;
        }

        return count;
}

size_t p_count_utf8_chars(const char *s) 
{
        if (s == NULL || *s == '\0')
                return 0;

        size_t count = 0;
        size_t i = 0;

        while (s[i] != '\0') {
                if ((s[i] & 0x80) == 0)
                        i += 1;                 /* однобайтовый символ 0xxxxxxx */
                else if ((s[i] & 0xE0) == 0xC0)
                        i += 2;                 /* двухбайтовый символ 110xxxxx */
                else if ((s[i] & 0xF0) == 0xE0)
                        i += 3;                 /* трехбайтовый символ 1110xxxx */
                else if ((s[i] & 0xF8) == 0xF0)
                        i += 4;                 /* четырехбайтовый символ 11110xxx */
                else
                        i += 1; /* некорректный utf8 символ, пропускаем его */

                count++;
        }

        return count;
}

size_t *p_extract_digits(const char *s, size_t *d_count) 
{
        if (s == NULL || *s == '\0') {
                *d_count = 0;
                return NULL;
        }

        /* подсчитываем количество цифр в строке */
        *d_count = 0;
        for (size_t i = 0; s[i] != '\0'; i++)
                if (isdigit(s[i]))
                        (*d_count)++;

        /* выделяем память под массив для хранения цифр */
        size_t *digits = (size_t *)malloc(*d_count * sizeof(size_t));
        if (digits == NULL) {
                *d_count = 0;
                return NULL;
        }

        /* заполняем массив цифрами */
        size_t itr = 0;
        for (size_t i = 0; s[i] != '\0'; i++)
                if (isdigit(s[i]))
                        digits[itr++] = s[i] - '0'; /* преобразуем символ в цифру */

        return digits;
}

int p_strcpy(char *to, char *from, int count) 
{
        if (from == NULL || *from == '\0')
                return P_ERROR;

        int n = (count + 7) / 8;

        switch (count % 8) {
        case 0: do { *to++ = *from++;
        case 7:      *to++ = *from++;
        case 6:      *to++ = *from++;
        case 5:      *to++ = *from++;
        case 4:      *to++ = *from++;
        case 3:      *to++ = *from++;
        case 2:      *to++ = *from++;
        case 1:      *to++ = *from++;
                } while (--n > 0);
        }

        return P_SUCCESS;
}

void *p_memcpy(void *dest, const void *src, size_t size) 
{
        unsigned char *d = (unsigned char*)dest;
        const unsigned char *s = (const unsigned char*)src;

        size_t i = 0;

        /* Копируем 16 байт за раз с помощью SSE2 */
        for (; i + 15 < size; i += 16) {
                __m128i chunk = _mm_loadu_si128((__m128i*)(s + i));
                _mm_storeu_si128((__m128i*)(d + i), chunk);
        }

        /* Копируем оставшиеся байты один за другим */
        for (; i < size; ++i) {
                d[i] = s[i];
        }

        return dest;
}

