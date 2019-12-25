// movie-chooser-bot.cpp : Defines the entry point for the application.
//

#include <atomic>
#include <fstream>
#include <getopt.h>
#include <iostream>
#include <signal.h>
#include <string>

#include "config.h"
#include "log.h"
#include "tg-api.h"
#include "threadpool.h"

volatile std::atomic_bool interrupt_flag = false;

void signal_handler(int signum)
{
    LOGTRACE << "interrupt requested by signal " << signum;
    interrupt_flag.store(true, std::memory_order_relaxed);
}

bool parse_cmd(int argc, char *argv[], Config &config)
{
    bool success = true;
    int opt;
    while ((opt = getopt(argc, argv, "t:d:")) != -1) {
        switch (opt) {
        case 't':
            config.setTelegramBotApiKey(optarg);
            break;
        case 'd':
            config.setTmdbApiKey(optarg);
            break;
        default:
            success = false;
            break;
        }
    }
    return success;
}

int main(int argc, char *argv[])
{
    LogManager::instance().setLogLevel(LogLevel::TRACE);

    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " -t <tg bot api key> -d <tmdb api key>" << std::endl;
        return 1;
    }
    Config appConfig;
    if (!parse_cmd(argc, argv, appConfig)) {
        std::cerr << "usage: " << argv[0] << " -t <tg bot api key> -d <tmdb api key>" << std::endl;
        return 1;
    }

    struct sigaction act = {};
    act.sa_handler = signal_handler;
    sigaction(SIGINT, &act, nullptr);
    sigaction(SIGTERM, &act, nullptr);

    try {
        TelegramBotApi api(appConfig);
        int64_t lastUpdateId = 0;
        while (!interrupt_flag.load(std::memory_order_relaxed)) {
            auto updatesTask = api.getUpdates(lastUpdateId);
            auto updates = updatesTask.get();
            for (size_t i = 0; i < updates.size(); ++i) {
                if (auto updateBody = updates[i].body) {
                    LOGTRACE << "QUERY: " << updateBody->query
                             << ", user name: " << updateBody->user.firstName << " "
                             << updateBody->user.lastName.value_or("")
                             << ", username: " << updateBody->user.username.value_or("N/A");
                }
            }
            if (!updates.empty()) { lastUpdateId = updates[updates.size() - 1].id; }
        }
    } catch (const TelegramBotApi::Error &e) {
        LOGERROR << "TelegramBotApi exception: " << e.what();
    } catch (const std::exception &e) {
        LOGERROR << "exception: " << e.what();
    } catch (...) {
        LOGERROR << "unknwon exception";
    }

    return 0;
}
