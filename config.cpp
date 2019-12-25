#include "config.h"

void Config::setTelegramBotApiKey(const std::string &key)
{
    m_telegramBotApiKey = key;
}

const std::string &Config::telegramBotApiKey() const
{
    return m_telegramBotApiKey;
}

void Config::setTmdbApiKey(const std::string &key)
{
    m_tdbApiKey = key;
}

const std::string &Config::tmdbApiKey() const
{
    return m_tdbApiKey;
}
