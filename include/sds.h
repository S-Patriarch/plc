/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <sds.h>
 */

#ifndef __PLC_SDS_H
#define __PLC_SDS_H     1

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <plc/plcdef.h>

/* SDS (Simple Dynamic Strings - простые динамические строки).
   Данная библиотека является попыткой повторить библиотеку antirez (создателя
   Redis), которому работа со строками С доставляла ряд проблем, а именно:
   - strlen работает за O(n), поскольку выполняет проход по памяти до нулевого
   терминатора '\0';
   - strcat требует ручного управления емкостью буфера и часто дополнительно
   сканирует строку внутри своей реализации;
   - строки в стиле С не позволяют безопасно хранить произвольные бинарные
   данные, поскольку нулевой байт '\0' считается концом строки.

   SDS решает все эти проблемы, сохраняя метаданные в заголовке, который
   располагается непосредственно перед буфером строки. Такая компоновка
   позволяет:
   - получить доступ к метаданным простым смещением указателя;
   - передавать sds в любые функции, ожидающие const char *.  */

P_BEGIN_DECLS

struct sds_s {
        size_t  len;    /* текущая длина строки (не считая '\0')  */
        size_t  free;   /* количество байт, доступных для добавления 
                           без realloc  */
        char    buf[];  /* гибкий массив - здесь лежат сами символы строки  */
};

/*
 * Получение информации о строке.
 * Эти функции выполняются за O(1), просто читая заголовок.
 */

/* Текущая длина строки в байтах (без учета '\0').  */
static inline size_t p_sds_len (const char *s) P_NOEXCEPT 
{
        struct sds_s *hdr = (struct sds_s *)(s - sizeof(struct sds_s));
        return(hdr->len);
}

/* Количество свободнах байт после конца строки.
   Можно добавить столько байт без нового выделения памяти.  */
static inline size_t p_sds_avail (const char *s) P_NOEXCEPT 
{
        struct sds_s *hdr = (struct sds_s *)(s - sizeof(struct sds_s));
        return(hdr->free);
}

/* Полный размер выделенной памяти под строку.
   Включает заголовок, данные и свободное место.  */
static inline size_t p_sds_allocsize (const char *s) P_NOEXCEPT 
{
        struct sds_s *hdr = (struct sds_s*)(s - sizeof(struct sds_s));
        return(sizeof(struct sds_s) + hdr->len + hdr->free + 1);
}

/*
 * Резервирование и оптимизация памяти.
 */

/* Гарантировать, что в строке есть как минимум addlen свободного места.
   Если места уже достаточно - ничего не делает.
   Полезно перед серией маленьких добавлений, чтобы избежать
   многократных перевыделений.  */
extern char *p_sds_reserve (char *s, size_t addlen) P_NOEXCEPT;

/* Убрать все свободное место, уменьшив буфер до точного размера строки.
   Экономит память, когда строка больше не будет меняться.  */
extern char *p_sds_removefreespace (char *s) P_NOEXCEPT;

/*
 * Создание новых строк.
 * Все функции возвращают char * или NULL при ошибке выделения памяти.
 * После вызова строку можно использовать с printf, puts и т.д.
 */

/* Создать новую sds-строку из обычной С-строки (с завершающим '\0').
   Если s == NULL, будет создана пустая строка "".  */
extern char *p_sds_new (const char *s) P_NOEXCEPT;

/* Создание sds-строки из буфера произвольной длины.
   В отличии от p_sds_new, здесь допустимы нулевые байты внутри данных.  */
extern char *p_sds_newlen (const void *s, size_t slen) P_NOEXCEPT; 

/* Создать пустую sds-строку (эквивалент p_sds_new("")).  */
extern char *p_sds_empty (void) P_NOEXCEPT;

/* Создать точную копию существующей sds-строки.  */
extern char *p_sds_dup (const char *s) P_NOEXCEPT;

/*
 * Уничтожение.
 */

/* Освободить память, занятую sds-строкой.
   Безопасно передавать NULL (ничего не делает)  */
extern void p_sds_free (char *s) P_NOEXCEPT;

/*
 * Модификация строки.
 * Все функции модификации возвращают ту же (возможно перераспределенную)
 * строку, поэтому результат нужно присваивать обратно:
 *      str = p_sds_cat(str, "patriarch");
 * Это необходимо, потому что при расширении realloc может переместить
 * данные в другую область памяти.
 */

/* Дописать С-строку в конец sds-строки.
   Автоматически расширит буфер при необходимости.  */
extern char *p_sds_cat (char *s, const char *t) P_NOEXCEPT;

/* Дописать произвольный буфер заданной длины.
   Полезно для бинарных данных, где могут быть нулевые байты.  */
extern char *p_sds_catlen (char *s, const void *t, size_t len) P_NOEXCEPT;

/* Дописать другую sds-строку.  */
extern char *p_sds_catsds (char *s, const char *t) P_NOEXCEPT;

/* Полностью заменить содержимое строки на новую С-строку.
   Расширит буфер, если новая строка длиннее выделенного места.  */
extern char *p_sds_cpy (char *s, const char*t) P_NOEXCEPT;

/* Заменить содержимое на буфер произвольной длины.  */
extern char *p_sds_cpylen (char *s, const void *t, size_t len) P_NOEXCEPT;

/*
 * Форматированный вывод.
 */

/* Дописать отформатированную строку (printf-подобный сиснтаксис).
   Пример: str = p_sds_catprintf(str, "Count: %d, Name: %s", 42, "foo");  */
extern char *p_sds_catprintf (char *s, const char *fmt, ...) P_NOEXCEPT;

/* То же, но принимает готовый va_list (для оберток с variadic аргументами).  */
extern char *p_sds_catvprintf (char *s, const char *fmt, va_list ap) P_NOEXCEPT;

/* Облегченный формат.  */
extern char *p_sds_catfmt (char *s, const char *fmt, ...) P_NOEXCEPT;

/*
 * Обрезка и извлечение подстрок.
 */

/* Удалить с обоих концов строки все символы, входящие в набор cset.
   Пример: p_sds_trim(str, " \t\n\r") - обрезает пробельные символы.  */
extern void p_sds_trim (char *s, const char *cset) P_NOEXCEPT;

/* Выделить подстроку, модифицируя строку на месте.
   Поддерживает отрицательные индексы:
        p_sds_range(str, 0, 4)   - первые 5 символов
        p_sds_range(str, -3, -1) - три последних символа  */
extern void p_sds_range (char *s, ptrdiff_t start, ptrdiff_t end) P_NOEXCEPT;

/* Разбить строку по разделителям на массив sds-строк.
   В переменную count записывается количество получившихся частей.
   Возвращает массив, который нужно освободить.
   Если разделитель не найден - возвращает массив из одного элемента.  */
extern char **p_sds_split (const char *s, const char *sep, int *count) P_NOEXCEPT;

/* Освободить массив sds-строк, полученных в результате p_sds_split, а затем и
   сам массив. Безопасно передавать NULL в s.
   Параметры:
        s     - массив указателей на sds-строки
        count - количество элементов в массиве
   Примеры:
        size_t count;
        char **tokens = p_sds_split(str, ",", &count);
        // ... использование tokens ...
        p_sds_free_split(tokens, count); // tokens становится невалидным  */
extern void p_sds_splitfree (char **s, size_t count) P_NOEXCEPT;

/*
 * Преобразование регистра.
 */

/* Приводит все символы строки к нижнему регистру на месте. 
   Модифицирует исходную строку, не создавая новой.  */
extern void p_sds_tolower (char *s) P_NOEXCEPT;

/* Приводит все символы строки к верхнему регистру на месте. 
   Модифицирует исходную строку, не создавая новой.  */
extern void p_sds_toupper (char *s) P_NOEXCEPT;

P_END_DECLS

#endif /* sds.h  */
