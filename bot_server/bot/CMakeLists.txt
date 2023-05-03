add_library(bot src/bot.cpp)

target_include_directories(bot PUBLIC include)

add_subdirectory(../../shared ../../build)

target_link_libraries(bot shared)
