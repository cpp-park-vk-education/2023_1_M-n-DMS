cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_FLAGS "-Wall -Wpedantic -Wextra ${CMAKE_CXX_FLAGS}")

project(Parktronic VERSION 0.1.0)

# Set the SERVER_TYPE variable based on the server we're running on
set(SERVER_TYPE "bot" CACHE STRING "Type of server to build for")

# Conditionally add the appropriate executable when passing -DSERVER_TYPE=
if(SERVER_TYPE STREQUAL "bot")
    find_package(Threads REQUIRED)
    find_package(OpenSSL REQUIRED)
    find_package(Boost COMPONENTS system REQUIRED)
    find_package(CURL)
    include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
    if (CURL_FOUND)
        include_directories(${CURL_INCLUDE_DIRS})
        add_definitions(-DHAVE_CURL)
    endif()

    add_executable(bot_exec ${CMAKE_CURRENT_SOURCE_DIR}/bot_server/main.cpp)
    add_subdirectory(bot_server/bot)
    target_link_libraries(bot_exec bot)
elseif(SERVER_TYPE STREQUAL "head")
    add_executable(head_exec ${CMAKE_CURRENT_SOURCE_DIR}/head_server/main.cpp)
    add_subdirectory(head_server/head)
    target_link_libraries(head_exec head)
elseif(SERVER_TYPE STREQUAL "parking")
    add_executable(parking_exec ${CMAKE_CURRENT_SOURCE_DIR}/parking_server/main.cpp)
    add_subdirectory(parking_server/parking)
    target_link_libraries(parking_exec parking)
endif()
