/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : plcdef.h
 */

#ifndef __PLC_PLCDEF_H
#define __PLC_PLCDEF_H  1

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define P_SUCCESS       0
#define P_FAILURE       1
#define P_ERROR         -1

#define P_FALSE         0
#define P_TRUE          1

#define P_ESC           '\033' 
#define P_VTAB          '\013'
#define P_BELL          '\007'

#define P_BUFSIZ_1      1
#define P_BUFSIZ_2      2
#define P_BUFSIZ_4      4
#define P_BUFSIZ_8      8
#define P_BUFSIZ_16     16
#define P_BUFSIZ_32     32
#define P_BUFSIZ_64     64
#define P_BUFSIZ_128    128
#define P_BUFSIZ_256    256
#define P_BUFSIZ_512    512
#define P_BUFSIZ_1K     1024
#define P_BUFSIZ_2K     2048
#define P_BUFSIZ_4K     4096
#define P_BUFSIZ_8K     8192
#define P_BUFSIZ_16K    16384
#define P_BUFSIZ_32K    32768
#define P_BUFSIZ_65K    65536
#define P_BUFSIZ_131K   131072
#define P_BUFSIZ_262K   262144
#define P_BUFSIZ_524K   524288

/* Права доступа по умолчанию к создаваемым файлам. */
#define P_FILE_MODE     (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
 
/* Права доступа по умолчанию к создаваемым каталогам. */
#define P_DIR_MODE      (P_FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

#define P_MIN(a, b)     ((a) < (b) ? (a) : (b))
#define P_MAX(a, b)     ((a) > (b) ? (a) : (b))

/* Макрос P_SWAP на базе typeof позволяет одной строкой менять
 * местами int, double, struct, указатели и вообще любые типы.
 * Только не следует использовать его с i++ и похожими 
 * выражениями.
 */
#define P_SWAP(a, b)    do {                             \
                                __typeof__(a) tmp = (a); \
                                (a) = (b);               \
                                (b) = tmp;               \
                        } while (0)

/* В стандарте С доступен атрибут cleanup (очистка) через расширение
 * GNU C (__attribute__((cleanup))). Он не является частью стандартов
 * ISO C (C99, C11, c17 и т.д.), но поддерживается компиляторами
 * GCC и Clang.
 * Атрибут cleanup автоматически вызывает указанную функцию очистки
 * при выходе переменной из области видимости (например, при выходе 
 * из блока {}, возврате из функции или при возникновении исключения
 * через setjmp/longjmp в некоторых случаях). 
 */
#define P_AUTO_CLEANUP(func)    __attribute__((cleanup(func)))

/* Макрос для объявления массива переменной длины (VLA) 
 * на стеке и обнуления всех его элементов.
 */
#define P_DECLARE_AND_ZERO_ARRAY(type, name, size) \
        type name[size];                           \
        memset(name, 0, (size) * sizeof(type))

/* Макрос для объявления массива переменной длины (VLA) 
 * в динамической памяти.
 * В этом случае не забывайте освобождать память с
 * помощью free().
 */
#define P_DECLARE_AND_ZERO_DYNAMIC_ARRAY(type, name, size) \
        type *name = (type *)calloc(size, sizeof(type))

#endif  /* __PLC_PLCDEF_H */
