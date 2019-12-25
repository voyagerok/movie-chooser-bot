#ifndef TMDBAPI_H
#define TMDBAPI_H

#include "config.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class TmdbApi
{
public:
    struct ApiConfiguration
    {
        std::string base_url;
        std::string secure_base_url;
        std::vector<std::string> backdrop_sizes;
        std::vector<std::string> logo_sizes;
        std::vector<std::string> poster_sizes;
        std::vector<std::string> profile_sizes;
    };

    struct Movie
    {
        int64_t id;
        std::string title;
        std::string release_date;
        std::optional<std::string> poster_path;
    };

    TmdbApi(const Config &config);
    std::vector<Movie> search(const std::string &query, const std::string &lang);

private:
    std::unordered_map<int, std::string> getGenres();
    ApiConfiguration getApiConfig();

private:
    const std::string m_apiKey;
    std::optional<std::unordered_map<int, std::string>> m_genres;
    std::optional<ApiConfiguration> m_apiConfig;
};

#endif // TMDBAPI_H
