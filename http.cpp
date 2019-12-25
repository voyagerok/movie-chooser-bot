#include "http.h"

#include <curl/curl.h>
#include <iostream>
#include <memory>
#include <sstream>

#include "log.h"

namespace {

using CurlHandle = std::unique_ptr<CURL, void (*)(CURL *)>;

CurlHandle createCurlHandle()
{
    return CurlHandle(curl_easy_init(), curl_easy_cleanup);
}

size_t write_cb(char *p, size_t size, size_t nmemb, void *userdata)
{
    auto data = static_cast<std::string *>(userdata);
    data->append(p, nmemb);
    return nmemb;
}

std::string escapeString(const CurlHandle &handle, const std::string &url)
{
    char *s = curl_easy_escape(handle.get(), url.c_str(), url.size());
    std::string res(s);
    curl_free(s);
    return res;
}

std::string appendUrlQuery(const CurlHandle &handle,
                           const std::string &url,
                           const Http::Options &options)
{
    std::string newUrl(url);
    if (!options.empty()) {
        newUrl += '?';
        for (const auto &opt : options) {
            newUrl += opt.first;
            newUrl += '=';
            newUrl += escapeString(handle, opt.second);
            newUrl += '&';
        }
        newUrl.erase(newUrl.end() - 1);
    }
    return newUrl;
}

int sock_cb(CURL *e, curl_socket_t s, int what, void *cbp, void *sockp) {}
int multi_timer_cb(CURLM *multi, long timeout_ms, void *userinfo) {}

} // namespace

std::string Http::get(const std::string &url, const Options &options)
{
    LOGTRACE << "GET " << url;
    auto handle = createCurlHandle();
    std::string responseData;
    curl_easy_setopt(handle.get(), CURLOPT_WRITEDATA, &responseData);
    curl_easy_setopt(handle.get(), CURLOPT_WRITEFUNCTION, write_cb);

    if (options.empty()) {
        curl_easy_setopt(handle.get(), CURLOPT_URL, url.c_str());
    } else {
        std::string fullUrl = appendUrlQuery(handle, url, options);
        curl_easy_setopt(handle.get(), CURLOPT_URL, fullUrl.c_str());
    }

    curl_easy_setopt(handle.get(), CURLOPT_HTTPGET, 1);
    auto rc = curl_easy_perform(handle.get());
    if (rc != CURLE_OK) {
        std::ostringstream oss;
        oss << "GET " << url << " failed: " << curl_easy_strerror(rc);
        throw std::runtime_error(oss.str());
    }

    return responseData;
}

HttpSession::HttpSession(EpollWrapper &epoll, Timer &timer)
    : m_multi(curl_multi_init()), m_epoll(epoll), m_timer(timer)
{}

void HttpSession::get(const std::string &url, const Http::Options &options, Http::Callback callback)
{
    auto handle = createCurlHandle();
    std::string responseData;
    curl_easy_setopt(handle.get(), CURLOPT_WRITEDATA, &responseData);
    curl_easy_setopt(handle.get(), CURLOPT_WRITEFUNCTION, write_cb);

    if (options.empty()) {
        curl_easy_setopt(handle.get(), CURLOPT_URL, url.c_str());
    } else {
        std::string fullUrl = appendUrlQuery(handle, url, options);
        curl_easy_setopt(handle.get(), CURLOPT_URL, fullUrl.c_str());
    }

    curl_easy_setopt(handle.get(), CURLOPT_HTTPGET, 1);
}
