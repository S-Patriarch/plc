/*
 * (C) 2025, S-Patriarch
 * This file is part of the PLC library.
 *
 * Patriarch Library C : io.c
 */

#include <plc/io.h>
#include <plc/plcdef.h>
#include <stdio.h>
#include <string.h>

int mout(const char *message, 
         enum mess_format format, 
         const char *attr_m, 
         const char *attr_f)
{
        if (message == NULL || *message == '\0') 
                return ERROR;

        char sf[4];
        int res_m, res_f;

        if (format == mess_n)
                strcpy(sf, "");
        if (format == mess_w)
                strcpy(sf, "W: ");
        if (format == mess_e)
                strcpy(sf, "E: ");

        if (attr_f == NULL) 
                res_f = printf("%s", sf);
        else 
                res_f = printf("%s%s\033[0m", attr_f, sf);
        
        if (res_f == EOF) 
                return ERROR;

        if (attr_m == NULL) 
                res_m = printf("%s", message);
        else 
                res_m = printf("%s%s\033[0m", attr_m, message);
        
        if (res_m == EOF) 
                return ERROR;

        return (res_f + res_m);
}

