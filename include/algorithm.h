/* Copyright (C) 2025-2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <algorithm.h>
 */

#ifndef __PLC_ALGORITHM_H
#define __PLC_ALGORITHM_H       1

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <plc/plcdef.h>
#include <plc/string.h>

P_BEGIN_DECLS

/* Преобразование 2-ного числа в 10-ное.  */
extern int p_bintodec (long long bin_number) P_NOEXCEPT;

/* Преобразование 10-ного числа в 2-ное.  */
extern long long p_dectobin (int dec_number) P_NOEXCEPT;

/* Преобразование 8-ного числа в 10-ное.  */
extern int p_octtodec (int oct_number) P_NOEXCEPT;

/* Преобразование 10-ного числа в 8-ное.  */
extern int p_dectooct (int dec_number) P_NOEXCEPT;

/* Преобразование 2-ного числа в 8-ное.  */
extern int p_bintooct (long long bin_number) P_NOEXCEPT;

/* Преобразование 8-ного числа в 2-ное.  */
extern long long p_octtobin (int oct_number) P_NOEXCEPT;

/* Функции p_bintohex и p_hextobin возвращают результирующую
   строку, память которой в последствии необходимо освободить.
   Пример:
        res = p_bintohex(input);
        ...
        free(res);  */
/* Преобразование 2-ного числа в 16-ное.  */
extern char *p_bintohex (char bin_number[]) P_NOEXCEPT;
/* Преобразование 16-ного числа в 2-ное.  */
extern char *p_hextobin (char hex_number[]) P_NOEXCEPT;

/* Бинарное возведение в степень.  */
extern int p_powi (int base, int exponentnt) P_NOEXCEPT;

/* Генерация случайного числа.  */
extern int p_random_int (int min, int max) P_NOEXCEPT;

/* Упаковка 8 значений bool в 1 байт.  */
extern uint8_t p_pack_bools (bool b1, bool b2, bool b3, bool b4,
                             bool b5, bool b6, bool b7, bool b8) P_NOEXCEPT;

/* Распаковка 1 байта на 8 значений типа bool.  */
extern void p_unpack_bools (uint8_t p,                            
                            bool *b1, bool *b2, bool *b3, bool *b4,
                            bool *b5, bool *b6, bool *b7, bool *b8) P_NOEXCEPT;

/* Упаковка 8 значений unsigned char в 1 байт.  */
extern void p_pack_chars (unsigned char *p,
                          unsigned char c1, unsigned char c2, 
                          unsigned char c3, unsigned char c4, 
                          unsigned char c5, unsigned char c6, 
                          unsigned char c7, unsigned char c8) P_NOEXCEPT;

/* Распаковка 1 байта на 8 значений типа unsigned char.  */
extern void p_unpack_chars (unsigned char p,
                            unsigned char *c1, unsigned char *c2, 
                            unsigned char *c3, unsigned char *c4, 
                            unsigned char *c5, unsigned char *c6, 
                            unsigned char *c7, unsigned char *c8) P_NOEXCEPT;

/* Умножение с защитой от переполнения.
   Работает всегда, но ьедленно (O(log b))  */
extern unsigned long long int p_mod_mul (unsigned long long int a,
                                         unsigned long long int b,
                                         unsigned long long int mod) P_NOEXCEPT;

/* Быстрое возведение в степень по модулю.  */
extern unsigned long long int p_mod_pow (unsigned long long int base,
                                         unsigned long long int exp,
                                         unsigned long long int mod) P_NOEXCEPT;

/* Определяет, является ли число 'a' доказательством составности n.
   Тест Миллера-Рабина.
   Если ни одно из последовательных возведений в квадрат не дало n-1,
   значит 'a' - свидетель того, что n является составным числом.
   Возвращает 0 если a не свидетель (n, возможно, простое), 
              1 если a - свидетель составности n.  */
extern unsigned char p_witness (unsigned long long int a,
                                unsigned long long int n) P_NOEXCEPT;

/* Находит позицию младшего установленного бита.
   Нахождение производится с помощью одного умножения и одного обращения к
   таблице.  Константа 0x077CB531 выбрана не случайно.  Это последовательность
   де Брейна.  Если умножить ее на изолированный бит, старшие биты образуют
   уникальный паттерн для каждой возможной позиции бита.  Одно умножение
   заменяет целый цикл.  */
extern int p_lowest_setbit (uint32_t v) P_NOEXCEPT;

/* Суммирование чисел с плавающей точкой.
   Уильям Кэхэн предложил эту идею в 1965 году.  Он также разработал стандарт
   IEEE 754 для предоставления чисел с плавающей точкой.  Алгоритм суммирования
   Кэхэна сохраняет информацию об утерянных битах и добавляет ее обратно в
   следующие вычисления.  Данный алгоритм особенно полезен при суммировании
   большого количества чисел или чисел с сильно различающимися порядками, так
   как он компенсирует ошибки округления.
   Аргументы:
        float nums[] = { 0.1f, 0.2f, 0.3f, 0.4f, 0.5f };
        int count = sizeof(nums) / sizeof(nums[0]);  */
extern float p_kahansum (float *nums, int count) P_NOEXCEPT;

P_END_DECLS

#endif /* algorithm.h  */
