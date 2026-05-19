/* Copyright (C) 2025, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <indicators.h>
 */

#ifndef __PLC_INDICATORS_H
#define __PLC_INDICATORS_H      1

#include <plc/types/ind_progress_s.h>
#include <plc/types/ind_spinner_s.h>
#include <plc/types/ind_counter_s.h>
#include <plc/types/ind_points_s.h>
#include <plc/types/ind_wave_s.h>

P_BEGIN_DECLS

#define LEN_ATTR_COLOR  30
#define LEN_COUNTER     256
#define LEN_BLOCK       10

/*
 * Indicator : progress bar
 */

extern void p_setind_pr (size_t y,
                         size_t x,
                         size_t width,
                         size_t min_iterator,
                         size_t max_iterator,
                         const char *attrcolor_block,
                         const char *attrcolor_backing,
                         const char *attrcolor_bracket,
                         const char *block,
                         const char *backing,
                         char bracket_left,
                         char bracket_rigth,
                         size_t count_y,
                         size_t count_x,
                         const char *attrcolor_count,
                         const char *count,
                         ind_progress_s *pr) P_NOEXCEPT; 

extern size_t p_getind_pr_min (ind_progress_s *pr) P_NOEXCEPT;
extern size_t p_getind_pr_max (ind_progress_s *pr) P_NOEXCEPT;
extern void p_idraw_pr (size_t index, ind_progress_s *pr) P_NOEXCEPT;

/*
 * Indicator : spinner
 */

extern void p_setind_sp (size_t y,
                         size_t x,
                         const char *attrcolor,
                         ind_spinner_s *sp) P_NOEXCEPT; 

extern void p_idraw_sp (ind_spinner_s *sp) P_NOEXCEPT;

/*
 * Indicator : counter
 */

extern void p_setind_c (size_t y,
                        size_t x,
                        const char *attrcolor,
                        const char *counter,
                        ind_counter_s *c) P_NOEXCEPT; 

extern void p_idraw_c (size_t index, ind_counter_s *c) P_NOEXCEPT;

/*
 * Indicator : points
 */

extern void p_setind_p (size_t y,
                        size_t x,
                        const char *attrcolor,
                        unsigned int ms_delay, 
                        ind_points_s *p) P_NOEXCEPT; 

extern void p_idraw_p (ind_points_s *p) P_NOEXCEPT;

/*
 * Indicator : wave
 */

extern void p_setind_w (size_t y,
                        size_t x,
                        const char *attrcolor,
                        unsigned int ms_delay, 
                        ind_wave_s *w) P_NOEXCEPT; 

extern void p_idraw_w (ind_wave_s *w) P_NOEXCEPT;

P_END_DECLS

#endif /* indicators.h  */
