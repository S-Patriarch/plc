/* Define line_s.
   Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

#ifndef __plc_line_s_defined
#define __plc_line_s_defined    1

#include <stddef.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

enum line_direct {
        line_h,                       /* горизонтальная линия  */
        line_v                        /* вертикальная линия  */
};

enum line_view {
        line_sl,                      /* одинарная тонкая линия  */
        line_sh,                      /* одинарная толстая линия  */
        line_dl                       /* двойная тонкая линия  */
};

typedef struct {
        size_t line_y;                /* координата y начала линии  */
        size_t line_x;                /* координата x начала линии  */
        size_t line_length;           /* длина линии  */
        enum line_direct line_direct; /* направление линии  */
        enum line_view line_view;     /* вид линии  */
        char line_attrcolor[30];      /* атрибуты цвета и фона линии  */
} line_s;

P_END_DECLS

#endif /* line_s.h  */
