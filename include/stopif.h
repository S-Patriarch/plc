/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : stopif.h
 */

#ifndef __PLC_STOPIF_H
#define __PLC_STOPIF_H  1

#include <stdio.h>
#include <stdlib.h>

/* Присвоить этой переменной значение 's', если нужно, чтобы программа
   останавливалась после ошибки.
   В противном случае программа будет возвращать код ошибки. */
char error_mode;

/* Куда писать сообщение об ошибке? Если NULL, писать в stderr. */
FILE *error_log;

#define STOPIF(assertion, error_action, ...) {                                \
                if (assertion) {                                              \
                        fprintf(error_log ? error_log : stderr, __VA_ARGS__); \
                        fprintf(error_log ? error_log : stderr, "\n");        \
                        if (error_mode == 's') abort();                       \
                        else                   {error_action;}                \
                }                                                             \
        }

#endif  /* __PLC_STOPIF_H */
