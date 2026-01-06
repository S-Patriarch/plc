/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : system.c
 */

#include <plc/system.h>
#include <plc/plcdef.h>
#include <locale.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

int 
p_locale_available(const char *locale)
{
        char *old_locale = setlocale(LC_ALL, NULL);
        char *res = setlocale(LC_ALL, locale);

        if (res == NULL) 
                return(P_FALSE);

        setlocale(LC_ALL, old_locale);
        return(P_TRUE);
}

char *
p_getlocale(void) 
{
        const char     *env_name = "LANG";
        char           *env_val;
        char           *env_res;
        char           *dot_pos;
        size_t          len;

        env_val = getenv(env_name);
        if (env_name == NULL)
                return(NULL);

        /* Находим позицию первой точки в env_val */
        dot_pos = strchr(env_val, '.');

        /* Если точки нет, возвращаем копию всей строки */
        if (dot_pos == NULL) {
                env_res = strdup(env_val);
                return(env_res);
        }

        /* Вычисляем длину подстроки до точки */
        len = dot_pos - env_val;

        /* Выделяем память для подстроки (+1 для нулевого терминатора) */
        env_res = (char *)malloc(len + 1);
        if (env_res == NULL)
                return(NULL);

        /* Копируем подстроку */
        memcpy(env_res, env_val, len);
        env_res[len] = '\0';

        return(env_res);
}

void
p_free_ptr(void **ptr) 
{
        if (ptr != NULL && *ptr != NULL) {
                free(*ptr);
                *ptr = NULL;
        }
}

