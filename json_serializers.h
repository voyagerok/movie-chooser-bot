#ifndef JSON_SERIALIZERS_H
#define JSON_SERIALIZERS_H

#include "json.hpp"
#include "tg-api.h"

void from_json(const nlohmann::json &j, TelegramBotApi::User &user);
void from_json(const nlohmann::json &j, TelegramBotApi::Update &update);
void from_json(const nlohmann::json &j, TelegramBotApi::InlineQuery &query);
void from_json(const nlohmann::json &j, TelegramBotApi::UpdatesResponse &resp);

#endif // JSON_SERIALIZERS_H
