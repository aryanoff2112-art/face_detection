cmake_minimum_required(VERSION 3.20)

project(face_detection)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

add_executable(face_detection main.cpp)

target_link_libraries(face_detection PRIVATE ${OpenCV_LIBS})

target_include_directories(face_detection PRIVATE ${OpenCV_INCLUDE_DIRS})
