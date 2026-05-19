/* Define ind_progress_s.
   Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

#ifndef __plc_ind_progress_s_defined
#define __plc_ind_progress_s_defined    1

#include <stddef.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

typedef struct ind_progress {
        size_t  pr_y;
        size_t  pr_x;
        size_t  pr_width;
        size_t  pr_min_iterator;
        size_t  pr_max_iterator;
        size_t  pr_count_y;
        size_t  pr_count_x;
        char    pr_attrcolor_block[30];
        char    pr_attrcolor_backing[30];
        char    pr_attrcolor_bracket[30];
        char    pr_attrcolor_count[30];
        char    pr_block[10];
        char    pr_backing[10];
        char    pr_count[256];
        char    pr_bracket_left;
        char    pr_bracket_rigth;
} ind_progress_s;

P_END_DECLS

#endif /* ind_progress_s.h  */
