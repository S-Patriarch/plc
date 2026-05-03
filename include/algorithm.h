/* Copyright (C) 2025-2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <algorithm.h>
 */

#ifndef __PLC_ALGORITHM_H
#define __PLC_ALGORITHM_H       1

#include <stdbool.h>
#include <stdint.h>
#include <plc/plcdef.h>

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

P_END_DECLS

#endif /* algorithm.h  */
