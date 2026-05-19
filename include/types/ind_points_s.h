/* Define ind_points_s.
   Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

#ifndef __plc_ind_points_s_defined
#define __plc_ind_points_s_defined      1

#include <stddef.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

typedef struct {
        size_t        p_y;
        size_t        p_x;
        char          p_attrcolor[30];
        unsigned int  p_ms_delay;
} ind_points_s;

P_END_DECLS

#endif /* ind_points_s.h  */
