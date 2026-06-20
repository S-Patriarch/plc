/* Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <menu.h>
 */

#ifndef __PLC_MENU_H
#define __PLC_MENU_H    1

#include <string.h>
#include <stdio.h>
#include <plc/types/menu_s.h>
#include <plc/plcdef.h>
#include <plc/conio.h>

P_BEGIN_DECLS

#define LEN_ATTR_COLOR  30

extern void p_setmenu (const char *attrcolor_n,
                       const char *attrcolor_s,
                       menu_style_s *mst) P_NOEXCEPT;

extern void p_mdraw (const menu_style_s *mst,
                     const menu_s *m,
                     size_t menu_size,
                     size_t selected) P_NOEXCEPT;

P_END_DECLS

#endif /* menu.h  */
