/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : line.c
 */

#include <plc/line.h>
#include <plc/conio.h>
#include <string.h>
#include <stdio.h>

void p_setline(size_t y,
               size_t x,
               size_t length,
               enum line_direct line_direct,
               enum line_view line_view,
               const char *attrcolor,
               line_s *line) 
{
        line->line_y = y;
        line->line_x = x;
        line->line_length = length;
        line->line_direct = line_direct;
        line->line_view = line_view;

        if (attrcolor == NULL || *attrcolor == '\0') 
                strncpy(line->line_attrcolor, "\033[39;49m", LEN_ATTR_COLOR-1);
        else 
                strncpy(line->line_attrcolor, attrcolor, LEN_ATTR_COLOR-1);
        line->line_attrcolor[LEN_ATTR_COLOR-1] = '\0';
}

void p_ldraw(const line_s *line) 
{
        p_resattr();
        p_setattr(line->line_attrcolor);

        if (line->line_direct == line_h) {
                for (size_t i = 0; i != line->line_length; i++) {
                        p_cursor_move(line->line_y, line->line_x + i);
                        if (line->line_view == line_sl)
                                printf("%s", "\u2500");
                        if (line->line_view == line_sh)
                                printf("%s", "\u2501");
                        if (line->line_view == line_dl)
                                printf("%s", "\u2550");
                }
        }

        if (line->line_direct == line_v) {
                for (size_t i = 0; i != line->line_length; i++) {
                        p_cursor_move(line->line_y + i, line->line_x);
                        if (line->line_view == line_sl)
                                printf("%s", "\u2502");
                        if (line->line_view == line_sh)
                                printf("%s", "\u2503");
                        if (line->line_view == line_dl)
                                printf("%s", "\u2551");
                }
        }

        fflush(stdout);
        p_resattr();
}

