add_library(head src/head.cpp)

target_include_directories(head PUBLIC include)

add_subdirectory(../../shared ../../build)

find_package(libpqxx REQUIRED)

include_directories(${LIBPQXX_INCLUDE_DIRS})

target_link_libraries(head shared ${LIBPQXX_LIBRARIES} -lpqxx -lpq)
