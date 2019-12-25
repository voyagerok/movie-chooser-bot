#ifndef CONFIG_H
#define CONFIG_H

#include <string>

class Config
{
public:
    void setTelegramBotApiKey(const std::string &key);
    const std::string &telegramBotApiKey() const;

    void setTmdbApiKey(const std::string &key);
    const std::string &tmdbApiKey() const;

private:
    std::string m_telegramBotApiKey;
    std::string m_tdbApiKey;
};

#endif // !CONFIG_H
