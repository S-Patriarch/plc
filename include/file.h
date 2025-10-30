/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : file.h
 */

#ifndef __PLC_FILE_H
#define __PLC_FILE_H    1

int     p_file_copy(const char *f_src, const char *f_dst);
int     p_file_move(const char *f_src, const char *f_dst);

#endif  /* __PLC_FILE_H */
