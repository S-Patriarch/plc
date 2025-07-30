/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : string.h
 */

#ifndef __PLC_STRING_H
#define __PLC_STRING_H  1

#include <stddef.h>
#include <string.h>
#include <emmintrin.h>

/* переворот строки */
int p_strrev(char *s);

/* разделение строки на слова */
char **p_strspl(const char *s, size_t *w_count);

/* освобождение памяти, выделенной под массив слов */
void p_free_words(char **words, size_t w_count);

/* подсчет количества вхождений символов в строку */
size_t p_count_char(const char *s, char c);

/* подсчет количества слов в строке */
size_t p_count_words(const char *s);

/*подсчет количества символов utf8 в строке */
size_t p_count_utf8_chars(const char *s);

/* последовательное извлечение цифр из строки */
size_t *p_extract_digits(const char *s, size_t *d_count);

/*
   Копирование одной строки в другую.
  
   Устройство Даффа - безумный, но рабочий С-код.
   Данный код является легендарным примером Duff's Device - странной, но
   реально работающей техники, в которой switch переходит внутрь do-while,
   чтобы ускорить копирование памяти:
   - count % 8 определяет, сколько байт "добавить",
   - цикл копирует по 8 байт за раз,
   и все это работает, хотя выглядит как баг.
 
   Это не шутка, а валидный ANSI C.
 */
int p_strcpy(char *to, char *from, int count); 

/*
   Аллокатор памяти - копирует байт за байтом.
  
   Данная реализация использует 128-битные регистры процессора (XMM0-XMM15) для
   хранения данных и ускорения операции копирования - по 16 байт за раз.
   _mm_loadu_si128  - загружает 16 байт (без выравнивания)
   _mm_storeu_si128 - сохраняет 16 байт (без выравнивания)
   Для оставшихся байт происходит откат к покопийному копированию.
  
   Работает безопасно с невыравненной памятью (loadu/storeu), но с 
   выравненной памятью будет быстрее.
 */
void *p_memcpy(void *dest, const void *src, size_t size);

#endif  /* __PLC_STRING_H */
