/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : window.h
 */

#ifndef __PLC_WINDOW_H
#define __PLC_WINDOW_H  1

#include <stddef.h>

#define LEN_ATTR_COLOR  30
#define LEN_TITLE       256
#define LEN_TEXT        4096

enum win_box_view {
        wb_n,           /* пустая */
        wb_sl,          /* одинарная тонкая */
        wb_slr,         /* одинарная тонкая с круглыми углами */
        wb_sh,          /* одинарная толстая */
        wb_dl,          /* двойная тонкая */
        wb_dm,          /* из сериала "Dark matter" */
        wb_dml          /* из сериала "Dark matter" с тонкими горизонталями */
};

enum win_title_location {
        wt_l,           /* расположение с лева */
        wt_c,           /* расположение по центру */
        wt_r            /* расположение с права */
};

struct win_title {
        char wtl_title[LEN_TITLE];
        char wtl_attrcolor[LEN_ATTR_COLOR];
        enum win_title_location wtl_location;
};

typedef struct window {
        size_t win_y;
        size_t win_x;
        size_t win_height;
        size_t win_width;
        enum   win_box_view wbv;
        char   win_attrcolor[LEN_ATTR_COLOR];
        struct win_title wtl;
} window_s;

void setwintitle(const char *, 
                 enum win_title_location,
                 const char *, 
                 window_s *);

void setwindow(size_t, size_t,
               size_t, size_t,
               enum win_box_view,
               const char *,
               window_s *);

void wdraw(const window_s *);
int wout(const char *, const char *, size_t, const window_s *);

#endif  /* __PLC_WINDOW_H */
