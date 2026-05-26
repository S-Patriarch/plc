/* Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <window.h>
 */

#ifndef __PLC_WINDOW_H
#define __PLC_WINDOW_H  1

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <plc/types/window_s.h>
#include <plc/plcdef.h>
#include <plc/conio.h>
#include <plc/string.h>

P_BEGIN_DECLS

#define LEN_ATTR_COLOR  30
#define LEN_TITLE       256
#define LEN_TEXT        4096

extern void p_setwintitle (const char *title,
                           enum win_title_location location,
                           const char *attrcolor,
                           window_s *win) P_NOEXCEPT; 
        
extern void p_setwindow (size_t y,
                         size_t x,
                         size_t height,
                         size_t width,
                         enum win_box_view view,
                         const char *attrcolor,
                         window_s *win) P_NOEXCEPT; 

extern void p_wdraw (const window_s *win) P_NOEXCEPT;

extern int p_wout (const char *s,
                   const char *attrcolor,
                   size_t from_tb,
                   const window_s *win) P_NOEXCEPT;

P_END_DECLS

#endif /* window.h  */
