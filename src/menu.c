/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : menu.c
 */

#include <plc/plcdef.h>
#include <plc/menu.h>
#include <plc/conio.h>
#include <string.h>
#include <stdio.h>

void p_setmenu(const char *attrcolor_n,
               const char *attrcolor_s,
               menu_style_s *mst)
{
        if (attrcolor_n == NULL || *attrcolor_n == '\0')
                strncpy(mst->m_attrcolor_n, "\033[39;49m", LEN_ATTR_COLOR-1);
        else    strncpy(mst->m_attrcolor_n, attrcolor_n, LEN_ATTR_COLOR-1);
        mst->m_attrcolor_n[LEN_ATTR_COLOR-1] = '\0';

        if (attrcolor_s == NULL || *attrcolor_s == '\0')
                strncpy(mst->m_attrcolor_s, "\033[7m", LEN_ATTR_COLOR-1);
        else    strncpy(mst->m_attrcolor_s, attrcolor_s, LEN_ATTR_COLOR-1);
        mst->m_attrcolor_s[LEN_ATTR_COLOR-1] = '\0';
}

void p_mdraw(const menu_style_s *mst,
             const menu_s *m,
             size_t menu_size,
             size_t selected)
{
        for (size_t i = 0; i != menu_size; ++i) {
                if (i == selected) {
                        p_resattr();
                        p_setattr(mst->m_attrcolor_s);
                        p_cursor_move(m[i].m_y, m[i].m_x);
                        printf("%s", m[i].m_name);
                } else {
                        p_resattr();
                        p_setattr(mst->m_attrcolor_n);
                        p_cursor_move(m[i].m_y, m[i].m_x);
                        printf("%s", m[i].m_name);
                }
        }

        fflush(stdout);

        char c;
        for (;;) {
                p_getchar(&c);

                if (c == P_ESC) {
                        char key[3];

                        p_getchar(&c);
                        key[0] = c;
                        p_getchar(&c);
                        key[1] = c;

                        // cтрелка вверх - влево
                        if (key[0] == '[' && (key[1] == 'A' || key[1] == 'D'))
                                if (selected > 0) {
                                        --selected;
                                        p_resattr();
                                        p_setattr(mst->m_attrcolor_n);
                                        p_cursor_move(m[selected+1].m_y, m[selected+1].m_x);
                                        printf("%s", m[selected+1].m_name);
                                        p_resattr();
                                        p_setattr(mst->m_attrcolor_s);
                                        p_cursor_move(m[selected].m_y, m[selected].m_x);
                                        printf("%s", m[selected].m_name);
                                        fflush(stdout);
                                }
                        // cтрелка вниз - вправо
                        if (key[0] == '[' && (key[1] == 'B' || key[1] == 'C'))
                                if (selected < menu_size-1) {
                                        ++selected;
                                        p_resattr();
                                        p_setattr(mst->m_attrcolor_n);
                                        p_cursor_move(m[selected-1].m_y, m[selected-1].m_x);
                                        printf("%s", m[selected-1].m_name);
                                        p_resattr();
                                        p_setattr(mst->m_attrcolor_s);
                                        p_cursor_move(m[selected].m_y, m[selected].m_x);
                                        printf("%s", m[selected].m_name);
                                        fflush(stdout);
                                }
                }
                else if (c == '\n')
                        m[selected].function();
        }

        p_resattr();
}

