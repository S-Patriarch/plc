#include <plc/conio.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
        int ok;
        char str[256];
        char c;

        init_conio();
        screen_save();
        cursor_move(1, 1);

        ok = gethiddens(str, sizeof(str));
        printf("ok  : %d\n", ok);
        printf("str : %s\n", str);

        setattr("\033[33;48;5;233m");

        putchar('\n');
        puts("Test string...");

        ok = getch(&c);
        resattr();
        screen_restore();

        exit(EXIT_SUCCESS);
}

