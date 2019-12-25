#include "tmdbapi.h"

TmdbApi::TmdbApi(const Config &config) : m_apiKey(config.tmdbApiKey()) {}

std::vector<TmdbApi::Movie> TmdbApi::search(const std::string &query, const std::string &lang)
{
    //    if (!m_genres) {
    //        m_genres = getGenres();
    //    }
    //    if (!m_apiConfig) {
    //        m_apiConfig = getApiConfig();
    //    }
}
