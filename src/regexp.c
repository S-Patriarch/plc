/* Copyright (C) 2026, S-Patriarch
   This file is part of the PLC library.  */

/*
 *      Patriarch Library C:                            regexp.c
 */

#include <plc/regexp.h>

int
p_match(const char *regexp, const char *text) P_NOEXCEPT
{
        if (regexp[0] == '^')
                return(p_matchhere(regexp + 1, text));
        do { /* нужно проверять, даже если строка пуста  */
                if (p_matchhere(regexp, text))
                        return(1);
        } while (*text++ != '\0');
        return(0);
}

int
p_matchhere(const char *regexp, const char *text) P_NOEXCEPT
{
        if (regexp[0] == '\0')
                return(1);
        if (regexp[1] == '*')
                return(p_matchstar(regexp[0], regexp + 2, text));
        if (regexp[0] == '$' && regexp[1] == '\0')
                return(*text == '\0');
        if (*text != '\0' && (regexp[0] == '.' || regexp[0] == *text))
                return(p_matchhere(regexp + 1, text + 1));
        return(0);
}

int
p_matchstar(int c, const char *regexp, const char *text) P_NOEXCEPT
{
        do { /* a * соответствует нулю или более вхождениям  */
                if (p_matchhere(regexp, text))
                        return(1);
        } while (*text != '\0' && (*text++ == c || c == '.'));
        return(0);
}
