#include <plc/string.h>
#include <plc/plcdef.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) 
{
        char str[] = "1Test 2string3...";
        printf("Исходная строка  -> %s\n", str);

        int res = strrev(str);
        if (res == ERROR)
                exit(EXIT_FAILURE);

        printf("strrev           -> %s\n", str);

        size_t len = count_char(str, 's');
        printf("count_char       -> %zu\n", len);

        len = count_words(str);
        printf("count_words      -> %zu\n", len);
        
        len = count_utf8_chars(str);
        printf("count_utf8_chars -> %zu\n", len);

        size_t d_count;
        size_t *digits = extract_digits(str, &d_count);
        if (digits != NULL && d_count != 0) {
                printf("d_count          -> %zu\n", d_count);
                printf("digits           -> ");
                for (size_t i = 0; i != d_count; i++)
                        printf("%zu ", digits[i]);
                putchar('\n');
                free(digits);
                digits = NULL;
        } else
                puts("W: Отсутствуют цифры или память не выделена.");

        exit(EXIT_SUCCESS);
}

