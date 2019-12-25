#ifndef TIMER_H
#define TIMER_H

#include <boost/noncopyable.hpp>
#include <cstdint>

class Timer : private boost::noncopyable
{
public:
    Timer();
    ~Timer();

    void start(int64_t msec);
    void stop();
    int fd() { return m_fd; }

private:
    int m_fd = -1;
};

#endif // TIMER_H
