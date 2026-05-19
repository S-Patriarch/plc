/* Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <line.h>
 */

#ifndef __PLC_LINE_H
#define __PLC_LINE_H    1

#include <plc/types/line_s.h>

P_BEGIN_DECLS

#define LEN_ATTR_COLOR  30

extern void p_setline (size_t y,
                       size_t x,
                       size_t length,
                       enum line_direct line_direct,
                       enum line_view line_view,
                       const char *attrcolor,
                       line_s *line) P_NOEXCEPT; 

extern void p_ldraw (const line_s *line) P_NOEXCEPT;

P_END_DECLS

#endif /* line.h  */
