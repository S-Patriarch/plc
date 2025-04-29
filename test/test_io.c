#include <plc/io.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
        int res = mout("Test of mout()...\n", mess_w, NULL, "\033[33m");
        printf("%d\n", res);

        exit(EXIT_SUCCESS);        
}

