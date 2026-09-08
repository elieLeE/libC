#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdbool.h>

typedef enum logger_level_t {
    LOGGER_TRACE,
    LOGGER_DEBUG,
    LOGGER_INFO,
    LOGGER_WARNING,
    LOGGER_ERROR,
    LOGGER_FATAL
} logger_level_t;

__attribute ((format (printf, 4, 5)))
void logger_log(const char *file, int line, int level, const char *fmt, ...);

__attribute ((format (printf, 3, 4)))
void _logger_assert_failed(const char *file, int line, const char *fmt, ...);

#define logger_trace(fmt, ...)                                                \
    logger_log(__FILE__, __LINE__, LOGGER_TRACE, fmt, ##__VA_ARGS__)

#define logger_debug(fmt, ...)                                                \
    logger_log(__FILE__, __LINE__, LOGGER_DEBUG, fmt, ##__VA_ARGS__)

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

#endif


