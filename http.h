#ifndef HTTP_H
#define HTTP_H

#include <curl/curl.h>
#include <functional>
#include <future>
#include <string>
#include <vector>

#include "epollwrapper.h"
#include "timer.h"

class Http
{
public:
    using Option = std::pair<std::string, std::string>;
    using Options = std::vector<Option>;
    using Callback = std::function<void(std::future<std::string>)>;
    static std::string get(const std::string &url, const Options &options = Options());
};

class HttpSession
{
public:
    HttpSession(EpollWrapper &epollWrapper, Timer &timer);
    ~HttpSession();

    void get(const std::string &url, const Http::Options &options, Http::Callback callback);
    void get(const std::string &url, Http::Callback callback);

private:
    CURLM *m_multi;
    EpollWrapper &m_epoll;
    Timer &m_timer;
};

#endif // !HTTP_H
