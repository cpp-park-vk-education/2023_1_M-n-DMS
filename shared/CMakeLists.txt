add_library(shared src/shared.cpp)

target_include_directories(shared PUBLIC include)

find_package(OpenCV REQUIRED)

find_package(GTest CONFIG REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${GTest_INCLUDE_DIRS})

target_link_libraries(shared ${GTest_LIBS} ${OpenCV_LIBS} -pthread -lgtest -lgtest_main)
