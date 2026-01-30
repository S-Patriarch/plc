/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : string.h
 */

#ifndef __PLC_STRING_H
#define __PLC_STRING_H  1

#include <stddef.h>
#include <plc/plcdef.h>

/* переворот строки  */
int     p_strrev(char *s) P_NOEXCEPT;

/* разделение строки на слова  */
char  **p_strspl(const char *s, size_t *w_count) P_NOEXCEPT;

/* освобождение памяти, выделенной под массив слов  */
void    p_free_words(char **words, size_t w_count) P_NOEXCEPT;

/* подсчет количества вхождений символов в строку  */
size_t  p_count_char(const char *s, char c) P_NOEXCEPT;

/* подсчет количества слов в строке  */
size_t  p_count_words(const char *s) P_NOEXCEPT;

/*подсчет количества символов utf8 в строке  */
size_t  p_count_utf8_chars(const char *s) P_NOEXCEPT;

/* последовательное извлечение цифр из строки  */
size_t *p_extract_digits(const char *s, size_t *d_count) P_NOEXCEPT;

/* Копирование одной строки в другую.
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
int     p_strcpy(char *to, char *from) P_NOEXCEPT; 

/* Аллокатор памяти - копирует байт за байтом.
 * 
 * Данная реализация использует 128-битные регистры процессора (XMM0-XMM15) 
 * для хранения данных и ускорения операции копирования - по 16 байт за раз.
 * _mm_loadu_si128  - загружает 16 байт (без выравнивания)
 * _mm_storeu_si128 - сохраняет 16 байт (без выравнивания)
 * Для оставшихся байт происходит откат к покопийному копированию.
 * 
 * Работает безопасно с невыравненной памятью (loadu/storeu), но с 
 * выравненной памятью будет быстрее.
 */
void   *p_memcpy(void *dest, const void *src, size_t size) P_NOEXCEPT;

typedef struct {
        char   *data;
        size_t  length;
        size_t  capacity;
} string_s;

string_s   *p_string_new(void) P_NOEXCEPT;                                           /* создание пустой строки  */
string_s   *p_string_from_cstr(const char *cstr) P_NOEXCEPT;                         /* создание строки  */
void        p_string_free(string_s *str) P_NOEXCEPT;                                 /* удаление строки  */
void        p_string_append(string_s *str, const char *s) P_NOEXCEPT;                /* добавление строки  */
void        p_string_append_char(string_s *str, char c) P_NOEXCEPT;                  /* добавление символа к строке  */
void        p_string_clear(string_s *str) P_NOEXCEPT;                                /* очистка строки  */
int         p_string_empty(const string_s *str) P_NOEXCEPT;                          /* проверка пустой строки  */
int         p_string_compare(const string_s *str1, const string_s *str2) P_NOEXCEPT; /* сравнение строк  */
size_t      p_string_length(const string_s *str) P_NOEXCEPT;                         /* длина строки  */
const char *p_string_cstr(const string_s *str) P_NOEXCEPT;                           /* предоставление строки  */

/* Объединение массива строк в одну через разделитель.
 * 
 * Возвращает 0 в случае успеха и -1 при ошибке.
 * strings   - указатель на массив строк (массив указателей на char)
 * count     - размер массива строк
 * delimiter - строка-разделитель
 * result    - адрес указателя, куда будет записан результат
 *
 * После отработки данной функции следует освободить результат.
 * Например:
 *              free(result);
 */
int     p_strjoin(char **strings, size_t count, 
                  const char *delimiter, 
                  char **result) P_NOEXCEPT;

#endif  /* __PLC_STRING_H  */
