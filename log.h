#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <ostream>
#include <sstream>

enum class LogLevel { ERROR = 1, WARNING, TRACE, DEBUG };

inline std::string logLevelToStdString(LogLevel level)
{
    switch (level) {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::ERROR:
        return "ERROR";
    case LogLevel::TRACE:
        return "TRACE";
    case LogLevel::WARNING:
        return "WARN";
    }
}

class LogSink
{
public:
    LogSink(std::ostream &os) : m_os(os) {}

    ~LogSink() { m_os << m_oss.str() << std::endl; }

    template<class T>
    LogSink &operator<<(T &&val)
    {
        m_oss << std::forward<T>(val);
        return *this;
    }

private:
    std::ostream &m_os;
    std::ostringstream m_oss;
};

class LogManager
{
public:
    static LogManager &instance();
    void setLogLevel(LogLevel level) { m_logLevel = level; }
    LogLevel logLevel() const { return m_logLevel; }
    static std::string header(LogLevel level, const char *file, const char *func, int line);

private:
    LogManager() : m_logLevel(LogLevel::ERROR) {}

private:
    LogLevel m_logLevel;
};

#define LOGWITHLEVEL(level) \
    for (int l = static_cast<int>(LogManager::instance().logLevel()); \
         l >= static_cast<int>(level); \
         l = 0) \
    LogSink(std::cerr) << LogManager::header(static_cast<LogLevel>(l), \
                                             __FILE__, \
                                             __FUNCTION__, \
                                             __LINE__)

#define LOGERROR LOGWITHLEVEL(LogLevel::ERROR)
#define LOGWARNING LOGWITHLEVEL(LogLevel::WARNING)
#define LOGTRACE LOGWITHLEVEL(LogLevel::TRACE)
#define LOGDEBUG LOGWITHLEVEL(LogLevel::DEBUG)

#endif // !LOG_H
