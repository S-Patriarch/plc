/* Define window_s.
   Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

#ifndef __plc_window_s_defined
#define __plc_window_s_defined  1

#include <stddef.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

enum win_box_view {
        wb_n,           /* пустая  */
        wb_sl,          /* одинарная тонкая  */
        wb_slr,         /* одинарная тонкая с круглыми углами  */
        wb_sh,          /* одинарная толстая  */
        wb_dl,          /* двойная тонкая  */
        wb_dm,          /* из сериала "Dark matter"  */
        wb_dml          /* из сериала "Dark matter" с тонкими горизонталями  */
};

enum win_title_location {
        wt_l,           /* расположение с лева  */
        wt_c,           /* расположение по центру  */
        wt_r            /* расположение с права  */
};

struct win_title {
        char  wtl_title[256];
        char  wtl_attrcolor[30];
        enum win_title_location wtl_location;
};

typedef struct {
        size_t  win_y;
        size_t  win_x;
        size_t  win_height;
        size_t  win_width;
        enum    win_box_view wbv;
        char    win_attrcolor[30];
        struct win_title wtl;
} window_s;

P_END_DECLS

#endif /* window_s.h  */
