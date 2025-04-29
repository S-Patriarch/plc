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

int powi(int, int); /* бинарное возведение в степень */

int bintodec(long long); /* преобразование 2-ного числа в 10-ное */
long long dectobin(int); /* преобразование 10-ного числа в 2-ное */
int octtodec(int);       /* преобразование 8-ного числа в 10-ное */
int dectooct(int);       /* преобразование 10-ного числа в 8-ное */
int bintooct(long long); /* преобразование 2-ного числа в 8-ное */
long long octtobin(int); /* преобразование 8-ного числа в 2-ное */

int random_int(int, int); /* генерация случайного числа */

/* упаковка 8 значений bool в 1 байт */
uint8_t pack_bools(bool, bool, bool, bool,
                   bool, bool, bool, bool);
/* распаковка 1 байта на 8 значений типа bool */
void unpack_bools(uint8_t,
                  bool *, bool *, bool *, bool *,
                  bool *, bool *, bool *, bool *);

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
unsigned int strhash(const char *);

/* Хэширование списка указателей */
unsigned int ptr_list_hash(void **);

#endif  /* __PLC_ALGORITHM_H */
