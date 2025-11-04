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

#define P_BUFSIZ_1K     1024
#define P_BUFSIZ_2K     2048
#define P_BUFSIZ_4K     4096
#define P_BUFSIZ_8K     8192

/*
 * Права доступа по умолчанию к создаваемым файлам.
 */
#define P_FILE_MODE     (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
 
/*
 * Права доступа по умолчанию к создаваемым каталогам.
 */
#define P_DIR_MODE      (P_FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

#define p_min(a, b)     ((a) < (b) ? (a) : (b))
#define p_max(a, b)     ((a) > (b) ? (a) : (b))

/*
 * Макрос для объявления массива переменной длины (VLA) 
 * на стеке и обнуления всех его элементов.
 */
#define P_DECLARE_AND_ZERO_ARRAY(type, name, size) \
        type name[size];                           \
        memset(name, 0, (size) * sizeof(type))

/*
 * Макрос для объявления массива переменной длины (VLA) 
 * в динамической памяти.
 * В этом случае не забывайте освобождать память с
 * помощью free().
 */
#define P_DECLARE_AND_ZERO_DYNAMIC_ARRAY(type, name, size) \
        type *name = (type *)calloc(size, sizeof(type))

#endif  /* __PLC_PLCDEF_H */
