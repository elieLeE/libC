#include <ctype.h>
#include <string.h>

#include "str.h"

void upper_string(char *str)
{
    for (unsigned int i = 0; i < strlen(str); i++) {
        str[i] = toupper(str[i]);
    }
}

void lower_string(char *str)
{
    for (unsigned int i = 0; i < strlen(str); i++) {
        str[i] = tolower(str[i]);
    }
}

