/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : algorithm.h
 */

#ifndef __PLC_ALGORITHM_H
#define __PLC_ALGORITHM_H       1

#include <stdbool.h>
#include <stdint.h>

int p_powi(int base, int exponentnt);  /* бинарное возведение в степень */

int p_bintodec(long long bin_number);  /* преобразование 2-ного числа в 10-ное */
long long p_dectobin(int dec_number);  /* преобразование 10-ного числа в 2-ное */
int p_octtodec(int oct_number);        /* преобразование 8-ного числа в 10-ное */
int p_dectooct(int dec_number);        /* преобразование 10-ного числа в 8-ное */
int p_bintooct(long long bin_number);  /* преобразование 2-ного числа в 8-ное */
long long p_octtobin(int oct_number);  /* преобразование 8-ного числа в 2-ное */

int p_random_int(int min, int max);    /* генерация случайного числа */

/* упаковка 8 значений bool в 1 байт */
uint8_t p_pack_bools(bool b1, bool b2, bool b3, bool b4,
                     bool b5, bool b6, bool b7, bool b8);

/* распаковка 1 байта на 8 значений типа bool */
void p_unpack_bools(uint8_t p,                            
                    bool *b1, bool *b2, bool *b3, bool *b4,
                    bool *b5, bool *b6, bool *b7, bool *b8);

/*
 * Функция хэширования строки.
 * Авторство приписывается Дэниэлу Дж. Бернстайну.
 * 
 * Для каждого символа в строке (или каждого байта многобайтного символа UTF-8)
 * вычисленный ранее результат умножается на 33 и к произведению прибавляется
 * текущий символ (или байт). 
 * С высокой вероятностью произойдет переполнение переменной типа
 * unsigned int, в которой хэш сохраняется, но это всего лишь еще один неявный
 * детерминированный шаг алгоритма.
 */
unsigned int p_strhash(const char *s);

/* Хэширование списка указателей */
unsigned int p_ptr_list_hash(void **in);

#endif  /* __PLC_ALGORITHM_H */
