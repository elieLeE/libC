#ifndef __COLORS_H__
#define __COLORS_H__

/**
 * Using of color: printf(COLOR_* " Warning\n" COLOR_RESET");
*/
#define COLOR_RED "\x1B[31m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_YELLOW "\x1B[33m"
#define COLOR_BLUE "\x1B[34m"
#define COLOR_MAGENTA "\x1B[35m"
#define COLOR_WHITE "\x1B[37m"

#define COLOR_RESET "\x1b[0m"

#define TXT_BOLD "\e[1m"
#define TXT_FMT_RESET "\e[1"

#endif
