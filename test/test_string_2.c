#include <plc/string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

int main(void) 
{
        const char *str = "Hello world this is a test_string_2";
        size_t w_count;
        char **words = strspl(str, &w_count);

        printf("Исходная строка : %s\n", str);
        if (words != NULL) {
                printf("Количество слов в строке : %zu\n", w_count);
                for (size_t i = 0; i != w_count; i++)
                        printf("Слово %d : %s\n", i + 1, words[i]);

                free_words(words, w_count);
        } else
                puts("E: ...");

        exit(EXIT_SUCCESS);
}

