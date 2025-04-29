#include <plc/system.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
        const char *loc = "ru_RU.utf8";
        bool is = is_locale_available(loc);

        if (is)
                printf("Локаль '%s' доступна.\n", loc);
        else
                printf("Локаль '%s' недоступна.\n", loc);

        exit(EXIT_SUCCESS);        
}

