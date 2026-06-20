/* Define menu_s.
   Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

#ifndef __plc_menu_s_defined
#define __plc_menu_s_defined    1

#include <stddef.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

typedef struct {
        char m_attrcolor_n[30]; /* цвет не активных пунктов меню  */
        char m_attrcolor_s[30]; /* цвет активного пункта меню  */
} menu_style_s;

typedef struct {
        char *m_name;           /* пункт меню  */
        void (*function)();     /* указатель на функцию выбора пункта меню  */
        size_t m_y;
        size_t m_x;
} menu_s;

P_END_DECLS

#endif /* menu_s.h  */
