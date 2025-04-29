#include <plc/conio.h>
#include <plc/window.h>
#include <stdlib.h>
#include <stdio.h>

static window_s w1;
static void init_window(void);

int main(void)
{
        char c;

        init_window();

        screen_save();
        cursor_hidden();

        wdraw(&w1);

        getch(&c);
        cursor_visible();
        screen_restore();

        exit(EXIT_SUCCESS);        
}

static void init_window(void) 
{
        setwindow(3, 10, 
                  10, 70,
                  wb_slr,
                  "\033[90;48;5;234m",
                  &w1);

        setwintitle(" plc/window in libplc.a ",
                    wt_l,
                    "\033[97;48;5;234m",
                    &w1);
}

