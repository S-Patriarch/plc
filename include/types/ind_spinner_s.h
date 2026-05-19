/* Define ind_spinner_s.
   Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

#ifndef __plc_ind_spinner_s_defined
#define __plc_ind_spinner_s_defined     1

#include <stddef.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

typedef struct {
        size_t  sp_y;
        size_t  sp_x;
        size_t  sp_iterator;
        char    sp_attrcolor[30];
        char    sp_spinner[4];
} ind_spinner_s;

P_END_DECLS

#endif /* ind_spinner_s.h  */
