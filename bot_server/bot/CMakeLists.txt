add_library(bot src/bot.cpp)

target_include_directories(bot PUBLIC include)

add_subdirectory(../../shared ../../build)

target_link_libraries(bot shared)

target_link_libraries(bot /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})
