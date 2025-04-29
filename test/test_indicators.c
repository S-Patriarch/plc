#include <plc/conio.h>
#include <plc/indicators.h>
#include <plc/dt.h>
#include <stdio.h>
#include <unistd.h>

static ind_progress_s pr;
static ind_spinner_s sp;
static ind_counter_s cn;
static ind_points_s p;
static ind_wave_s w;

static void init_ind(void);

int main(void)
{
        char c;

        init_ind();

        screen_save();
        cursor_hidden();
        cursor_move(1, 1);

        printf("Test progress : ");
        for (size_t i = getind_pr_min(&pr); i <= getind_pr_max(&pr); ++i) {
                idraw_pr(i, &pr);
                delay_ms(100);
        }

        putchar('\n');

        printf("Test spinner  : ");
        for (size_t i = 1; i <= 36; ++i) {
                idraw_sp(&sp);
                delay_ms(100);
        }

        putchar('\n');

        printf("Test counter  : ");
        for (size_t i = 1; i <= 100; ++i) {
                idraw_c(i, &cn);
                delay_ms(100);
        }

        putchar('\n');

        printf("Test points   : ");
        for (size_t i = 1; i <= 16; ++i) {
                idraw_p(&p);
                delay_ms(100);
        }

        putchar('\n');

        printf("Test wave     : ");
        for (size_t i = 1; i <= 16; ++i) {
                idraw_w(&w);
                delay_ms(100);
        }

        getch(&c);
        cursor_visible();
        screen_restore();
}

static void init_ind(void) 
{
        char attr[30] = "\033[96;49m";

        setind_sp(0, 0, attr, &sp);

        setind_c(0, 0, attr, "/100", &cn);
        
        setind_pr(0, 0,
                  30,
                  1, 100,
                  "\033[96;49m",
                  "\033[38;5;238;49m",
                  "\033[96;49m",
                  "\u2588",
                  "\u2591",
                  '|', '|',
                  9, 10,
                  "\033[96;49m",
                  "%",
                  &pr);

        setind_p(0, 0, attr, 200, &p);

        setind_w(0, 0, attr,50, &w);
}

