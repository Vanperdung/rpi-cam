#include <cstdarg> // variadic helpers
#include <cstdio> // vsnprintf

#include <RPI/log.h>

using namespace RPI;

static eLogSeverity sLogLevel = RPI_LOG_INFO;

static inline const char *severityToString(eLogSeverity level)
{
    switch (level)
    {
    case RPI_LOG_ERROR:
        return "[ERROR]";
        break;
    case RPI_LOG_WARNING:
        return "[WARNING]";
        break;
    case RPI_LOG_INFO:
        return "[INFO]";
        break;
    case RPI_LOG_DEBUG:
        return "[DEBUG]";
        break;
    default:
        return "[INVALID]";
        break;
    }
}

void RPI::logging(eLogSeverity level, const char *file, const int line, const char *fmt, ...)
{
    va_list args;
    char buf[1024] = {};

    va_start(args, fmt);

    if (level <= sLogLevel && (vsnprintf(buf, sizeof(buf), fmt, args) > 0))
        printf("%s-%s:%d: %s\n", severityToString(level), file, line, buf);

    va_end(args);
}