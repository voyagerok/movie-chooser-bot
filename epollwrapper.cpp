#include "epollwrapper.h"

#include <cstring>
#include <sstream>
#include <sys/epoll.h>
#include <unistd.h>

thread_local char errbuf[512];

EpollWrapper::EpollWrapper()
{
    m_fd = epoll_create1(0);
    if (m_fd == -1) {
        std::ostringstream oss;
        oss << "epoll_create1 failed: " << strerror_r(errno, errbuf, sizeof(errbuf));
        throw std::runtime_error(oss.str());
    }
}

EpollWrapper::~EpollWrapper()
{
    close(m_fd);
}

void EpollWrapper::add_fd(int fd)
{
    epoll_ctl(m_fd, EPOLL_CTL_ADD, fd, nullptr);
}
