/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : p_conio.h
 */

#ifndef __PLC_P_CONIO_H
#define __PLC_P_CONIO_H   2

#include <stddef.h>

struct text_info {
        size_t cur_row;
        size_t cur_col;
};

struct win_size {
        size_t ws_row;
        size_t ws_col;
};

#endif  /* __PLC_P_CONIO_H */
