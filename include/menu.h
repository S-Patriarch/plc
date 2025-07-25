/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : menu.h
 */

#ifndef __PLC_MENU_H
#define __PLC_MENU_H    1

#include <stddef.h>

#define LEN_ATTR_COLOR  30

typedef struct menu_style {
        char  m_attrcolor_n[LEN_ATTR_COLOR];  /* цвет не активных пунктов меню */
        char  m_attrcolor_s[LEN_ATTR_COLOR];  /* цвет активного пункта меню */
} menu_style_s;

typedef struct menu {
        char   *m_name;          /* пункт меню */
        void    (*function)();   /* указатель на функцию выбора пункта меню */
        size_t  m_y;
        size_t  m_x;
} menu_s;

void p_setmenu(const char *attrcolor_n,
               const char *attrcolor_s,
               menu_style_s *mst);

void p_mdraw(const menu_style_s *mst,
             const menu_s *m,
             size_t menu_size,
             size_t selected);

#endif  /* __PLC_MENU_H */
