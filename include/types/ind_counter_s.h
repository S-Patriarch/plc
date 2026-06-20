/* Define ind_counter_s.
   Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

#ifndef __plc_ind_counter_s_defined
#define __plc_ind_counter_s_defined     1

#include <stddef.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

typedef struct {
        size_t  c_y;
        size_t  c_x;
        char    c_attrcolor[30];
        char    c_counter[256];
} ind_counter_s;

P_END_DECLS

#endif /* ind_counter_s.h  */
