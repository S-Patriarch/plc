/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : conio.h
 */

#ifndef __PLC_CONIO_H
#define __PLC_CONIO_H   1

#include <stddef.h>

void init_conio(void); /* инициализация структур и ... */

void setrow(size_t); /* установка координаты y */
void setcol(size_t); /* установка координаты x */

size_t getrow(void); /* получение координаты y */
size_t getcol(void); /* получение координаты x */

size_t getwsrow(void); /* получение количества строк экрана */
size_t getwscol(void); /* получение количества столбцов экрана */

void gotoyx(size_t, size_t); /* установка позиции курсора */

int getch(char *);              /* получение символа с stdin без нажатия enter */
int gethiddens(char *, size_t); /* скрытое считывание неформатированных данных из потока*/

void setattr(const char *); /* установка атрибутов */
void resattr(void);         /* сброс всех атрибутов */

void screen_save(void);    /* вход в альтернативный экран */
void screen_restore(void); /* выход из альтернативного экрана */

/*
 * Функции управления положением курсора.
 */
void cursor_move(size_t, size_t);  /* установка позиции курсора */
void cursor_down(size_t);          /* перемещение курсора вниз */
void cursor_up(size_t);            /* перемещение курсора вверх */
void cursor_forward(size_t);       /* перемещение курсора вперед */
void cursor_backward(size_t);      /* перемещение курсора назад */
void cursor_save(void);            /* сохранение текущей позиции курсора */
void cursor_unsave(void);          /* восстановление позиции курсора */
void cursor_save_and_attr(void);   /* сохранение текущей позиции курсора вместе с атрибутами */
void cursor_unsave_and_attr(void); /* восстановление позиции курсора вместе с атрибутами */
void cursor_hidden(void);          /* скрытие курсора */
void cursor_visible(void);         /* показ курсора */

/*
 * Функции управления скроллингом экрана.
 */
void scroll_screen_all(void);       /* разрешает прокрутку всего экрана целиком */
void scroll_screen(size_t, size_t); /* разрешает прокрутку от начальной до конечной строки */
void scroll_down(void);             /* прокрутка экрана на одну строку вниз */
void scroll_up(void);               /* прокрутка экрана на одну строку вверх */

/*
 * Функции управления табуляцией.
 */
void tab_set(void);       /* установка табуляции в текущей позиции */
void tab_clear(void);     /* очищение табуляции в текущей позиции */
void tab_clear_all(void); /* очищение всех установленных позиций табуляции */

/*
 * Функции стирания текста.
 */
void erase_end_of_line(void);   /* стирание текста от текущей позиции курсора до конца текущей строки */
void erase_begin_of_line(void); /* стирание текста от текущей позиции курсора до начала текущей позиции */
void erase_line(void);          /* стирание всей текущей строки */
void erase_down(void);          /* стирание экрана от текущей строки вниз до нижней границе экрана */
void erase_up(void);            /* стирание экрана от текущей строки вверх до верхней границе экрана */
void erase_screen(void);        /* стирание экрана с заполнением его цветом фона */

/*
 * Функции установки режима терминала.
 */
void device_reset(void);      /* сброс всех настроек терминала в значения по умолчанию */
void line_wrap_enable(void);  /* разрешает переход на следующую строку */
void line_wrap_disable(void); /* запрещает переход на следующую строку */

#endif  /* __PLC_CONIO_H */
