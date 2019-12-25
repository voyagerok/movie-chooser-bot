# CMakeList.txt : CMake project for movie-chooser-bot, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project("movie-chooser-bot")

set(CMAKE_CXX_STANDARD 17)

find_package(CURL REQUIRED)
find_package(Boost 1.65 REQUIRED)

set(SRC
    main.cpp
    config.h
    config.cpp
    tg-api.h
    tg-api.cpp
    http.h
    http.cpp
    log.h
    log.cpp
    json_serializers.cpp
    json_serializers.h
    json.hpp
    tmdbapi.cpp
    tmdbapi.h
    threadpool.cpp
    threadpool.h
    epollwrapper.cpp
    epollwrapper.h
    timer.cpp
    timer.h)

# Add source to this project's executable.
add_executable (${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES} Boost::headers pthread)

