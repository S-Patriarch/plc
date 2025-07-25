/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : indicators.c
 */

#include <plc/indicators.h>
#include <plc/conio.h>
#include <plc/dt.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

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
                 ind_progress_s *pr) 
{
        pr->pr_y = y;
        pr->pr_x = x;

        pr->pr_width = width;

        pr->pr_min_iterator = min_iterator;
        pr->pr_max_iterator = max_iterator;

        if (attrcolor_block == NULL || *attrcolor_block == '\0') 
                strncpy(pr->pr_attrcolor_block, "\033[39;49m", LEN_ATTR_COLOR-1);
        else 
                strncpy(pr->pr_attrcolor_block, attrcolor_block, LEN_ATTR_COLOR-1);
        pr->pr_attrcolor_block[LEN_ATTR_COLOR-1] = '\0';

        if (attrcolor_backing == NULL) 
                strncpy(pr->pr_attrcolor_backing, "", LEN_ATTR_COLOR-1);
        else 
                strncpy(pr->pr_attrcolor_backing, attrcolor_backing, LEN_ATTR_COLOR-1);
        pr->pr_attrcolor_backing[LEN_ATTR_COLOR-1] = '\0';

        if (attrcolor_bracket == NULL || *attrcolor_bracket == '\0') 
                strncpy(pr->pr_attrcolor_bracket, "\033[39;49m", LEN_ATTR_COLOR-1);
        else 
                strncpy(pr->pr_attrcolor_bracket, attrcolor_bracket, LEN_ATTR_COLOR-1);
        pr->pr_attrcolor_bracket[LEN_ATTR_COLOR-1] = '\0';

        if (block == NULL || *block == '\0') 
                strncpy(pr->pr_block, "\u2584", LEN_BLOCK-1);
        else 
                strncpy(pr->pr_block, block, LEN_BLOCK-1);
        pr->pr_block[LEN_BLOCK-1] = '\0';

        if (backing == NULL) 
                strncpy(pr->pr_backing, "", LEN_BLOCK-1);
        else 
                strncpy(pr->pr_backing, backing, LEN_BLOCK-1);
        pr->pr_backing[LEN_BLOCK-1] = '\0';

        pr->pr_bracket_left = bracket_left;
        pr->pr_bracket_rigth = bracket_rigth;

        pr->pr_count_y = count_y;
        pr->pr_count_x = count_x;

        if (attrcolor_count == NULL || *attrcolor_count == '\0') 
                strncpy(pr->pr_attrcolor_count, "\033[39;49m", LEN_ATTR_COLOR-1);
        else 
                strncpy(pr->pr_attrcolor_count, attrcolor_count, LEN_ATTR_COLOR-1);
        pr->pr_attrcolor_count[LEN_ATTR_COLOR-1] = '\0';

        if (count == NULL) 
                strncpy(pr->pr_count, "", LEN_BLOCK-1);
        else 
                strncpy(pr->pr_count, count, LEN_BLOCK-1);
        pr->pr_count[LEN_BLOCK-1] = '\0';
}

size_t p_getind_pr_min(ind_progress_s *pr) 
{
        return pr->pr_min_iterator;
}

size_t p_getind_pr_max(ind_progress_s *pr) 
{
        return pr->pr_max_iterator;
}

void p_idraw_pr(size_t index, ind_progress_s *pr) 
{
        bool is_yx;

        if (pr->pr_y > 0 && pr->pr_x > 0) 
                is_yx = true;
        else
                is_yx = false;

        // вычисляем процент выполнения
        double progress = (double)index / (double)pr->pr_max_iterator;
        size_t complete = (size_t)(progress * (double)pr->pr_width);

        if (is_yx) 
                p_cursor_move(pr->pr_y, pr->pr_x);
        else
                p_cursor_save_and_attr();

        p_resattr();
        p_setattr(pr->pr_attrcolor_bracket);
        printf("%c", pr->pr_bracket_left);

        for (size_t i = 0; i <= pr->pr_width; ++i) {
                if (i <= complete) {
                        p_resattr();
                        p_setattr(pr->pr_attrcolor_block);
                        printf("%s", pr->pr_block);
                } else {
                        p_resattr();
                        p_setattr(pr->pr_attrcolor_backing);
                        printf("%s", pr->pr_backing);
                }
                fflush(stdout);
        }

        p_resattr();
        p_setattr(pr->pr_attrcolor_bracket);
        printf("%c", pr->pr_bracket_rigth);

        if (pr->pr_count[0] != '\0') {
                p_resattr();
                p_setattr(pr->pr_attrcolor_count);

                if (is_yx) 
                        p_cursor_move(pr->pr_count_y, pr->pr_count_x);
                else 
                        printf("%c", ' ');
                
                printf("%zu%s", (size_t)(progress*100.0), pr->pr_count); 
        }
        
        if (!is_yx)
                p_cursor_unsave_and_attr();
        
        fflush(stdout);
        p_resattr();
}

void p_setind_sp(size_t y,
                 size_t x,
                 const char *attrcolor,
                 ind_spinner_s *sp) 
{
        sp->sp_spinner[0] = '|';
        sp->sp_spinner[1] = '/';
        sp->sp_spinner[2] = '-';
        sp->sp_spinner[3] = '\\';

        sp->sp_iterator = 0;

        sp->sp_y = y;
        sp->sp_x = x;

        if (attrcolor == NULL || *attrcolor == '\0') 
                strncpy(sp->sp_attrcolor, "\033[39;49m", LEN_ATTR_COLOR-1);
        else 
                strncpy(sp->sp_attrcolor, attrcolor, LEN_ATTR_COLOR-1);
        sp->sp_attrcolor[LEN_ATTR_COLOR-1] = '\0';
}

void p_idraw_sp(ind_spinner_s *sp) 
{
        p_resattr();
        p_setattr(sp->sp_attrcolor);

        if (sp->sp_y > 0 && sp->sp_x > 0) {
                p_cursor_move(sp->sp_y, sp->sp_x);
                printf("%c", sp->sp_spinner[sp->sp_iterator++]);
                if (sp->sp_iterator == 4)
                        sp->sp_iterator = 0;
        } else {
                printf("%c", sp->sp_spinner[sp->sp_iterator++]);
                if (sp->sp_iterator == 4)
                        sp->sp_iterator = 0;
                p_cursor_backward(1);        
        }

        fflush(stdout);
        p_resattr();
}

void p_setind_c(size_t y,
                size_t x,
                const char *attrcolor,
                const char *counter,
                ind_counter_s *c) 
{
        c->c_y = y;
        c->c_x = x;

        if (attrcolor == NULL || *attrcolor == '\0') 
                strncpy(c->c_attrcolor, "\033[39;49m", LEN_ATTR_COLOR-1);
        else 
                strncpy(c->c_attrcolor, attrcolor, LEN_ATTR_COLOR-1);
        c->c_attrcolor[LEN_ATTR_COLOR-1] = '\0';

        if (counter == NULL) { 
                strncpy(c->c_counter, "", LEN_COUNTER-1);
        } else 
                strncpy(c->c_counter, counter, LEN_COUNTER-1);
        c->c_counter[LEN_COUNTER-1] = '\0';
}

void p_idraw_c(size_t index, ind_counter_s *c) 
{
        p_resattr();
        p_setattr(c->c_attrcolor);

        if (c->c_y > 0 && c->c_x > 0) {
                p_cursor_move(c->c_y, c->c_x);
                printf("%zu", index);
                printf("%s", c->c_counter);
        } else {
                p_cursor_save_and_attr();
                printf("%zu", index);
                printf("%s", c->c_counter);
                p_cursor_unsave_and_attr();
        }

        fflush(stdout);
        p_resattr();
}

void p_setind_p(size_t y,
                size_t x,
                const char *attrcolor,
                unsigned int ms_delay, 
                ind_points_s *p) 
{
        p->p_y = y;
        p->p_x = x;

        if (attrcolor == NULL || *attrcolor == '\0') 
                strncpy(p->p_attrcolor, "\033[39;49m", LEN_ATTR_COLOR-1);
        else 
                strncpy(p->p_attrcolor, attrcolor, LEN_ATTR_COLOR-1);
        p->p_attrcolor[LEN_ATTR_COLOR-1] = '\0';

        p->p_ms_delay = ms_delay;
}

void p_idraw_p(ind_points_s *p) 
{
        const char *str = "   ";

        p_resattr();
        p_setattr(p->p_attrcolor);

        if (p->p_y > 0 && p->p_x > 0) {
                p_cursor_move(p->p_y, p->p_x);
                printf("%s", str);
                p_cursor_backward(3);
                for (size_t i = 0; i != 3; ++i) {
                        p_delay_ms(p->p_ms_delay); 
                        printf("%c", '.');                       
                        fflush(stdout);
                }
        } else {
                p_cursor_save_and_attr();
                printf("%s", str);
                p_cursor_backward(3);
                for (size_t i = 0; i != 3; ++i) {
                        p_delay_ms(p->p_ms_delay); 
                        printf("%c", '.');                       
                        fflush(stdout);
                }
                p_cursor_unsave_and_attr();
        }

        fflush(stdout);
        p_resattr();
}

void p_setind_w(size_t y,
                size_t x,
                const char *attrcolor,
                unsigned int ms_delay, 
                ind_wave_s *w) 
{
        w->w_y = y;
        w->w_x = x;

        if (attrcolor == NULL || *attrcolor == '\0') 
                strncpy(w->w_attrcolor, "\033[39;49m", LEN_ATTR_COLOR-1);
        else 
                strncpy(w->w_attrcolor, attrcolor, LEN_ATTR_COLOR-1);
        w->w_attrcolor[LEN_ATTR_COLOR-1] = '\0';

        w->w_ms_delay = ms_delay;

        w->w_wave[0]  = "\u2581\u2581\u2581";
        w->w_wave[1]  = "\u2581\u2582\u2584";
        w->w_wave[2]  = "\u2582\u2584\u2586";
        w->w_wave[3]  = "\u2584\u2586\u2588";
        w->w_wave[4]  = "\u2586\u2588\u2588";
        w->w_wave[5]  = "\u2588\u2588\u2586";
        w->w_wave[6]  = "\u2588\u2586\u2584";
        w->w_wave[7]  = "\u2586\u2584\u2582";
        w->w_wave[8]  = "\u2584\u2582\u2581";
        w->w_wave[9]  = "\u2582\u2581\u2581";
        w->w_wave[10] = "\u2581\u2581\u2581";
}

void p_idraw_w(ind_wave_s *w) 
{
        bool is_yx;

        if (w->w_y > 0 && w->w_x > 0) 
                is_yx = true;
        else
                is_yx = false;
        
        p_resattr();
        p_setattr(w->w_attrcolor);

        if (is_yx) 
                p_cursor_move(w->w_y, w->w_x);
        else
                p_cursor_save_and_attr();

        for (size_t i = 0; i != 11; ++i) {
                printf("%s", w->w_wave[i]);
                fflush(stdout);
                p_delay_ms(w->w_ms_delay);
                p_cursor_backward(3);
        }

        if (!is_yx)
                p_cursor_unsave_and_attr();

        fflush(stdout);
        p_resattr();
}

