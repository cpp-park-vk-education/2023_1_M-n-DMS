add_library(head src/head.cpp)

target_include_directories(head PUBLIC include)

add_subdirectory(../../shared ../../build)

target_link_libraries(head shared)
