/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : indicators.h
 */

#ifndef __PLC_INDICATORS_H
#define __PLC_INDICATORS_H      1

#include <stddef.h>

#define LEN_ATTR_COLOR  30
#define LEN_COUNTER     256
#define LEN_BLOCK       10

/*
 * Indicator : progress bar
 */

typedef struct ind_progress {
        size_t  pr_y;
        size_t  pr_x;
        size_t  pr_width;
        size_t  pr_min_iterator;
        size_t  pr_max_iterator;
        size_t  pr_count_y;
        size_t  pr_count_x;
        char    pr_attrcolor_block[LEN_ATTR_COLOR];
        char    pr_attrcolor_backing[LEN_ATTR_COLOR];
        char    pr_attrcolor_bracket[LEN_ATTR_COLOR];
        char    pr_attrcolor_count[LEN_ATTR_COLOR];
        char    pr_block[LEN_BLOCK];
        char    pr_backing[LEN_BLOCK];
        char    pr_count[LEN_COUNTER];
        char    pr_bracket_left;
        char    pr_bracket_rigth;
} ind_progress_s;

void p_setind_pr(size_t y,
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
                 ind_progress_s *pr); 

size_t p_getind_pr_min(ind_progress_s *pr);
size_t p_getind_pr_max(ind_progress_s *pr);
void   p_idraw_pr(size_t index, ind_progress_s *pr);

/*
 * Indicator : spinner
 */

typedef struct ind_spinner {
        size_t  sp_y;
        size_t  sp_x;
        size_t  sp_iterator;
        char    sp_attrcolor[LEN_ATTR_COLOR];
        char    sp_spinner[4];
} ind_spinner_s;

void p_setind_sp(size_t y,
                 size_t x,
                 const char *attrcolor,
                 ind_spinner_s *sp); 

void p_idraw_sp(ind_spinner_s *sp);

/*
 * Indicator : counter
 */

typedef struct ind_counter {
        size_t  c_y;
        size_t  c_x;
        char    c_attrcolor[LEN_ATTR_COLOR];
        char    c_counter[LEN_COUNTER];
} ind_counter_s;

void p_setind_c(size_t y,
                size_t x,
                const char *attrcolor,
                const char *counter,
                ind_counter_s *c); 

void p_idraw_c(size_t index, ind_counter_s *c);

/*
 * Indicator : points
 */

typedef struct ind_points {
        size_t        p_y;
        size_t        p_x;
        char          p_attrcolor[LEN_ATTR_COLOR];
        unsigned int  p_ms_delay;
} ind_points_s;

void p_setind_p(size_t y,
                size_t x,
                const char *attrcolor,
                unsigned int ms_delay, 
                ind_points_s *p); 

void p_idraw_p(ind_points_s *p);

/*
 * Indicator : wave
 */

typedef struct ind_wave {
        size_t        w_y;
        size_t        w_x;
        char          w_attrcolor[LEN_ATTR_COLOR];
        const char   *w_wave[11];
        unsigned int  w_ms_delay;
} ind_wave_s;

void p_setind_w(size_t y,
                size_t x,
                const char *attrcolor,
                unsigned int ms_delay, 
                ind_wave_s *w); 

void p_idraw_w(ind_wave_s *w);

#endif  /* __PLC_INDICATORS_H */
