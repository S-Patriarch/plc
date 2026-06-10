/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <crypto.h>
 */

#ifndef __PLC_CRYPTO_H
#define __PLC_CRYPTO_H  1

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

/* Функция хэширования строки.
   Авторство приписывается Дэниэлу Дж. Бернстайну.
   Для каждого символа в строке (или каждого байта многобайтного символа UTF-8)
   вычисленный ранее результат умножается на 33 и к произведению прибавляется
   текущий символ (или байт). 
   С высокой вероятностью произойдет переполнение переменной типа
   unsigned int, в которой хэш сохраняется, но это всего лишь еще один неявный
   детерминированный шаг алгоритма.  */
extern unsigned int p_strhash32 (const char *s) P_NOEXCEPT;
extern unsigned long long p_strhash64 (const char *s) P_NOEXCEPT;

/* Хэширование списка указателей  */
extern unsigned int p_ptr_list_hash (void **in) P_NOEXCEPT;

/* Функции p_crle и p_drle сжимают и востанавливают строки с помощью RLE
   (Run-Length Encoding).
   Пример входной строки: AAAABBBCCDAA
   Ожидаемый выход:       4A3B2C1D2A
   Возвращают 0 при успешном завершении, -1 при ошибке.  */
extern int p_crle (const char *s, char *out) P_NOEXCEPT;
extern int p_drle (const char *s, char *out) P_NOEXCEPT;

/* Подсчет количества единичных (установленных) битов в 64-битном числе
   (также известно как population count, popcount, или вес Хэмминга).
   Этот алгоритм (метод Wilkes-Wheeler-Gill) был опубликован в 1957 году.
   Он считает все 64 бита параллельно с помощью арифметики над битовыми масками,
   вместо того чтобы проходить по каждому биту в цикле. 
   Функция принимает 64-битное беззнаковое число и возвращает количество битов,
   равных 1. 
   Например:
        p_popcnt64(0b1011)              -> 3
        p_popcnt64(0)                   -> 0
        p_popcnt64(0xFFFFFFFFFFFFFFFF)  -> 64
   
   Пример использования:
   unsigned long long int value = 0x0F0F0F0F0F0F0F0F;
   int bs = p_popcnt64(value); // 32 (каждая маска 0xF дает 4 бита x 8 байт)  */
extern unsigned long long int p_popcnt64 (unsigned long long int x) P_NOEXCEPT;

P_END_DECLS

#endif /* crypto.h  */
