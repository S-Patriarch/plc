/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : conio.h
 */

#ifndef __PLC_CONIO_H
#define __PLC_CONIO_H   1

#include <stddef.h>
#include <plc/plcdef.h>

void    p_conio_init(void) P_NOEXCEPT;                 /* инициализация структур и ...  */

void    p_setrow(size_t row) P_NOEXCEPT;               /* установка координаты y  */
void    p_setcol(size_t col) P_NOEXCEPT;               /* установка координаты x  */

size_t  p_getrow(void) P_NOEXCEPT;                     /* получение координаты y  */
size_t  p_getcol(void) P_NOEXCEPT;                     /* получение координаты x  */

size_t  p_getwsrow(void) P_NOEXCEPT;                   /* получение количества строк экрана  */
size_t  p_getwscol(void) P_NOEXCEPT;                   /* получение количества столбцов экрана  */

void    p_gotoyx(size_t row, size_t col) P_NOEXCEPT;   /* установка позиции курсора  */

int     p_getchar(char *c) P_NOEXCEPT;                 /* получение символа с stdin без нажатия enter  */
int     p_gethiddens(char *s, size_t size) P_NOEXCEPT; /* скрытое считывание неформатированных данных из потока  */

void    p_setattr(const char *attr) P_NOEXCEPT;        /* установка атрибутов  */
void    p_resattr(void) P_NOEXCEPT;                    /* сброс всех атрибутов  */

void    p_screen_save(void) P_NOEXCEPT;                /* вход в альтернативный экран  */
void    p_screen_restore(void) P_NOEXCEPT;             /* выход из альтернативного экрана  */

void    p_clrscr(void)P_NOEXCEPT;                      /* очистка экрана и установка курсора в позицию 1, 1  */

/*
 * Функции управления положением курсора.
 */
void    p_cursor_move(size_t row, size_t col) P_NOEXCEPT; /* установка позиции курсора  */
void    p_cursor_down(size_t count) P_NOEXCEPT;           /* перемещение курсора вниз  */
void    p_cursor_up(size_t count) P_NOEXCEPT;             /* перемещение курсора вверх  */
void    p_cursor_forward(size_t count) P_NOEXCEPT;        /* перемещение курсора вперед  */
void    p_cursor_backward(size_t count) P_NOEXCEPT;       /* перемещение курсора назад  */
void    p_cursor_save(void) P_NOEXCEPT;                   /* сохранение текущей позиции курсора  */
void    p_cursor_unsave(void) P_NOEXCEPT;                 /* восстановление позиции курсора  */
void    p_cursor_save_and_attr(void) P_NOEXCEPT;          /* сохранение текущей позиции курсора вместе с атрибутами  */
void    p_cursor_unsave_and_attr(void) P_NOEXCEPT;        /* восстановление позиции курсора вместе с атрибутами  */
void    p_cursor_hidden(void) P_NOEXCEPT;                 /* скрытие курсора  */
void    p_cursor_visible(void) P_NOEXCEPT;                /* показ курсора  */

/*
 * Функции управления скроллингом экрана.
 */
void    p_scroll_screen_all(void) P_NOEXCEPT;                 /* разрешает прокрутку всего экрана целиком  */
void    p_scroll_screen(size_t begin, size_t end) P_NOEXCEPT; /* разрешает прокрутку от начальной до конечной строки  */
void    p_scroll_down(void) P_NOEXCEPT;                       /* прокрутка экрана на одну строку вниз  */
void    p_scroll_up(void) P_NOEXCEPT;                         /* прокрутка экрана на одну строку вверх  */

/*
 * Функции управления табуляцией.
 */
void    p_tab_set(void) P_NOEXCEPT;       /* установка табуляции в текущей позиции  */
void    p_tab_clear(void) P_NOEXCEPT;     /* очищение табуляции в текущей позиции  */
void    p_tab_clear_all(void) P_NOEXCEPT; /* очищение всех установленных позиций табуляции  */

/*
 * Функции стирания текста.
 */
void    p_erase_end_of_line(void) P_NOEXCEPT;   /* стирание текста от текущей позиции курсора до конца текущей строки  */
void    p_erase_begin_of_line(void) P_NOEXCEPT; /* стирание текста от текущей позиции курсора до начала текущей позиции  */
void    p_erase_line(void) P_NOEXCEPT;          /* стирание всей текущей строки  */
void    p_erase_down(void) P_NOEXCEPT;          /* стирание экрана от текущей строки вниз до нижней границе экрана  */
void    p_erase_up(void) P_NOEXCEPT;            /* стирание экрана от текущей строки вверх до верхней границе экрана  */
void    p_erase_screen(void) P_NOEXCEPT;        /* стирание экрана с заполнением его цветом фона  */

/*
 * Функции установки режима терминала.
 */
void    p_device_reset(void) P_NOEXCEPT;      /* сброс всех настроек терминала в значения по умолчанию  */
void    p_line_wrap_enable(void) P_NOEXCEPT;  /* разрешает переход на следующую строку  */
void    p_line_wrap_disable(void) P_NOEXCEPT; /* запрещает переход на следующую строку  */

#endif  /* __PLC_CONIO_H  */
