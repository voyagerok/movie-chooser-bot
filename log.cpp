#include "log.h"

#include <cstring>

static inline const char *filename(const char *file)
{
    const char *subs = strrchr(file, '/');
    return subs ? subs + 1 : file;
}

LogManager &LogManager::instance()
{
    static LogManager instance;
    return instance;
}

std::string LogManager::header(LogLevel level, const char *file, const char *func, int line)
{
    std::ostringstream oss;
    oss << logLevelToStdString(level) << ' ' << func << "()"
        << " [" << filename(file) << ":" << line << "] ";
    return oss.str();
}
