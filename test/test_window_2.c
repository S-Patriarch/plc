#include <plc/plc.h>
#include <stdio.h>

static window_s w1;
static void init_window(void);

int main(void)
{
        char ch;

        const char *str = "Какой-то длинный текст, который надо вывести в окне."
                          "Какой-то длинный текст, который надо вывести в окне."
                          "Какой-то длинный текст, который надо вывести в окне."
                          "Какой-то длинный текст, который надо вывести в окне."
                          "Какой-то длинный текст, который надо вывести в окне.";
/*
        const char *str = "Sdahfg sag gdsadjhas ksajlksdfkl sdlkfjlkfl djhasdjh."
                          "Sdahfg sag gdsadjhas ksajlksdfkl sdlkfjlkfl djhasdjh."
                          "Sdahfg sag gdsadjhas ksajlksdfkl sdlkfjlkfl djhasdjh."
                          "Sdahfg sag gdsadjhas ksajlksdfkl sdlkfjlkfl djhasdjh."
                          "Sdahfg sag gdsadjhas ksajlksdfkl sdlkfjlkfl djhasdjh.";
*/
        init_window();

        screen_save();
        cursor_hidden();

        wdraw(&w1);
        int res = wout(str, "\033[90;48;5;234m", &w1);

        getch(&ch);
        cursor_visible();
        screen_restore();
}

static void init_window(void) 
{
        setwindow(3, 10, 
                  10, 30,
                  wb_slr,
                  "\033[90;48;5;234m",
                  &w1);
}

