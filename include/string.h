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

/* переворот строки */
int strrev(char *);

/* разделение строки на слова */
char **strspl(const char *, size_t *);

/* освобождение памяти, выделенной под массив слов */
void free_words(char **, size_t);

/* подсчет количества вхождений символов в строку */
size_t count_char(const char *, char);

/* подсчет количества слов в строке */
size_t count_words(const char *);

/*подсчет количества символов utf8 в строке */
size_t count_utf8_chars(const char *);

/* последовательное извлечение цифр из строки */
size_t *extract_digits(const char *, size_t *);

/*
 * Копирование одной строки в другую.
 *
 * Устройство Даффа - безумный, но рабочий С-код.
 * Данный код является легендарным примером Duff's Device - странной, но
 * реально работающей техники, в которой switch переходит внутрь do-while,
 * чтобы ускорить копирование памяти:
 * - count % 8 определяет, сколько байт "добавить",
 * - цикл копирует по 8 байт за раз,
 * и все это работает, хотя выглядит как баг.
 *
 * Это не шутка, а валидный ANSI C.
 */
int strcp(char *, char *, int); 

#endif  /* __PLC_STRING_H */
