add_library(parking src/parking.cpp)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(parking ${GTEST_LIBRARIES} pthread)


target_include_directories(parking PUBLIC include)

add_subdirectory(../../shared ../../build)

target_link_libraries(parking shared)
