#include <plc/timer.h>
#include <plc/conio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

int main(void) 
{
        int32_t is;
        char c;
        double res_sec, res_min, res_hor;

        puts("Press any key...");
        is = getch(&c);

        putchar('\n');
        
        start_timer();
        puts("W: Начался замер.");
        
        is = getch(&c);

        stop_timer();
        puts("W: Закончился замер.");
        putchar('\n');

        res_sec = duration_seconds();
        res_min = duration_minutes();
        res_hor = duration_hours();

        puts("Результаты замера:");

        printf("секунд : %.2f\n", res_sec);
        printf("минут  : %.2f\n", res_min);
        printf("часов  : %.2f\n", res_hor);

        exit(EXIT_SUCCESS);
}

