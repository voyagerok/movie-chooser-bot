#include "timer.h"

#include <cstring>
#include <sstream>
#include <sys/timerfd.h>
#include <unistd.h>

#include "log.h"

thread_local char errbuf[512];

Timer::Timer()
{
    m_fd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (m_fd == -1) {
        char *err = strerror_r(errno, errbuf, sizeof(errbuf));
        LOGERROR << err;
        std::ostringstream oss;
        oss << "timerfd_create failed: " << err;
        throw std::runtime_error(oss.str());
    }
}

Timer::~Timer()
{
    struct itimerspec its;
    memset(&its, 0, sizeof(struct itimerspec));
    timerfd_settime(m_fd, 0, &its, nullptr);
    close(m_fd);
}

void Timer::start(int64_t msec)
{
    struct itimerspec its;
    if (msec > 0) {
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 0;
        its.it_value.tv_sec = msec / 1000;
        its.it_value.tv_nsec = (msec % 1000) * 1000 * 1000;
    } else {
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 0;
        its.it_value.tv_sec = 0;
        its.it_value.tv_nsec = 1;
    }
    if (timerfd_settime(m_fd, 0, &its, nullptr) == -1) {
        char *err = strerror_r(errno, errbuf, sizeof(errbuf));
        LOGERROR << err;
        std::ostringstream oss;
        oss << "timerfd_settime failed: " << err;
        throw std::runtime_error(oss.str());
    }
}

void Timer::stop()
{
    struct itimerspec its;
    memset(&its, 0, sizeof(struct itimerspec));
    if (timerfd_settime(m_fd, 0, &its, nullptr) == -1) {
        char *err = strerror_r(errno, errbuf, sizeof(errbuf));
        LOGERROR << err;
        std::ostringstream oss;
        oss << "timerfd_settime failed: " << err;
        throw std::runtime_error(oss.str());
    }
}
