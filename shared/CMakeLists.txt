include(FetchContent)

add_library(shared src/shared.cpp)

target_include_directories(shared PUBLIC include)

find_package(OpenCV REQUIRED)

find_package(GTest CONFIG REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS} ${GTest_INCLUDE_DIRS})

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)

FetchContent_MakeAvailable(json)

target_link_libraries(shared ${GTest_LIBS} ${OpenCV_LIBS} -pthread -lgtest -lgtest_main nlohmann_json::nlohmann_json)
