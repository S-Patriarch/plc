#include <plc/plc.h>
#include <stdlib.h>
#include <stdio.h>

void func0(void)
{
        resattr();
        setattr("\033[96;49m");
        cursor_move(15, 10);
        printf("Вызвана функция, закрепленная за 0 пунктом меню.");
        fflush(stdout);
}

void func1(void)
{
        resattr();
        setattr("\033[96;49m");
        cursor_move(15, 10);
        printf("Вызвана функция, закрепленная за 1 пунктом меню.");
        fflush(stdout);
}

void func2(void)
{
        resattr();
        setattr("\033[96;49m");
        cursor_move(15, 10);
        printf("Вызвана функция, закрепленная за 2 пунктом меню.");
        fflush(stdout);
}

void func3(void)
{
        resattr();
        setattr("\033[96;49m");
        cursor_move(15, 10);
        printf("Вызвана функция, закрепленная за 3 пунктом меню.");
        fflush(stdout);
}

void func4(void)
{
        resattr();
        setattr("\033[96;49m");
        cursor_move(15, 10);
        printf("Вызвана функция, закрепленная за 4 пунктом меню.");
        fflush(stdout);
}

void func_exit(void)
{
        cursor_visible();
        screen_restore();
        exit(EXIT_SUCCESS);
}

int main(void)
{
        menu_s menu[] = {
                {" Пункт_0 ", func0,     3, 10},
                {" Пункт_1 ", func1,     4, 15},
                {" Пункт_2 ", func2,     5, 20},
                {" Пункт_3 ", func3,     6, 25},
                {" Пункт_4 ", func4,     7, 30},
                {"  Выход  ", func_exit, 8, 35},
        };
        menu_style_s mst;

        setmenu(NULL, NULL, &mst);

        screen_save();
        cursor_hidden();

        mdraw(&mst, menu, 6, 0);
}
