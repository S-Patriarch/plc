/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : window.c
 */

#include <plc/plcdef.h>
#include <plc/window.h>
#include <plc/conio.h>
#include <plc/string.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

void setwintitle(const char *title,
                 enum win_title_location location,
                 const char *attrcolor,
                 window_s *win) 
{
        if (title == NULL || *title == '\0')
                strncpy(win->wtl.wtl_title, "", LEN_TITLE-1);
                
        else
                strncpy(win->wtl.wtl_title, title, LEN_TITLE-1);

        win->wtl.wtl_title[LEN_TITLE-1] = '\0';

        if (attrcolor == NULL || *attrcolor == '\0')
                strncpy(win->wtl.wtl_attrcolor, "\033[39;49m", LEN_ATTR_COLOR-1);
        else 
                strncpy(win->wtl.wtl_attrcolor, attrcolor, LEN_ATTR_COLOR-1);

        win->wtl.wtl_attrcolor[LEN_ATTR_COLOR-1] = '\0';

        win->wtl.wtl_location = location;
}

void setwindow(size_t y,
               size_t x,
               size_t height,
               size_t width,
               enum win_box_view view,
               const char *attrcolor,
               window_s *win) 
{
        win->win_y = y;
        win->win_x = x;
        win->win_height = height;
        win->win_width = width;
        win->wbv = view;

        if (attrcolor == NULL || *attrcolor == '\0')
                strncpy(win->win_attrcolor, "\033[39;49m", LEN_ATTR_COLOR-1);
        else 
                strncpy(win->win_attrcolor, attrcolor, LEN_ATTR_COLOR-1);

        win->win_attrcolor[LEN_ATTR_COLOR-1] = '\0';
}

void wdraw(const window_s *win) 
{
        resattr();
        setattr(win->win_attrcolor);

        for (size_t i = 0; i != win->win_height; i++) {
                for (size_t j = 0; j != win->win_width; j++) {
                        if (i == 0 || i == win->win_height-1) {
                                cursor_move(win->win_y + i, win->win_x + j);
                                if (win->wbv == wb_n)   printf("%s", " ");
                                if (win->wbv == wb_sl)  printf("%s", "\u2500");
                                if (win->wbv == wb_slr) printf("%s", "\u2500");
                                if (win->wbv == wb_sh)  printf("%s", "\u2501");
                                if (win->wbv == wb_dl)  printf("%s", "\u2550");
                        } else if (j == 0 || j == win->win_width-1) {
                                cursor_move(win->win_y + i, win->win_x + j);
                                if (win->wbv == wb_n)   printf("%s", " ");
                                if (win->wbv == wb_sl)  printf("%s", "\u2502");
                                if (win->wbv == wb_slr) printf("%s", "\u2502");
                                if (win->wbv == wb_sh)  printf("%s", "\u2503");
                                if (win->wbv == wb_dl)  printf("%s", "\u2551");
                        } else {
                                cursor_move(win->win_y + i, win->win_x + j);
                                printf("%s", " ");
                        }

                }
        }

        cursor_move(win->win_y, win->win_x);
        if (win->wbv == wb_n)   printf("%s", " ");
        if (win->wbv == wb_sl)  printf("%s", "\u250C");
        if (win->wbv == wb_slr) printf("%s", "\u256D");
        if (win->wbv == wb_sh)  printf("%s", "\u250F");
        if (win->wbv == wb_dl)  printf("%s", "\u2554");

        cursor_move(win->win_y, win->win_x + win->win_width-1);
        if (win->wbv == wb_n)   printf("%s", " ");
        if (win->wbv == wb_sl)  printf("%s", "\u2510");
        if (win->wbv == wb_slr) printf("%s", "\u256E");
        if (win->wbv == wb_sh)  printf("%s", "\u2513");
        if (win->wbv == wb_dl)  printf("%s", "\u2557");

        cursor_move(win->win_y + win->win_height-1, win->win_x + win->win_width-1);
        if (win->wbv == wb_n)   printf("%s", " ");
        if (win->wbv == wb_sl)  printf("%s", "\u2518");
        if (win->wbv == wb_slr) printf("%s", "\u256F");
        if (win->wbv == wb_sh)  printf("%s", "\u251B");
        if (win->wbv == wb_dl)  printf("%s", "\u255D");

        cursor_move(win->win_y + win->win_height-1, win->win_x);
        if (win->wbv == wb_n)   printf("%s", " ");
        if (win->wbv == wb_sl)  printf("%s", "\u2514");
        if (win->wbv == wb_slr) printf("%s", "\u2570");
        if (win->wbv == wb_sh)  printf("%s", "\u2517");
        if (win->wbv == wb_dl)  printf("%s", "\u255A");

        fflush(stdout);
        resattr();

        if (win->wtl.wtl_title[0] != '\0') {
                setattr(win->wtl.wtl_attrcolor);

                size_t wtl_size = count_utf8_chars(win->wtl.wtl_title);
                
                if (win->wtl.wtl_location == wt_l)
                        cursor_move(win->win_y, 
                                    win->win_x + 3);
                if (win->wtl.wtl_location == wt_r)
                        cursor_move(win->win_y, 
                                    (win->win_x+win->win_width-1) - 2 - wtl_size);
                if (win->wtl.wtl_location == wt_c)
                        cursor_move(win->win_y, 
                                    win->win_x + win->win_width/2 - wtl_size/2 - 1);
                
                printf("%s", win->wtl.wtl_title);
                
                fflush(stdout);
                resattr();
        }
}

int wout(const char *s,
         const char *attrcolor,
         size_t from_tb,
         const window_s *win)
{
        if (s == NULL || *s == '\0') return ERROR;

        size_t count = 0; // счетчик символов в текущей строке
        size_t y = win->win_y + 1 + from_tb;
        size_t x = win->win_x + 1;
        size_t row = win->win_y + win->win_height - 1;
        size_t col = win->win_width - 2;

        resattr();
        setattr(attrcolor);

        cursor_move(y, x);

        while (*s != '\0') {
                int char_len = 1;
                if ((*s & 0xE0) == 0xC0) char_len = 2;
                else if ((*s & 0xF0) == 0xE0) char_len = 3;
                else if ((*s & 0xF8) == 0xF0) char_len = 4;

                for (int i = 0; i < char_len && *s != '\0'; ++i)
                        putchar(*s++);

                fflush(stdout);
                ++count;

                if (count == col && y != row) {
                        count = 0;
                        cursor_move(++y, x);
                }
                if (y == row) break;
        }

        fflush(stdout);
        resattr();

        return SUCCESS;
}
