/*
 * (C) 2025-26, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : system.h
 */

#ifndef __PLC_SYSTEM_H
#define __PLC_SYSTEM_H  1

#include <plc/plcdef.h>
#include <time.h>
#include <sys/time.h>

/* Определение доступности языковой локали.  */
int     p_locale_available(const char *locale) P_NOEXCEPT;

/* Получает языковую локаль из параметров окружения пользователя.
 * Возвращает указатель на строку языковой локали в случае успеха, 
 * NULL - в случае ошибки. 
 *
 * После использования результата, необходимо освободить память:
 *      char *lang;
 *      lang = p_getlocale();
 *      free(lang);
 */
char   *p_getlocale(void) P_NOEXCEPT;

/* Функция-обертка для освобождения ресурса и обнуления указателя
 * на этот освобождаемый ресурс.
 *
 * Использование:
 *      int *ip = (int *)malloc(sizeof(int));
 *      p_free_ptr((void **)&ip);
 */
void    p_free_ptr(void **ptr) P_NOEXCEPT;

/* Данная функция используется при необходимости указания абсолютного времени, 
 * а не относительного. 
 *
 * Например, если потребуется ограничить время ожидания 3 минутами, мы должны
 * сохранить в структуре timespec не 3 минуты, а текущее время + 3 минуты.
 * Для этого можно воспользоваться функцией clock_gettime, возвращающей
 * текущее время в виде структуры timespec. Однако эта функция поддерживается
 * не всеми платформами. Вместо нее можно использовать функцию gettimeofday,
 * чтобы получить текущее время в виде структуры timeval, и затем 
 * преобразовать ее в структуру timespec.
 *
 * Возвращает 0 в случае успеха, -1 - в случае неудачи.
 */
int     p_make_timeout(struct timespec *tsp, long minutes) P_NOEXCEPT;

/* Надежная версия функции signal() с использованием функции sigaction()
 * стандарта POSIX.
 */
p_sigfunc * p_signal(int signo, p_sigfunc *func) P_NOEXCEPT;

#endif  /* __PLC_SYSTEM_H  */
