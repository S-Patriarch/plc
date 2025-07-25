/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : conio.c
 */

#include "p_conio.h"
#include <plc/conio.h>
#include <plc/plcdef.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>

struct text_info t;
struct win_size ws;

void p_conio_init(void)
{
        struct winsize s;

        t.cur_row = 0;
        t.cur_col = 0;

        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &s) == 0) {
                ws.ws_row = s.ws_row;
                ws.ws_col = s.ws_col;
        } else {
                ws.ws_row = 0;
                ws.ws_col = 0;
        }
}

void p_setrow(size_t row)
{
        t.cur_row = row;
}

void p_setcol(size_t col)
{
        t.cur_col = col;
}

size_t p_getrow(void)
{
        return t.cur_row;
}

size_t p_getcol(void)
{
        return t.cur_col;
}

size_t p_getwsrow(void)
{
        return ws.ws_row;
}

size_t p_getwscol(void)
{
        return ws.ws_col;
}

void p_gotoyx(size_t row, size_t col)
{
        t.cur_row = row;
        t.cur_col = col;
        printf("\033[%zu;%zuH", t.cur_row, t.cur_col);
}

int p_getchar(char *c)
{
        struct termios old_t;

        if (tcgetattr(0, &old_t) < 0) 
                return P_ERROR;

        old_t.c_lflag &= ~ICANON;
        old_t.c_lflag &= ~ECHO;
        old_t.c_cc[VMIN] = 1;
        old_t.c_cc[VTIME] = 0;
        
        if (tcsetattr(0, TCSANOW, &old_t) < 0) 
                return P_ERROR;
        if (read(0, c, 1) < 0) 
                return P_ERROR;
        
        old_t.c_lflag |= ICANON;
        old_t.c_lflag |= ECHO;
        
        if (tcsetattr(0, TCSADRAIN, &old_t) < 0) 
                return P_ERROR;

        return P_SUCCESS;
}

int p_gethiddens(char *s, size_t size)
{
        struct termios old_t, new_t;

        if (tcgetattr(STDIN_FILENO, &old_t) < 0) 
                return P_ERROR;
        
        new_t = old_t;
        new_t.c_lflag &= ~ECHO;
        
        if (tcsetattr(STDIN_FILENO, TCSANOW, &new_t) < 0) 
                return P_ERROR;
        
        if (fgets(s, size, stdin) != NULL) {
                size_t len = strlen(s);
                if (len > 0 && s[len-1] == '\n')
                        s[len-1] = '\0';
        } else  
                return P_ERROR;
        
        if (tcsetattr(STDIN_FILENO, TCSANOW, &old_t) < 0) 
                return P_ERROR;

        return P_SUCCESS;
}

void p_setattr(const char *attr)
{
        printf("%s", attr);
}

void p_resattr(void)
{
        const size_t RESET = 0;
        printf("\033[%zum", RESET);
}

void p_screen_save(void)
{
        printf("\033[?1049h");
}

void p_screen_restore(void)
{
        printf("\033[?1049l");
}

void p_clrscr(void)
{
        printf("\033[2J\033[1;1H");
}

/*
 * Функции управления положением курсора.
 */
void p_cursor_move(size_t row, size_t col)
{
        t.cur_row = row;
        t.cur_col = col;
        printf("\033[%zu;%zuH", t.cur_row, t.cur_col);
}

void p_cursor_down(size_t count)
{
        t.cur_row += count;
        printf("\033[%zuB", count);
}

void p_cursor_up(size_t count)
{
        t.cur_row -= count;
        printf("\033[%zuA", count);
}

void p_cursor_forward(size_t count) 
{
        t.cur_col += count;
        printf("\033[%zuC", count);
}

void p_cursor_backward(size_t count)
{
        t.cur_col -= count;
        printf("\033[%zuD", count);
}

void p_cursor_save(void)
{
        printf("\033[s");
}

void p_cursor_unsave(void)
{
        printf("\033[u");
}

void p_cursor_save_and_attr(void)
{
        printf("\0337");
}

void p_cursor_unsave_and_attr(void) 
{
        printf("\0338");
}

void p_cursor_hidden(void)          
{
        printf("\033[?25l");
}

void p_cursor_visible(void)         
{
        printf("\033[?25h");
}

/*
 * Функции управления скроллингом экрана.
 */
void p_scroll_screen_all(void)
{
        printf("\033[r");
}

void p_scroll_screen(size_t begin, size_t end)
{
        printf("\033[%zu;%zur", begin, end);
}

void p_scroll_down(void)
{
        printf("\033D");
}

void p_scroll_up(void)
{
        printf("\033M");
}

/*
 * Функции управления табуляцией.
 */
void p_tab_set(void)
{
        printf("\033H");
}

void p_tab_clear(void)
{
        printf("\033[g");
}

void p_tab_clear_all(void)
{
        printf("\033[3g");
}

/*
 * Функции стирания текста.
 */
void p_erase_end_of_line(void)
{
        printf("\033[K");
}

void p_erase_begin_of_line(void)
{
        printf("\033[1K");
}

void p_erase_line(void)
{
        printf("\033[2K");
}

void p_erase_down(void)
{
        printf("\033[J");
}

void p_erase_up(void)
{
        printf("\033[1J");
}

void p_erase_screen(void)
{
        printf("\033[2J");
}

/*
 * Функции установки режима терминала.
 */
void p_device_reset(void)
{
        printf("\033c");
}

void p_line_wrap_enable(void)
{
        printf("\033[7h");
}

void p_line_wrap_disable(void)
{
        printf("\033[7l");
}

