cmake_minimum_required(VERSION 3.22)
project(final_project)

set(CMAKE_CXX_STANDARD 20)

set(OpenCV_DIR C:/opencv/MinGW-build)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

aux_source_directory(src SRC_FILES)

add_executable(final_project main.cpp ${SRC_FILES})
add_executable(test test.cpp ${SRC_FILES})

find_package(OpenCV REQUIRED)
target_include_directories(final_project PRIVATE include third_party)
target_include_directories(test PRIVATE include third_party)
target_link_libraries(final_project ${OpenCV_LIBS} wsock32)
target_link_libraries(test ${OpenCV_LIBS} wsock32)
