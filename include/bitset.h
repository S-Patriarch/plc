/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <bitset.h>
 */

#ifndef __PLC_BITSET_H
#define __PLC_BITSET_H  1

/* Данная реализация является попыткой написания аналога std::bitset из С++.
   Используется динамический массив 64-битных слов для хранения битов и 
   предоставляет все основные методы.

   Ключевые особенности:
        Индексация справа налево - bs[0] это младший бит (как в двоичном
        представлении).

   Пример использования:
        struct bitset_s *bs = p_bitset_create(100);
        p_bitset_set(bs, 42, 1);
        p_bitset_flip(bs, 73);
        printf("Count: %zu\n", p_bitset_count(bs));
        char *str = p_bitset_to_string(bs);
        printf("Bits: %s\n", str);
        free(str);
        p_bitset_destroy(bs);  */

#include <stddef.h>
#include <stdint.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

#define P_BITSET_WORDBITS       (64)

struct bitset_s {
        unsigned long long int *bs_data;        /* массив слов  */
        size_t bs_numbits;                      /* количество бит  */
        size_t bs_numwords;                     /* количество слов  */
};
 
/* Создание и уничтожение.  */
extern struct bitset_s *p_bitset_create (size_t numbits) P_NOEXCEPT;
extern void p_bitset_destroy (struct bitset_s *bs) P_NOEXCEPT;
extern struct bitset_s *p_bitset_clone (const struct bitset_s *bs) P_NOEXCEPT;

/* Доступ к битам.  */
extern unsigned char p_bitset_test (const struct bitset_s *bs, size_t pos) 
        P_NOEXCEPT;
extern void p_bitset_set (struct bitset_s *bs, size_t pos, unsigned char value) 
        P_NOEXCEPT;
extern void p_bitset_set_all (struct bitset_s *bs, unsigned char value) 
        P_NOEXCEPT;
extern void p_bitset_flip (struct bitset_s *bs, size_t pos) P_NOEXCEPT;
extern void p_bitset_flip_all (struct bitset_s *bs) P_NOEXCEPT;
extern void p_bitset_reset (struct bitset_s *bs, size_t pos) P_NOEXCEPT;
extern void p_bitset_reset_all (struct bitset_s *bs) P_NOEXCEPT;

/* Побитовые операции над всем bitset.  */
extern void p_bitset_and (struct bitset_s *dst, const struct bitset_s *src) 
        P_NOEXCEPT;
extern void p_bitset_or (struct bitset_s *dst, const struct bitset_s *src) 
        P_NOEXCEPT;
extern void p_bitset_xor (struct bitset_s *dst, const struct bitset_s *src) 
        P_NOEXCEPT;
extern void p_bitset_not (struct bitset_s *bs) P_NOEXCEPT;

/* Сдвиги.  */
extern void p_bitset_shl (struct bitset_s *bs, size_t shift) P_NOEXCEPT;
extern void p_bitset_shr (struct bitset_s *bs, size_t shift) P_NOEXCEPT;

/* Запросы.  */
extern size_t p_bitset_count (const struct bitset_s *bs) P_NOEXCEPT;
extern size_t p_bitset_size (const struct bitset_s *bs) P_NOEXCEPT;
extern unsigned char p_bitset_all (const struct bitset_s *bs) P_NOEXCEPT;
extern unsigned char p_bitset_any (const struct bitset_s *bs) P_NOEXCEPT;
extern unsigned char p_bitset_none (const struct bitset_s *bs) P_NOEXCEPT;

/* Конвертация.  */
extern unsigned long long int p_bitset_to_ullong (const struct bitset_s *bs) 
        P_NOEXCEPT;
extern char *p_bitset_to_string (const struct bitset_s *bs) P_NOEXCEPT;
extern struct bitset_s *p_bitset_from_string (const char *str) P_NOEXCEPT;

/* Операции сравнения.  */
extern unsigned char p_bitset_equal (const struct bitset_s *a, 
                                     const struct bitset_s *b) P_NOEXCEPT;
extern unsigned char p_bitset_not_equal (const struct bitset_s *a, 
                                         const struct bitset_s *b) P_NOEXCEPT;

/* Хеширование.  */
extern unsigned long long int p_bitset_hash (const struct bitset_s *bs) 
        P_NOEXCEPT;

/* Итерация по установленным битам.  */
typedef void (*p_bitset_foreach_cb)(size_t pos, void *user_data);
extern void p_bitset_foreach_set (const struct bitset_s *bs, 
                                  p_bitset_foreach_cb cb, 
                                  void *user_data) P_NOEXCEPT;

P_END_DECLS

#endif /* bitset.h  */
