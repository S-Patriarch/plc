/*
 * (C) 2025-26, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : algorithm.c
 */

#include <plc/algorithm.h>
#include <plc/string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int 
p_bintodec(long long bin_number) P_NOEXCEPT
{
        int dec_number =0;
        int remainder;
        int i = 0;

        while (bin_number != 0) {
                remainder = bin_number % 10;
                bin_number /= 10;
                dec_number += remainder * p_powi(2, i);
                ++i;
        }

        return dec_number;
}

long long 
p_dectobin(int dec_number) P_NOEXCEPT
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
 
int 
p_octtodec(int oct_number) P_NOEXCEPT
{
        int dec_number = 0;
        int remainder;
        int i = 0;

        while (oct_number != 0) {
                remainder = oct_number % 10;
                oct_number /= 10;
                dec_number += remainder * p_powi(8, i);
                ++i;
        }

        return dec_number;
}

int 
p_dectooct(int dec_number) P_NOEXCEPT
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

int 
p_bintooct(long long bin_number) P_NOEXCEPT
{
        int oct_number = 0;
        int dec_number = 0;
        int i = 0;

        while (bin_number != 0) {
                dec_number += (bin_number % 10) * p_powi(2, i);
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

long long 
p_octtobin(int oct_number) P_NOEXCEPT
{
        long long bin_number = 0;
        int dec_number = 0;
        int i = 0;

        while (oct_number != 0) {
                dec_number += (oct_number % 10) * p_powi(8, i);
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

char *
p_bintohex(char bin_number[]) P_NOEXCEPT 
{
        int     i, j;
        int     length = strlen(bin_number);
        int     padding = (4 - (length % 4)) % 4;
        char    padded_bin[129];
        char    hex_adecimal[33] = "";

        memset(padded_bin, '0', padding);
        strcpy(padded_bin + padding, bin_number);

        char   *bin_hex_digits[] = {
                "0000", "0001", "0010", "0011", "0100", "0101",
                "0110", "0111", "1000", "1001", "1010", "1011",
                "1100", "1101", "1110", "1111"
        };
        for (i = 0; i < length + padding; i += 4) {
                char group[5];
                strncpy(group, padded_bin + i, 4);
                group[4] = '\0';
                for (j = 0; j < 16; j++) {
                        if (strcmp(group, j[bin_hex_digits]) == 0) {
                                char hex_digit[2];
                                sprintf(hex_digit, "%X", j);
                                strcat(hex_adecimal, hex_digit);
                                break;
                        }
                }
        }

        return(strdup(hex_adecimal));
}

char *
p_hextobin(char hex_number[]) P_NOEXCEPT 
{
        unsigned int    hexnum;
        int             i = 0;
        char            binary[33] = "";

        sscanf(hex_number, "%x", &hexnum);
        while (hexnum) {
                i++[binary] = '0' + hexnum % 2;
                hexnum /= 2;
        }
        i[binary] = '\0';
        p_strrev(binary);

        return(strdup(binary));
}

int
p_powi(int base, int exponent) P_NOEXCEPT 
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

int 
p_random_int(int min, int max) P_NOEXCEPT
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
uint8_t 
p_pack_bools(bool b1, bool b2, bool b3, bool b4,
             bool b5, bool b6, bool b7, bool b8) P_NOEXCEPT
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

void 
p_unpack_bools(uint8_t p,
               bool *b1, bool *b2, bool *b3, bool *b4,
               bool *b5, bool *b6, bool *b7, bool *b8) P_NOEXCEPT
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

void 
p_pack_chars(unsigned char *p,
             unsigned char c1, unsigned char c2, 
             unsigned char c3, unsigned char c4, 
             unsigned char c5, unsigned char c6, 
             unsigned char c7, unsigned char c8) P_NOEXCEPT 
{
        *p |= c1 << 7;
        *p |= c2 << 6;
        *p |= c3 << 5;
        *p |= c4 << 4;
        *p |= c5 << 3;
        *p |= c6 << 2;
        *p |= c7 << 1;
        *p |= c8;
}

void 
p_unpack_chars(unsigned char p,
               unsigned char *c1, unsigned char *c2, 
               unsigned char *c3, unsigned char *c4, 
               unsigned char *c5, unsigned char *c6, 
               unsigned char *c7, unsigned char *c8) P_NOEXCEPT 
{
        *c1 = (p >> 7) & 1;
        *c2 = (p >> 6) & 1;
        *c3 = (p >> 5) & 1;
        *c4 = (p >> 4) & 1;
        *c5 = (p >> 3) & 1;
        *c6 = (p >> 2) & 1;
        *c7 = (p >> 1) & 1;
        *c8 = p & 1;
}

