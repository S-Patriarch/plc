/*
 * (C) 2026, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : crypto.h
 */

#ifndef __PLC_CRYPTO_H
#define __PLC_CRYPTO_H  1

#include <plc/plcdef.h>

/* Функция хэширования строки.
 * Авторство приписывается Дэниэлу Дж. Бернстайну.
 * 
 * Для каждого символа в строке (или каждого байта многобайтного символа UTF-8)
 * вычисленный ранее результат умножается на 33 и к произведению прибавляется
 * текущий символ (или байт). 
 * С высокой вероятностью произойдет переполнение переменной типа
 * unsigned int, в которой хэш сохраняется, но это всего лишь еще один неявный
 * детерминированный шаг алгоритма.
 */
unsigned int            p_strhash32(const char *s) P_NOEXCEPT;
unsigned long long      p_strhash64(const char *s) P_NOEXCEPT;

/* Хэширование списка указателей  */
unsigned int            p_ptr_list_hash(void **in) P_NOEXCEPT;

#endif  /* __PLC_CRYPTO_H  */
