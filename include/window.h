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
        char  wtl_title[LEN_TITLE];
        char  wtl_attrcolor[LEN_ATTR_COLOR];
        enum win_title_location wtl_location;
};

typedef struct window {
        size_t  win_y;
        size_t  win_x;
        size_t  win_height;
        size_t  win_width;
        enum    win_box_view wbv;
        char    win_attrcolor[LEN_ATTR_COLOR];
        struct win_title wtl;
} window_s;

void p_setwintitle(const char *title,
                   enum win_title_location location,
                   const char *attrcolor,
                   window_s *win); 

void p_setwindow(size_t y,
                 size_t x,
                 size_t height,
                 size_t width,
                 enum win_box_view view,
                 const char *attrcolor,
                 window_s *win); 

void p_wdraw(const window_s *win);

int p_wout(const char *s,
           const char *attrcolor,
           size_t from_tb,
           const window_s *win);

#endif  /* __PLC_WINDOW_H */
