#include "tg-api.h"

#include "http.h"
#include "json.hpp"
#include "json_serializers.h"
#include "log.h"

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <sstream>

namespace nl = nlohmann;

TelegramBotApi::Error::Error(const std::string &msg) : m_msg(msg) {}

const char *TelegramBotApi::Error::what() const noexcept
{
    return m_msg.c_str();
}

TelegramBotApi::TelegramBotApi(const Config &config)
    : m_apiUrlBase(std::string("https://api.telegram.org/bot") + config.telegramBotApiKey()),
      m_pool(ThreadPool::instance())
{}

std::vector<TelegramBotApi::Update> TelegramBotApi::getUpdatesImpl(std::string apiUrl,
                                                                   int64_t lastUpdateId)
{
    std::string resp = Http::get(apiUrl,
                                 {{"timeout", "30"},
                                  {"offset", boost::lexical_cast<std::string>(lastUpdateId + 1)}});

    std::vector<Update> queries;
    try {
        nl::json root = nl::json::parse(resp);
        auto updatesResp = root.get<UpdatesResponse>();
        if (std::holds_alternative<std::string>(updatesResp)) {
            auto errResp = std::get<std::string>(updatesResp);
            LOGERROR << errResp;
            std::ostringstream oss;
            oss << "got error response from getUpdates request: " << errResp;
            throw Error(oss.str());
        }
        return std::get<UpdateList>(updatesResp);
    } catch (const nl::json::parse_error &e) {
        LOGERROR << "error during response parsing: " << e.what();
        std::ostringstream oss;
        oss << "could not parse response from getUpdates request: " << e.what();
        throw Error(oss.str());
    } catch (const nl::json::type_error &e) {
        LOGERROR << "type error during response parsing: " << e.what();
        std::ostringstream oss;
        oss << "could not parse response from getUpdates request: " << e.what();
        throw Error(oss.str());
    }
}

std::future<std::vector<TelegramBotApi::Update>> TelegramBotApi::getUpdates(int64_t lastUpdateId)
{
    static std::string apiUrl = m_apiUrlBase + "/getUpdates";
    return m_pool.add(&TelegramBotApi::getUpdatesImpl, apiUrl, lastUpdateId);
}
