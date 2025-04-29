/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : algorithm.c
 */

#include <plc/algorithm.h>
#include <stdlib.h>
#include <time.h>

int powi(int base, int exponent) 
{
        int res = 1;

        while (exponent > 0) {
                if ((exponent % 2) == 1)
                        res *= base;
                base *= base;
                exponent /= 2;
        }

        return res;
}

int bintodec(long long bin_number)
{
        int dec_number =0;
        int remainder;
        int i = 0;

        while (bin_number != 0) {
                remainder = bin_number % 10;
                bin_number /= 10;
                dec_number += remainder * powi(2, i);
                ++i;
        }

        return dec_number;
}

long long dectobin(int dec_number)
{
        long long bin_number = 0;
        int remainder;
        int i = 1;

        while (dec_number != 0) {
                remainder = dec_number % 2;
                dec_number /= 2;
                bin_number += remainder * i;
                i *= 10;
        }

        return bin_number;
}
 
int octtodec(int oct_number)
{
        int dec_number = 0;
        int remainder;
        int i = 0;

        while (oct_number != 0) {
                remainder = oct_number % 10;
                oct_number /= 10;
                dec_number += remainder * powi(8, i);
                ++i;
        }

        return dec_number;
}

int dectooct(int dec_number)
{
        int oct_number = 0;
        int remainder;
        int i = 1;

        while (dec_number != 0) {
                remainder = dec_number % 8;
                dec_number /= 8;
                oct_number += remainder * i;
                i *= 10;
        }

        return oct_number;
}

int bintooct(long long bin_number)
{
        int oct_number = 0;
        int dec_number = 0;
        int i = 0;

        while (bin_number != 0) {
                dec_number += (bin_number % 10) * powi(2, i);
                ++i;
                bin_number /= 10;
        }

        i = 1;
        while (dec_number != 0) {
                oct_number += (dec_number % 8) * i;
                dec_number /= 8;
                i *= 10;
        }

        return oct_number;
}

long long octtobin(int oct_number)
{
        long long bin_number = 0;
        int dec_number = 0;
        int i = 0;

        while (oct_number != 0) {
                dec_number += (oct_number % 10) * powi(8, i);
                ++i;
                oct_number /= 10;
        }

        i = 1;
        while (dec_number != 0) {
                bin_number += (dec_number % 2) * i;
                dec_number /= 2;
                i *= 10;
        }

        return bin_number;
}

int random_int(int min, int max)
{
        srand(time(NULL));
        return (min + rand() % (max - min + 1));
}

/*
 * Функция упаковывает 8 значений типа bool впеременную размером 
 * 1 байт.
 * Это значительно может сэкономить потребление памяти, поскольку
 * в логическом типе данных bool есть лишь два возможных значения
 * (true и false), которые могут быть представлены одним битом,
 * но по факту занимают целый байт памяти.
 * А это, в свою очередь, из-за того, что переменные используют
 * уникальные адреса памяти, а они выделяются только в байтах
 * переменная bool занимает 1 бит, а другие 7 бит - тратятся 
 * впустую.
 */
uint8_t pack_bools(bool b1, bool b2, bool b3, bool b4,
                   bool b5, bool b6, bool b7, bool b8)
{
        uint8_t p = 0;

        p |= b1 << 7;
        p |= b2 << 6;
        p |= b3 << 5;
        p |= b4 << 4;
        p |= b5 << 3;
        p |= b6 << 2;
        p |= b7 << 1;
        p |= b8;

        return p;
}

void unpack_bools(uint8_t p,
                  bool *b1, bool *b2, bool *b3, bool *b4,
                  bool *b5, bool *b6, bool *b7, bool *b8)
{
        *b1 = (p >> 7) & 1;
        *b2 = (p >> 6) & 1;
        *b3 = (p >> 5) & 1;
        *b4 = (p >> 4) & 1;
        *b5 = (p >> 3) & 1;
        *b6 = (p >> 2) & 1;
        *b7 = (p >> 1) & 1;
        *b8 = p & 1;
}

unsigned int strhash(const char *s) 
{
        unsigned int hash = 5381;
        char c;
        while ((c = *s++)) hash = hash*33 + c;
        return hash;
}

unsigned int ptr_list_hash(void **in) 
{
        unsigned int hash = 5381;
        void *c;
        while ((c = *in++)) hash = hash*33 + (uintptr_t)c;
        return hash;
}

