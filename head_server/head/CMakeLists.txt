include(FetchContent)

add_library(head src/head.cpp)

target_include_directories(head PUBLIC include)

add_subdirectory(../../shared ../../build)

find_package(libpqxx REQUIRED)

include_directories(${LIBPQXX_INCLUDE_DIRS})

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)

FetchContent_MakeAvailable(json)

target_link_libraries(head nlohmann_json::nlohmann_json shared ${LIBPQXX_LIBRARIES} -lpqxx -lpq)
