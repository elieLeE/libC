#include <stdio.h>

#include "../../src/logger/logger.h"

int main(void)
{
    printf("hello_word normal\n");
    printf(COLOR_RED "hello_word en rouge\n" COLOR_RESET);
    printf(COLOR_YELLOW "hello_word en jaune\n" COLOR_RESET);
    printf(COLOR_GREEN "hello_word en vert\n" COLOR_RESET);
    printf(COLOR_BLUE "hello_word en bleue\n" COLOR_RESET);

    printf(TXT_BOLD COLOR_RED "hello_word en rouge et gras\n"
           TXT_FMT_RESET COLOR_RESET);

    printf("hello_word normal\n\n");

    logger_info("message info: %s\n", "le message");
    logger_warning("message warning: %s\n", "le message");
    logger_error("message error: %s\n", "le message");
    /* will create a core dump */
    //logger_fatal("message fatal: %s\n", "le message");

    printf("%s", COLOR_BLUE);
    printf("ici\n");
    printf("%smesssage\n" COLOR_RESET, COLOR_RED);

    return 0;
}
