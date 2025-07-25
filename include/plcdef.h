/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : plcdef.h
 */

#ifndef __PLC_PLCDEF_H
#define __PLC_PLCDEF_H  1

#include <string.h>
#include <stdlib.h>

#define P_SUCCESS       0
#define P_FAILURE       1
#define P_ERROR         -1

#define P_FALSE         0
#define P_TRUE          1

#define P_ESC           '\033' 

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
