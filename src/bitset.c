/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : bitset.c
 */

#include <plc/bitset.h>
#include <plc/plcdef.h>
#include <stdlib.h>
#include <assert.h>

/* Вычисление количества слов для заданного числа бит.  */
static inline size_t
p_words_for_bits (size_t n) 
{
        return ((n + P_BITSET_WORDBITS - 1) / P_BITSET_WORDBITS);
}

/* Получить маску для последнего слова (чтобы игнорировать лишние биты).  */
static inline unsigned long long int
p_last_word_mask (size_t numbits) 
{
        size_t remainder = numbits % P_BITSET_WORDBITS;
        if (remainder == 0) return (UINT64_MAX);
        return (((unsigned long long int)1 << remainder) - 1);
}

/* Создание нового bitset. */
struct bitset_s *
p_bitset_create (size_t numbits) P_NOEXCEPT 
{
        struct bitset_s *bs = (struct bitset_s *)malloc (sizeof (struct bitset_s));
        if (!bs) return (NULL);

        bs->bs_numbits = numbits;
        bs->bs_numwords = p_words_for_bits (numbits);
        bs->bs_data = (unsigned long long int *)calloc (bs->bs_numwords,
                                                        sizeof (unsigned long long int));

        if (!bs->bs_data) {
                free (bs);
                return (NULL);
        }
        return (bs);
}

/* Уничтожение bitset.  */
void 
p_bitset_destroy (struct bitset_s *bs) P_NOEXCEPT 
{
        if (bs) {
                free (bs->bs_data);
                free (bs);
        }
}

/* Создание копии bitset.  */
struct bitset_s *
p_bitset_clone (const struct bitset_s *bs) P_NOEXCEPT 
{
        struct bitset_s *clone = p_bitset_create (bs->bs_numbits);
        if (clone) 
                memcpy (clone->bs_data, bs->bs_data, bs->bs_numwords *
                        sizeof (unsigned long long int));
        return (clone);
}

/* Проверка бита.  */
unsigned char 
p_bitset_test (const struct bitset_s *bs, size_t pos) P_NOEXCEPT 
{
        assert (pos < bs->bs_numbits);
        size_t word_index = pos / P_BITSET_WORDBITS;
        size_t bit_offset = pos % P_BITSET_WORDBITS;
        return ((word_index[bs->bs_data] >> bit_offset) & 1);
}

/* Установка значения бита.  */
void 
p_bitset_set (struct bitset_s *bs, size_t pos, unsigned char value) P_NOEXCEPT 
{
        assert (pos < bs->bs_numbits);
        size_t word_index = pos / P_BITSET_WORDBITS;
        size_t bit_offset = pos % P_BITSET_WORDBITS;

        if (value)
                word_index[bs->bs_data] |= ((unsigned long long int)1 <<
                                            bit_offset);
        else
                word_index[bs->bs_data] &= ~((unsigned long long int)1 <<
                                             bit_offset);
}

/* Установить все биты в заданное значение.  */
void 
p_bitset_set_all (struct bitset_s *bs, unsigned char value) P_NOEXCEPT 
{
        unsigned long long int fill_value = value ? UINT64_MAX : 0;
        memset (bs->bs_data, fill_value ? 0xFF : 0, 
                bs->bs_numwords * sizeof (unsigned long long int));

        /* корректируем последнее слово  */
        if (bs->bs_numbits % P_BITSET_WORDBITS != 0)
                (bs->bs_numwords-1)[bs->bs_data] &= p_last_word_mask (bs->bs_numbits);
}

/* Инвертировать бит.  */
void 
p_bitset_flip (struct bitset_s *bs, size_t pos) P_NOEXCEPT 
{
        assert (pos < bs->bs_numbits);
        size_t word_index = pos / P_BITSET_WORDBITS;
        size_t bit_offset = pos % P_BITSET_WORDBITS;
        word_index[bs->bs_data] ^= ((unsigned long long int)1 << bit_offset);
}

/* Инвертировать все биты.  */
void 
p_bitset_flip_all (struct bitset_s *bs) P_NOEXCEPT 
{
        size_t i;

        for (i = 0; i < bs->bs_numwords; i++)
                i[bs->bs_data] = ~(i[bs->bs_data]);
        
        /* корректируем последнее слово  */
        if (bs->bs_numbits % P_BITSET_WORDBITS != 0)
                (bs->bs_numwords-1)[bs->bs_data] &= p_last_word_mask (bs->bs_numbits);
}

/* Сбросить бит.  */
void 
p_bitset_reset (struct bitset_s *bs, size_t pos) P_NOEXCEPT 
{
        p_bitset_set (bs, pos, 0);
}

/* Сбросить все биты.  */
void 
p_bitset_reset_all (struct bitset_s *bs) P_NOEXCEPT 
{
        memset (bs->bs_data, 0, bs->bs_numwords * 
                sizeof (unsigned long long int));
}

/* Побитовое И (dst &= src).  */
void 
p_bitset_and (struct bitset_s *dst, const struct bitset_s *src) P_NOEXCEPT 
{
        size_t i;

        assert (dst->bs_numbits == src->bs_numbits);
        for (i = 0; i < dst->bs_numwords; i++)
                i[dst->bs_data] &= i[src->bs_data];
}

/* Побитовое ИЛИ (dst |= src).  */
void 
p_bitset_or (struct bitset_s *dst, const struct bitset_s *src) P_NOEXCEPT 
{
        size_t i;

        assert (dst->bs_numbits == src->bs_numbits);
        for (i = 0; i < dst->bs_numwords; i++)
                i[dst->bs_data] |= i[src->bs_data];
}

/* Побитовое исключение ИЛИ (dst ^= src).  */
void 
p_bitset_xor (struct bitset_s *dst, const struct bitset_s *src) P_NOEXCEPT 
{
        size_t i;

        assert (dst->bs_numbits == src->bs_numbits);
        for (i = 0; i < dst->bs_numwords; i++)
                i[dst->bs_data] ^= i[src->bs_data];
}

/* Побитовое НЕ (bs = ~bs).  */
void 
p_bitset_not (struct bitset_s *bs) P_NOEXCEPT 
{
        p_bitset_flip_all (bs);
}

/* Сдвиг влево.  */
void 
p_bitset_shl (struct bitset_s *bs, size_t shift) P_NOEXCEPT 
{
        if (shift == 0) return;
        if (shift >= bs->bs_numbits) {
                p_bitset_reset_all (bs);
                return;
        }

        size_t word_shift = shift / P_BITSET_WORDBITS;
        size_t bit_shift = shift % P_BITSET_WORDBITS;
        size_t i;

        if (bit_shift == 0) {
                /* просто сдвигаем слова  */
                memmove (word_shift[&bs->bs_data], bs->bs_data,
                         (bs->bs_numwords - word_shift) * 
                         sizeof (unsigned long long int));
                memset (bs->bs_data, 0, word_shift *
                        sizeof (unsigned long long int));
        } else {
                /* сдвигаем с переносом битов между словами  */
                for (i = bs->bs_numwords - 1; i > word_shift; i--)
                        i[bs->bs_data] = ((i-word_shift)[bs->bs_data]) | 
                                         ((i-word_shift-1)[bs->bs_data] >>
                                          (P_BITSET_WORDBITS - bit_shift));
                word_shift[bs->bs_data] = 0[bs->bs_data] << bit_shift;
                memset (bs->bs_data, 0, word_shift *
                        sizeof (unsigned long long int));
        }

        /* корректируем последнее слово  */
        if (bs->bs_numbits % P_BITSET_WORDBITS != 0)
                (bs->bs_numwords-1)[bs->bs_data] &= p_last_word_mask (bs->bs_numbits);
}

/* Сдвиг вправо.  */
void 
p_bitset_shr (struct bitset_s *bs, size_t shift) P_NOEXCEPT 
{
        if (shift == 0) return;
        if (shift >= bs->bs_numbits) {
                p_bitset_reset_all (bs);
                return;
        }

        size_t word_shift = shift / P_BITSET_WORDBITS;
        size_t bit_shift = shift % P_BITSET_WORDBITS;
        size_t last_word = bs->bs_numwords - word_shift - 1;
        size_t i;

        if (bit_shift == 0) {
                /* просто сдвигаем слова  */
                memmove (bs->bs_data, word_shift[&bs->bs_data], 
                         (bs->bs_numwords - word_shift) * 
                         sizeof (unsigned long long int));
                memset ((bs->bs_numwords - word_shift)[&bs->bs_data], 0, 
                        word_shift * sizeof (unsigned long long int));
        } else {
                /* сдвигаем с переносом битов между словами  */
                for (i = 0; i < last_word; i++)
                        i[bs->bs_data] = ((i+word_shift)[bs->bs_data]) | 
                                         ((i+word_shift+1)[bs->bs_data] <<
                                          (P_BITSET_WORDBITS - bit_shift));
                last_word[bs->bs_data] = (bs->bs_numwords - 1)[bs->bs_data] >> 
                                         bit_shift;
                memset ((last_word + 1)[&bs->bs_data], 0, (word_shift + 1) *
                        sizeof (unsigned long long int));
        }
}

/* Подсчет установленных битов (встроенная функция POPCNT для скорости).  */
size_t 
p_bitset_count (const struct bitset_s *bs) P_NOEXCEPT 
{
        size_t count = 0;
        size_t i;

        for (i = 0; i < bs->bs_numwords; i++)
                count += __builtin_popcountll (i[bs->bs_data]);
        return (count);
}

/* Размер в битах.  */
size_t 
p_bitset_size (const struct bitset_s *bs) P_NOEXCEPT 
{
        return (bs->bs_numbits);
}

/* Все биты установлены?  */
unsigned char 
p_bitset_all (const struct bitset_s *bs) P_NOEXCEPT 
{
        unsigned long long int  mask;
        size_t                  full_words = bs->bs_numwords;
        size_t                  i;

        if (bs->bs_numbits % P_BITSET_WORDBITS != 0) {
                full_words--;
                mask = p_last_word_mask (bs->bs_numbits);
                if (full_words[bs->bs_data] != mask) return (0);
        }
        for (i = 0; i < full_words; i++)
                if (i[bs->bs_data] != UINT64_MAX) return (0);

        return (1);
}

/* Хотябы один бит установлен?  */
unsigned char 
p_bitset_any (const struct bitset_s *bs) P_NOEXCEPT 
{
        size_t i;

        for (i = 0; i < bs->bs_numwords; i++)
                if (i[bs->bs_data] != 0) return (1);
        
        return (0);
}

/* Ни один бит не установлен?  */
unsigned char 
p_bitset_none (const struct bitset_s *bs) P_NOEXCEPT 
{
        return (!p_bitset_any (bs));
}

/* Конвертация в unsigned long long int (только для наборов до 64 бит).  */
unsigned long long int 
p_bitset_to_ullong (const struct bitset_s *bs) P_NOEXCEPT 
{
        assert (bs->bs_numbits <= 64);
        return (0[bs->bs_data]);
}

/* Строковое представление (младший бит справа, как в std::bitset).  */
char *
p_bitset_to_string (const struct bitset_s *bs) P_NOEXCEPT 
{
        size_t  i;
        char   *str = (char *)malloc (bs->bs_numbits + 1);
        if (!str) return (NULL);

        for (i = 0; i < bs->bs_numbits; i++)
                (bs->bs_numbits - 1 - i)[str] = p_bitset_test (bs, i) ? '1' : '0';

        bs->bs_numbits[str] = '\0';
        return (str);
}

/* Создание из строки (ожидает строку из '0' и '1').  */
struct bitset_s *
p_bitset_from_string (const char *str) P_NOEXCEPT 
{
        size_t                  i;
        size_t                  len = strlen (str);
        struct bitset_s         *bs = p_bitset_create (len);
        if (!bs) return (NULL);

        for (i = 0; i < len; i++) {
                if (i[str] == '1')
                        p_bitset_set (bs, len - 1 - i, 1);
                else 
                        p_bitset_set (bs, len - 1 - i, 0);
        }
        return (bs);
}

/* Сравнение на равенство.  */
unsigned char 
p_bitset_equal (const struct bitset_s *a, const struct bitset_s *b) P_NOEXCEPT 
{
        if (a->bs_numbits != b->bs_numbits) return (0);
        return (memcmp (a->bs_data, b->bs_data, a->bs_numwords *
                sizeof (unsigned long long int)) == 0);
}

/* Сравнение на неравенство.  */
unsigned char 
p_bitset_not_equal (const struct bitset_s *a, const struct bitset_s *b) P_NOEXCEPT 
{
        return (!p_bitset_equal (a, b));
}

/* Хеш-функция (FNV-1a).  */
unsigned long long int 
p_bitset_hash (const struct bitset_s *bs) P_NOEXCEPT 
{
        unsigned long long int  hash = 14695981039346656037ULL;
        size_t                  i;
        int                     j;

        for (i = 0; i < bs->bs_numwords; i++) {
                unsigned long long int word = i[bs->bs_data];
                for (j = 0; j < 8; j++) {
                        hash ^= (uint8_t)(word & 0xFF);
                        hash *= 1099511628211ULL;
                        word >>= 8;
                }
        }
        return (hash);
}

/* Итерация по установленным битам.  */
void 
p_bitset_foreach_set (const struct bitset_s *bs, p_bitset_foreach_cb cb, 
                      void *user_data) P_NOEXCEPT 
{
        unsigned long long int  word;
        size_t                  i, base;
        int                     tz;

        for (i = 0; i < bs->bs_numwords; i++) {
                word = i[bs->bs_data];
                base = i * P_BITSET_WORDBITS;

                while (word) {
                        tz = __builtin_ctzll (word);
                        if ((base + tz) < bs->bs_numbits)
                                cb (base + tz, user_data);
                        word &= word - 1; /* сброс младшего установленного бита  */
                }
        }
}

