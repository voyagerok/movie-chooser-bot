#include "json_serializers.h"

using namespace nlohmann;

void from_json(const nlohmann::json &j, TelegramBotApi::User &user)
{
    j.at("id").get_to(user.id);
    j.at("is_bot").get_to(user.isBot);
    j.at("first_name").get_to(user.firstName);
    if (auto it = j.find("last_name"); it != j.end()) {
        user.lastName = it->get<std::string>();
    }
    if (auto it = j.find("username"); it != j.end()) {
        user.username = it->get<std::string>();
    }
    if (auto it = j.find("language_code"); it != j.end()) {
        user.lang = it->get<std::string>();
    }
}

void from_json(const nlohmann::json &j, TelegramBotApi::InlineQuery &query)
{
    j.at("id").get_to(query.id);
    j.at("from").get_to(query.user);
    j.at("query").get_to(query.query);
}

void from_json(const nlohmann::json &j, TelegramBotApi::Update &update)
{
    j.at("update_id").get_to(update.id);
    if (auto it = j.find("inline_query"); it != j.end()) {
        update.body = it->get<TelegramBotApi::InlineQuery>();
    }
}

void from_json(const nlohmann::json &j, TelegramBotApi::UpdatesResponse &resp)
{
    bool ok = j.at("ok").get<bool>();
    if (ok) {
        resp = j.at("result").get<TelegramBotApi::UpdateList>();
    } else {
        resp = j.at("description").get<std::string>();
    }
}
