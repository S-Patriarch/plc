/*
 * (C) 2025-26, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : algorithm.h
 */

#ifndef __PLC_ALGORITHM_H
#define __PLC_ALGORITHM_H       1

#include <stdbool.h>
#include <stdint.h>
#include <plc/plcdef.h>

int        p_bintodec(long long bin_number) P_NOEXCEPT; /* преобразование 2-ного числа в 10-ное  */
long long  p_dectobin(int dec_number) P_NOEXCEPT;       /* преобразование 10-ного числа в 2-ное  */
int        p_octtodec(int oct_number) P_NOEXCEPT;       /* преобразование 8-ного числа в 10-ное  */
int        p_dectooct(int dec_number) P_NOEXCEPT;       /* преобразование 10-ного числа в 8-ное  */
int        p_bintooct(long long bin_number) P_NOEXCEPT; /* преобразование 2-ного числа в 8-ное  */
long long  p_octtobin(int oct_number) P_NOEXCEPT;       /* преобразование 8-ного числа в 2-ное  */

/* Функции p_bintohex и p_hextobin возвращают результирующую
 * строку, память которой в последствии необходимо освободить.
 * Пример:
 *      res = p_bintohex(input);
 *      ...
 *      free(res);
 */
char   *p_bintohex(char bin_number[]) P_NOEXCEPT;       /* преобразование 2-ного числа в 16-ное  */
char   *p_hextobin(char hex_number[]) P_NOEXCEPT;       /* преобразование 16-ного числа в 2-ное  */

int     p_powi(int base, int exponentnt) P_NOEXCEPT;    /* бинарное возведение в степень  */
int     p_random_int(int min, int max) P_NOEXCEPT;      /* генерация случайного числа  */

/* упаковка 8 значений bool в 1 байт  */
uint8_t p_pack_bools(bool b1, bool b2, bool b3, bool b4,
                     bool b5, bool b6, bool b7, bool b8) P_NOEXCEPT;

/* распаковка 1 байта на 8 значений типа bool  */
void    p_unpack_bools(uint8_t p,                            
                       bool *b1, bool *b2, bool *b3, bool *b4,
                       bool *b5, bool *b6, bool *b7, bool *b8) P_NOEXCEPT;

/* упаковка 8 значений unsigned char в 1 байт  */
void    p_pack_chars(unsigned char *p,
                     unsigned char c1, unsigned char c2, 
                     unsigned char c3, unsigned char c4, 
                     unsigned char c5, unsigned char c6, 
                     unsigned char c7, unsigned char c8) P_NOEXCEPT;

/* распаковка 1 байта на 8 значений типа unsigned char  */
void    p_unpack_chars(unsigned char p,
                       unsigned char *c1, unsigned char *c2, 
                       unsigned char *c3, unsigned char *c4, 
                       unsigned char *c5, unsigned char *c6, 
                       unsigned char *c7, unsigned char *c8) P_NOEXCEPT;

#endif  /* __PLC_ALGORITHM_H  */
