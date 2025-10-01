#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "logger.h"

static const char *get_fmt_text_lpg(logger_level_t level)
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
    return "";
}

__attribute ((format (printf, 4, 5)))
void logger_log(const char *file, int line, logger_level_t level,
                const char *fmt, ...)
{
    time_t now;
    struct tm *time_infos;
    char time_buf[50];
    size_t rc;
    va_list va;
    char buf[100];
    const char *txt_fmt;

    memset(time_buf, 0, 50);
    memset(buf, 0, 100);

    now = time(NULL);
    time_infos = localtime(&now);
    rc = snprintf(time_buf, sizeof(time_buf), "%02d/%02d/%04d %02d:%02d",
                  time_infos->tm_mday, time_infos->tm_mon + 1,
                  time_infos->tm_year + 1900, time_infos->tm_hour,
                  time_infos->tm_min);
    snprintf(time_buf + rc, sizeof(time_buf) - rc, ".%06ld", now / 1000);

    va_start(va, fmt);
    vsnprintf(buf, sizeof(buf), fmt, va);
    va_end(va);

    txt_fmt = get_fmt_text_lpg(level);
    printf(COLOR_WHITE "%s:%d %s -- %s%s" COLOR_RESET,
           file, line, time_buf, txt_fmt, buf);

    if (level == LOGGER_FATAL) {
        abort();
    }
}
