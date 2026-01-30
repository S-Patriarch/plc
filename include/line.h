/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : line.h
 */

#ifndef __PLC_LINE_H
#define __PLC_LINE_H    1

#include <stddef.h>
#include <plc/plcdef.h>

#define LEN_ATTR_COLOR  30

enum line_direct {
        line_h,                                 /* горизонтальная линия  */
        line_v                                  /* вертикальная линия  */
};

enum line_view {
        line_sl,                                /* одинарная тонкая линия  */
        line_sh,                                /* одинарная толстая линия  */
        line_dl                                 /* двойная тонкая линия  */
};

typedef struct line {
        size_t  line_y;                         /* координата y начала линии  */
        size_t  line_x;                         /* координата x начала линии  */
        size_t  line_length;                    /* длина линии  */
        enum line_direct line_direct;           /* направление линии  */
        enum line_view line_view;               /* вид линии  */
        char    line_attrcolor[LEN_ATTR_COLOR]; /* атрибуты цвета и фона линии  */
} line_s;

void    p_setline(size_t y,
                  size_t x,
                  size_t length,
                  enum line_direct line_direct,
                  enum line_view line_view,
                  const char *attrcolor,
                  line_s *line) P_NOEXCEPT; 

void    p_ldraw(const line_s *line) P_NOEXCEPT;

#endif  /* __PLC_LINE_H  */
