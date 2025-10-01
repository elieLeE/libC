#ifndef __LOGGER_H__
#define __LOGGER_H__

/**
 * Using of color: printf(COLOR_* " Warning\n" COLOR_RESET");
*/
#define COLOR_RED "\x1B[31m"
#define COLOR_GREEN "\x1B[32m"
#define COLOR_YELLOW "\x1B[33m"
#define COLOR_BLUE "\x1B[34m"
#define COLOR_WHITE "\x1B[37m"

#define COLOR_RESET "\x1b[0m"

#define TXT_BOLD "\e[1m"
#define TXT_FMT_RESET "\e[1"

typedef enum logger_level_t {
    LOGGER_INFO,
    LOGGER_WARNING,
    LOGGER_ERROR,
    LOGGER_FATAL
} logger_level_t;

__attribute ((format (printf, 4, 5)))
void logger_log(const char *file, int line, logger_level_t level,
                const char *fmt, ...);

#define logger_info(fmt, ...)                                                 \
    logger_log(__FILE__, __LINE__, LOGGER_INFO, fmt, ##__VA_ARGS__)

#define logger_warning(fmt, ...)                                              \
    logger_log(__FILE__, __LINE__, LOGGER_WARNING, fmt, ##__VA_ARGS__)

#define logger_error(fmt, ...)                                                \
    logger_log(__FILE__, __LINE__, LOGGER_ERROR, fmt, ##__VA_ARGS__)

#define logger_fatal(fmt, ...)                                                \
    logger_log(__FILE__, __LINE__, LOGGER_FATAL, fmt, ##__VA_ARGS__)

#endif


