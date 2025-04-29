#include <plc/conio.h>
#include <plc/line.h>
#include <stdlib.h>
#include <stdio.h>

static line_s l1, l2, l3, l4, l5, l6;
static void init_line(void);

int main(void)
{
        char c;

        init_line();

        screen_save();
        cursor_hidden();

        ldraw(&l1);
        ldraw(&l2);
        ldraw(&l3);
        ldraw(&l4);
        ldraw(&l5);
        ldraw(&l6);

        getch(&c);
        cursor_visible();
        screen_restore();

        exit(EXIT_SUCCESS);        
}

static void init_line(void) 
{
        setline(3, 10, 70,
                line_h,
                line_sl,
                NULL,
                &l1);

        setline(4, 10, 70,
                line_h,
                line_sh,
                "\033[38;5;100;48;5;235m",
                &l2);

        setline(5, 10, 70,
                line_h,
                line_dl,
                "\033[38;5;100;48;5;235m",
                &l3);

        setline(7, 10, 10,
                line_v,
                line_sl,
                "\033[38;5;100;48;5;235m",
                &l4);

        setline(7, 12, 10,
                line_v,
                line_sh,
                "\033[38;5;100;48;5;235m",
                &l5);

        setline(7, 14, 10,
                line_v,
                line_dl,
                "\033[38;5;100;48;5;235m",
                &l6);
}

