#ifndef EPOLLWRAPPER_H
#define EPOLLWRAPPER_H

#include <boost/noncopyable.hpp>

class EpollWrapper : private boost::noncopyable
{
public:
    EpollWrapper();
    ~EpollWrapper();

    void add_fd(int fd);

private:
    int m_fd = -1;
};

#endif // EPOLLWRAPPER_H
