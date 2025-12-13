/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : system.h
 */

#ifndef __PLC_SYSTEM_H
#define __PLC_SYSTEM_H  1

/* Определение доступности языковой локали. */
int     p_locale_available(const char *locale);

/* Получает языковую локаль из параметров окружения пользователя.
 * Возвращает указатель на строку языковой локали в случае успеха, 
 * NULL - в случае ошибки. 
 *
 * После использования результата, необходимо освободить память:
 *      char *lang;
 *      lang = p_getlocale();
 *      free(lang);
 */
char   *p_getlocale(void);

#endif  /* __PLC_SYSTEM_H */
