/*
 * (C) 2026, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : crypto.c
 */

#include <plc/crypto.h>
#include <plc/plcdef.h>
#include <stdint.h>

unsigned int 
p_ptr_list_hash(void **in) P_NOEXCEPT 
{
        unsigned int hash = 5381;
        void *c;
        while ((c = *in++)) hash = hash*33 + (uintptr_t)c;
        return hash;
}

unsigned int 
p_strhash32(const char *s) P_NOEXCEPT
{
        unsigned int    hash = 5381;
        unsigned char   c;

        while ((c = (unsigned char)*s++)) 
                hash = hash * 33 + c;

        return(hash);
}

unsigned long long
p_strhash64(const char *s) P_NOEXCEPT
{
        unsigned long long      hash = 14695981039346656037ULL;
        unsigned long long      prime = 1099511628211ULL;
        unsigned char           c;

        while ((c = (unsigned char)*s++)) {
                hash ^= c;
                hash *= prime;

                /* дополнительное перемешивание для защиты от коллизий  */
                hash ^= hash >> 31;
                hash ^= hash << 13;
        }

        return(hash);
}

