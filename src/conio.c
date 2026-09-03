/* Copyright (C) 2025-2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            conio.c
 */

#include <plc/conio.h>

struct text_info {
        size_t  cur_row;
        size_t  cur_col;
} t;

struct win_size {
        size_t  ws_row;
        size_t  ws_col;
} ws;

void 
p_conio_init(void) P_NOEXCEPT
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

void 
p_setrow(size_t row) P_NOEXCEPT
{
        t.cur_row = row;
}

void 
p_setcol(size_t col) P_NOEXCEPT
{
        t.cur_col = col;
}

size_t 
p_getrow(void) P_NOEXCEPT
{
        return (t.cur_row);
}

size_t 
p_getcol(void) P_NOEXCEPT
{
        return (t.cur_col);
}

size_t 
p_getwsrow(void) P_NOEXCEPT
{
        return (ws.ws_row);
}

size_t 
p_getwscol(void) P_NOEXCEPT
{
        return (ws.ws_col);
}

void
p_getsizeterm(size_t *rows, size_t *cols) P_NOEXCEPT
{
        struct winsize ws;

        if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
                *rows = 24;
                *cols = 80;
        } else {
                *rows = ws.ws_row;
                *cols = ws.ws_col;
        }
}

void 
p_gotoyx(size_t row, size_t col) P_NOEXCEPT
{
        t.cur_row = row;
        t.cur_col = col;
        printf("\033[%zu;%zuH", t.cur_row, t.cur_col);
}

int 
p_getchar(char *c) P_NOEXCEPT
{
        struct termios old_t;

        if (tcgetattr(0, &old_t) < 0) 
                return (P_ERROR);

        old_t.c_lflag &= ~ICANON;
        old_t.c_lflag &= ~ECHO;
        old_t.c_cc[VMIN] = 1;
        old_t.c_cc[VTIME] = 0;
        
        if (tcsetattr(0, TCSANOW, &old_t) < 0) 
                return (P_ERROR);
        if (read(0, c, 1) < 0) 
                return (P_ERROR);
        
        old_t.c_lflag |= ICANON;
        old_t.c_lflag |= ECHO;
        
        if (tcsetattr(0, TCSADRAIN, &old_t) < 0) 
                return (P_ERROR);

        return 0;
}

int 
p_gethiddens(char *s, size_t size) P_NOEXCEPT
{
        struct termios old_t, new_t;

        if (tcgetattr(STDIN_FILENO, &old_t) < 0) 
                return (P_ERROR);
        
        new_t = old_t;
        new_t.c_lflag &= ~ECHO;
        
        if (tcsetattr(STDIN_FILENO, TCSANOW, &new_t) < 0) 
                return (P_ERROR);
        
        if (fgets(s, size, stdin) != NULL) {
                size_t len = strlen(s);
                if (len > 0 && s[len-1] == '\n')
                        s[len-1] = '\0';
        } else  
                return (P_ERROR);
        
        if (tcsetattr(STDIN_FILENO, TCSANOW, &old_t) < 0) 
                return (P_ERROR);

        return 0;
}

char *
p_getpass(void) P_NOEXCEPT
{
        static char     buf[P_MAXPASSLEN + 1]; /* нулевой байт в конце  */
        char            *ptr;
        sigset_t        sig, osig;
        struct termios  ts, ots;
        FILE            *fp;
        int             c;

        if ((fp = fopen(ctermid(NULL), "r+")) == NULL)
                return(NULL);

        setbuf(fp, NULL);

        sigemptyset(&sig);
        sigaddset(&sig, SIGINT);             /* заблокировать SIGINT  */
        sigaddset(&sig, SIGTSTP);            /* заблокировать SIGTSTP  */
        sigprocmask(SIG_BLOCK, &sig, &osig); /* сохранить маску  */

        tcgetattr(fileno(fp), &ts); /* сохранить состояние терминала  */
        ots = ts;                   /* скопировать структуру  */
        ts.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL);
        tcsetattr(fileno(fp), TCSAFLUSH, &ts);

        ptr = buf;
        while ((c = getc(fp)) != EOF && c != '\n')
                if (ptr < &buf[P_MAXPASSLEN])
                        *ptr++ = c;
        *ptr = 0;       /* завершающий нулевой символ  */
        putc('\n', fp); /* вывести символ перевода строки  */

        tcsetattr(fileno(fp), TCSAFLUSH, &ots); /* восстановить состояние терминала  */
        sigprocmask(SIG_SETMASK, &osig, NULL);  /* восстановить маску  */
        fclose(fp);                             /* завершить работу с /dev/tty  */

        return(buf);
}

void 
p_setattr(const char *attr) P_NOEXCEPT
{
        printf("%s", attr);
}

void 
p_resattr(void) P_NOEXCEPT
{
        const size_t RESET = 0;
        printf("\033[%zum", RESET);
}

void 
p_screen_save(void) P_NOEXCEPT
{
        printf("\033[?1049h");
}

void 
p_screen_restore(void) P_NOEXCEPT
{
        printf("\033[?1049l");
}

void 
p_clrscr(void) P_NOEXCEPT
{
        printf("\033[2J\033[1;1H");
}

void
p_clrarea(size_t start_row, size_t start_col,
          size_t height, size_t width) P_NOEXCEPT
{
        size_t  i, j;

        for (i = 0; i < height; i++) {
                printf("\033[%zu;%zuH", start_row + i, start_col);
                for (j = 0; j < width; j++)
                        printf(" ");
        }
        fflush(stdout);
}

void
p_posarea(size_t start_row, size_t start_col,
          size_t offset_row, size_t offset_col) P_NOEXCEPT
{
        printf("\033[%zu;%zuH", start_row + offset_row, start_col + offset_col);
        fflush(stdout);
}

/*
 * Функции управления положением курсора.
 */
void 
p_cursor_move(size_t row, size_t col) P_NOEXCEPT
{
        t.cur_row = row;
        t.cur_col = col;
        printf("\033[%zu;%zuH", t.cur_row, t.cur_col);
}

void 
p_cursor_down(size_t count) P_NOEXCEPT
{
        t.cur_row += count;
        printf("\033[%zuB", count);
}

void 
p_cursor_up(size_t count) P_NOEXCEPT
{
        t.cur_row -= count;
        printf("\033[%zuA", count);
}

void 
p_cursor_forward(size_t count) P_NOEXCEPT
{
        t.cur_col += count;
        printf("\033[%zuC", count);
}

void 
p_cursor_backward(size_t count) P_NOEXCEPT
{
        t.cur_col -= count;
        printf("\033[%zuD", count);
}

void 
p_cursor_save(void) P_NOEXCEPT
{
        printf("\033[s");
}

void 
p_cursor_unsave(void) P_NOEXCEPT
{
        printf("\033[u");
}

void 
p_cursor_save_and_attr(void) P_NOEXCEPT
{
        printf("\0337");
}

void 
p_cursor_unsave_and_attr(void) P_NOEXCEPT
{
        printf("\0338");
}

void 
p_cursor_hidden(void) P_NOEXCEPT
{
        printf("\033[?25l");
}

void 
p_cursor_visible(void) P_NOEXCEPT  
{
        printf("\033[?25h");
}

/*
 * Функции управления скроллингом экрана.
 */
void 
p_scroll_screen_all(void) P_NOEXCEPT
{
        printf("\033[r");
}

void 
p_scroll_screen(size_t begin, size_t end) P_NOEXCEPT
{
        printf("\033[%zu;%zur", begin, end);
}

void 
p_scroll_down(void) P_NOEXCEPT
{
        printf("\033D");
}

void 
p_scroll_up(void) P_NOEXCEPT
{
        printf("\033M");
}

/*
 * Функции управления табуляцией.
 */
void 
p_tab_set(void) P_NOEXCEPT
{
        printf("\033H");
}

void 
p_tab_clear(void) P_NOEXCEPT
{
        printf("\033[g");
}

void 
p_tab_clear_all(void) P_NOEXCEPT
{
        printf("\033[3g");
}

/*
 * Функции стирания текста.
 */
void 
p_erase_end_of_line(void) P_NOEXCEPT
{
        printf("\033[K");
}

void 
p_erase_begin_of_line(void) P_NOEXCEPT
{
        printf("\033[1K");
}

void 
p_erase_line(void) P_NOEXCEPT
{
        printf("\033[2K");
}

void 
p_erase_down(void) P_NOEXCEPT
{
        printf("\033[J");
}

void 
p_erase_up(void) P_NOEXCEPT
{
        printf("\033[1J");
}

void 
p_erase_screen(void) P_NOEXCEPT
{
        printf("\033[2J");
}

/*
 * Функции установки режима терминала.
 */
void 
p_device_reset(void) P_NOEXCEPT
{
        printf("\033c");
}

void 
p_line_wrap_enable(void) P_NOEXCEPT
{
        printf("\033[7h");
}

void 
p_line_wrap_disable(void) P_NOEXCEPT
{
        printf("\033[7l");
}

