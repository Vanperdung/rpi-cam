#pragma once

namespace RPI
{

enum eLogSeverity
{
    RPI_LOG_ERROR = 0, 
    RPI_LOG_WARNING = 1,
    RPI_LOG_INFO = 2,
    RPI_LOG_DEBUG = 3,
    RPI_LOG_INVALID = 4
};

/**
 * @brief Core logging function (printf-style)
 * 
 * @param level Log severity (RPI_LOG_ERROR, RPI_LOG_WARNING, ...)
 * @param file  Current filename is calling logging
 * @param line  The number of line is calling logging
 * @param fmt   printf-style format string
 * @param ...   Additional arguments matching fmt
 */
void logging(eLogSeverity level, const char *file, const int line, const char *fmt, ...);

#define LOGGING(level, fmt, ...) \
    logging(level, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

} // namespace RPI