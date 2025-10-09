#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "logger.h"

static const char *get_fmt_text_log(logger_level_t level)
{
    switch (level) {
    case LOGGER_INFO:
        return "";
    case LOGGER_WARNING:
        return COLOR_YELLOW;
    case LOGGER_ERROR:
    case LOGGER_FATAL:
        return COLOR_RED TXT_BOLD;
    }

    logger_fatal("invalid logger level: %d", level);
    return "";
}

static const char *get_level_str(logger_level_t level)
{
    switch (level) {
    case LOGGER_INFO:
        return "INFO";
    case LOGGER_WARNING:
        return "WARNING";
    case LOGGER_ERROR:
        return "ERROR";
    case LOGGER_FATAL:
        return "FATAL";
    }

    logger_fatal("invalid logger level: %d", level);
    return "";
}

static void
logger_log_(const char *file, int line, logger_level_t level, const char *txt)
{
    time_t now;
    struct tm *time_infos;
    char time_buf[50];
    size_t rc;
    const char *txt_fmt;
    const char *level_txt;

    memset(time_buf, 0, 50);

    now = time(NULL);
    time_infos = localtime(&now);
    rc = snprintf(time_buf, sizeof(time_buf), "%02d/%02d/%04d %02d:%02d",
                  time_infos->tm_mday, time_infos->tm_mon + 1,
                  time_infos->tm_year + 1900, time_infos->tm_hour,
                  time_infos->tm_min);
    snprintf(time_buf + rc, sizeof(time_buf) - rc, ".%06ld", now / 1000);

    level_txt = get_level_str(level);
    txt_fmt = get_fmt_text_log(level);
    printf(COLOR_WHITE "%s:%d %s -- %s%s: %s" COLOR_RESET,
           file, line, time_buf, txt_fmt, level_txt, txt);

    if (txt[strlen(txt) - 1] != '\n') {
        printf("\n");
    }

    if (level == LOGGER_FATAL) {
        abort();
    }
}

__attribute ((format (printf, 4, 5)))
void logger_log(const char *file, int line, logger_level_t level,
                const char *fmt, ...)
{
    char buf[100];
    va_list va;

    memset(buf, 0, 100);

    va_start(va, fmt);
    vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);

    logger_log_(file, line, level, buf);
}

__attribute ((format (printf, 3, 4)))
void _logger_assert_failed(const char *file, int line, const char *fmt, ...)
{
    char buf[100] = "ASSERT FAILED => ";
    va_list va;

    va_start(va, fmt);
    vsnprintf(buf + strlen(buf), sizeof(buf), fmt, va);
    va_end(va);

    logger_log_(file, line, LOGGER_ERROR, buf);
}

void logger_test_begin_module(const char *module_name)
{
    printf("Run all tests of module " COLOR_MAGENTA "%s\n" COLOR_RESET,
           module_name);
}

void logger_test_end_module(const char *module_name)
{
    printf("Leave module " COLOR_MAGENTA "%s\n" COLOR_RESET,
           module_name);
}
void logger_test_start(const char *test_name)
{
    printf("\tSTART TEST " COLOR_BLUE "%s\n" COLOR_RESET, test_name);
}

void logger_test_result(const char *test_name, bool res)
{
    const char *txt_fmt, *txt_res;

    if (res) {
        txt_res = "OK";
        txt_fmt = COLOR_GREEN;
    } else {
        txt_res = "FAILED";
        txt_fmt = COLOR_RED;
    }

    printf("\t%s%s: %s\n" COLOR_RESET, test_name, txt_fmt, txt_res);
}

