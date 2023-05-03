add_library(parking src/parking.cpp)

target_include_directories(parking PUBLIC include)

add_subdirectory(../../shared ../../build)

target_link_libraries(parking shared)
