/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            <regexp.h>
 */

#ifndef __PLC_REGEXP_H
#define __PLC_REGEXP_H  1

#include <plc/plcdef.h>

P_BEGIN_DECLS

/*
 * Легенды UNIX Брайан Керниган и Роб Пайк написали сопоставитель регулярных
 * выражений всего в несколько десятках строк на С.  Он поддерживает ^,$,. и *.
 * Весь механизм сопоставления работает через рекурсию и арифметику указателей.
 */

/* Поиск регулярного выражения regexp в любом месте текста text.
   Возвращает 1 - сопоставимо, 0 - нет.  */
extern int p_match (const char *regexp, const char *text) P_NOEXCEPT;

/* Поиск регулярного выражения regexp в начале текста text.  */
extern int p_matchhere (const char *regexp, const char *text) P_NOEXCEPT;

/* Поиск регулярного выражения c*regexp в начале текста text.  */
extern int p_matchstar (int c, const char *regexp, const char *text) P_NOEXCEPT;

P_END_DECLS

#endif /* regexp.h  */
