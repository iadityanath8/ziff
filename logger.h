#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#ifndef LOG_DECL
#define LOG_DECL static inline
#endif


#define CLR_RESET  "\033[0m"
#define CLR_INFO   "\033[32m"
#define CLR_WARN   "\033[33m"
#define CLR_ERROR  "\033[31m"



#define log_info(...)   logger_info_impl(__FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)   logger_warn_impl(__FILE__, __LINE__, __VA_ARGS__)
#define log_error(...)  logger_error_impl(__FILE__, __LINE__, __VA_ARGS__)


LOG_DECL void logger_timestamp(void) {
    time_t t = time(NULL);
    struct tm tm;
    localtime_r(&t, &tm);
    printf("[%02d:%02d:%02d] ",
            tm.tm_hour, tm.tm_min, tm.tm_sec);
}


LOG_DECL void logger_vlog(const char *color, const char *level,
        const char *file, int line,
        const char *fmt, va_list ap)
{
    logger_timestamp();
    printf("%s[%s]%s %s:%d ",
            color, level, CLR_RESET, file, line);
    vprintf(fmt, ap);
    putchar('\n');
}


LOG_DECL void logger_info_impl(const char *file, int line,
        const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    logger_vlog(CLR_INFO, "INFO", file, line, fmt, ap);
    va_end(ap);
}

LOG_DECL void logger_warn_impl(const char *file, int line,
        const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    logger_vlog(CLR_WARN, "WARN", file, line, fmt, ap);
    va_end(ap);
}

LOG_DECL void logger_error_impl(const char *file, int line,
        const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    logger_vlog(CLR_ERROR, "ERROR", file, line, fmt, ap);
    va_end(ap);
}

#endif /* LOGGER_H */
