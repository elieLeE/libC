#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdbool.h>

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

typedef enum logger_level_t {
    LOGGER_INFO,
    LOGGER_WARNING,
    LOGGER_ERROR,
    LOGGER_FATAL
} logger_level_t;

__attribute ((format (printf, 4, 5)))
void logger_log(const char *file, int line, logger_level_t level,
                const char *fmt, ...);

__attribute ((format (printf, 3, 4)))
void _logger_assert_failed(const char *file, int line, const char *fmt, ...);

#define logger_info(fmt, ...)                                                 \
    logger_log(__FILE__, __LINE__, LOGGER_INFO, fmt, ##__VA_ARGS__)

#define logger_warning(fmt, ...)                                              \
    logger_log(__FILE__, __LINE__, LOGGER_WARNING, fmt, ##__VA_ARGS__)

#define logger_error(fmt, ...)                                                \
    logger_log(__FILE__, __LINE__, LOGGER_ERROR, fmt, ##__VA_ARGS__)

#define logger_fatal(fmt, ...)                                                \
    logger_log(__FILE__, __LINE__, LOGGER_FATAL, fmt, ##__VA_ARGS__)

#define logger_assert_failed(fmt, ...)                                        \
    _logger_assert_failed(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

void logger_test_begin_module(const char *test_name);
void logger_test_start(const char *test_name);
void logger_test_result(const char *test_name, bool res);
void logger_test_end_module(const char *test_name);

#define logger_test_ok(_test_name)                                            \
    logger_test_result(_test_name, true);

#endif


