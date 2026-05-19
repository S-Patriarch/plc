/* Define ind_wave_s.
   Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

#ifndef __plc_ind_wave_s_defined
#define __plc_ind_wave_s_defined        1

#include <stddef.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

typedef struct {
        size_t        w_y;
        size_t        w_x;
        char          w_attrcolor[30];
        const char   *w_wave[11];
        unsigned int  w_ms_delay;
} ind_wave_s;

P_END_DECLS

#endif /* ind_wave_s.h  */
