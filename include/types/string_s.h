/* Define string_s.
   Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

#ifndef __plc_string_s_defined
#define __plc_string_s_defined  1

#include <stddef.h>
#include <plc/plcdef.h>

P_BEGIN_DECLS

typedef struct {
        char    *data;
        size_t  length;
        size_t  capacity;
} string_s;

P_END_DECLS

#endif /* string_s.h  */
