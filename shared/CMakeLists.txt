add_library(shared src/shared.cpp)

target_include_directories(shared PUBLIC include)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

target_link_libraries( shared ${OpenCV_LIBS} )
