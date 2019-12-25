#ifndef TG_API_H
#define TG_API_H

#include "config.h"

#include <future>
#include <optional>
#include <variant>
#include <vector>

#include "threadpool.h"

class TelegramBotApi
{
public:
    class Error : public std::exception
    {
    public:
        Error(const std::string &msg);
        const char *what() const noexcept override;

    private:
        std::string m_msg;
    };

    struct User
    {
        int64_t id;
        bool isBot;
        std::string firstName;
        std::optional<std::string> lastName;
        std::optional<std::string> username;
        std::optional<std::string> lang;
    };

    struct InlineQuery
    {
        std::string id;
        User user;
        std::string query;
    };

    struct Update
    {
        int64_t id;
        std::optional<InlineQuery> body;
    };

    using UpdateList = std::vector<Update>;

    using UpdatesResponse = std::variant<std::string, UpdateList>;

    TelegramBotApi(const Config &config);
    std::future<std::vector<Update>> getUpdates(int64_t lastUpdateId = 0);

private:
    static std::vector<Update> getUpdatesImpl(std::string apiUrl, int64_t lastUpdateId);

private:
    const std::string m_apiUrlBase;
    ThreadPool &m_pool;
};

#endif // !TG_API_H
