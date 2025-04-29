#include <plc/plc.h>
#include <stdint.h>
#include <stdio.h>

int main(void) 
{
        uint8_t p = 0;

        bool b1 = true;
        bool b2 = false;
        bool b3 = true;
        bool b4 = false;
        bool b5 = true;
        bool b6 = false;
        bool b7 = true;
        bool b8 = false;

        puts("pack_bools");

        p = pack_bools(b1, b2, b3, b4, b5, b6, b7, b8);
        printf("p : %d\n\n", p);
        
        puts("unpack_bools");
        
        unpack_bools(p, &b1, &b2, &b3, &b4, &b5, &b6, &b7, &b8);
        printf("b1 : %d\n", b1);
        printf("b2 : %d\n", b2);
        printf("b3 : %d\n", b3);
        printf("b4 : %d\n", b4);
        printf("b5 : %d\n", b5);
        printf("b6 : %d\n", b6);
        printf("b7 : %d\n", b7);
        printf("b8 : %d\n", b8);

        putchar('\n');

        printf("powi(2, 3) : %d\n\n", powi(2, 3));

        printf("dectooct(42) : %d\n", dectooct(42));
        printf("dectobin(42) : %lld\n", dectobin(42));

        putchar('\n');

        char *str = "Test of string hash...";
        unsigned int hash = strhash(str);
        printf("String            : %s\n", str);
        printf("Hash string (dec) : %u\n", hash);
        printf("Hash string (hex) : %x\n", hash);
}

