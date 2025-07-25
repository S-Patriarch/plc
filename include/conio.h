/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : conio.h
 */

#ifndef __PLC_CONIO_H
#define __PLC_CONIO_H   1

#include <stddef.h>

void p_conio_init(void); /* инициализация структур и ... */

void p_setrow(size_t row); /* установка координаты y */
void p_setcol(size_t col); /* установка координаты x */

size_t p_getrow(void); /* получение координаты y */
size_t p_getcol(void); /* получение координаты x */

size_t p_getwsrow(void); /* получение количества строк экрана */
size_t p_getwscol(void); /* получение количества столбцов экрана */

void p_gotoyx(size_t row, size_t col);  /* установка позиции курсора */

int p_getchar(char *c);                 /* получение символа с stdin без нажатия enter */
int p_gethiddens(char *s, size_t size); /* скрытое считывание неформатированных данных из потока*/

void p_setattr(const char *attr); /* установка атрибутов */
void p_resattr(void);             /* сброс всех атрибутов */

void p_screen_save(void);    /* вход в альтернативный экран */
void p_screen_restore(void); /* выход из альтернативного экрана */

void p_clrscr(void); /* очистка экрана и установка курсора в позицию 1, 1 */

/*
 * Функции управления положением курсора.
 */
void p_cursor_move(size_t row, size_t col); /* установка позиции курсора */
void p_cursor_down(size_t count);           /* перемещение курсора вниз */
void p_cursor_up(size_t count);             /* перемещение курсора вверх */
void p_cursor_forward(size_t count);        /* перемещение курсора вперед */
void p_cursor_backward(size_t count);       /* перемещение курсора назад */
void p_cursor_save(void);                   /* сохранение текущей позиции курсора */
void p_cursor_unsave(void);                 /* восстановление позиции курсора */
void p_cursor_save_and_attr(void);          /* сохранение текущей позиции курсора вместе с атрибутами */
void p_cursor_unsave_and_attr(void);        /* восстановление позиции курсора вместе с атрибутами */
void p_cursor_hidden(void);                 /* скрытие курсора */
void p_cursor_visible(void);                /* показ курсора */

/*
 * Функции управления скроллингом экрана.
 */
void p_scroll_screen_all(void);                 /* разрешает прокрутку всего экрана целиком */
void p_scroll_screen(size_t begin, size_t end); /* разрешает прокрутку от начальной до конечной строки */
void p_scroll_down(void);                       /* прокрутка экрана на одну строку вниз */
void p_scroll_up(void);                         /* прокрутка экрана на одну строку вверх */

/*
 * Функции управления табуляцией.
 */
void p_tab_set(void);       /* установка табуляции в текущей позиции */
void p_tab_clear(void);     /* очищение табуляции в текущей позиции */
void p_tab_clear_all(void); /* очищение всех установленных позиций табуляции */

/*
 * Функции стирания текста.
 */
void p_erase_end_of_line(void);   /* стирание текста от текущей позиции курсора до конца текущей строки */
void p_erase_begin_of_line(void); /* стирание текста от текущей позиции курсора до начала текущей позиции */
void p_erase_line(void);          /* стирание всей текущей строки */
void p_erase_down(void);          /* стирание экрана от текущей строки вниз до нижней границе экрана */
void p_erase_up(void);            /* стирание экрана от текущей строки вверх до верхней границе экрана */
void p_erase_screen(void);        /* стирание экрана с заполнением его цветом фона */

/*
 * Функции установки режима терминала.
 */
void p_device_reset(void);      /* сброс всех настроек терминала в значения по умолчанию */
void p_line_wrap_enable(void);  /* разрешает переход на следующую строку */
void p_line_wrap_disable(void); /* запрещает переход на следующую строку */

#endif  /* __PLC_CONIO_H */
