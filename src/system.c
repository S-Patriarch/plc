/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : system.c
 */

#include <plc/system.h>
#include <locale.h>

int is_locale_available(const char *locale)
{
        char *old_locale = setlocale(LC_ALL, NULL);
        char *res = setlocale(LC_ALL, locale);

        if (res == NULL) 
                return 0; // false

        setlocale(LC_ALL, old_locale);
        return 1; // true
}

