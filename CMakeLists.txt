cmake_minimum_required(VERSION 3.20)

project(FaceDetection)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(FaceDetection
    main.cpp
    face_db.cpp
    matcher.cpp
    util.cpp
    enroll.cpp
    recognize.cpp
)

target_link_libraries(FaceDetection PRIVATE ${OpenCV_LIBS})

target_include_directories(FaceDetection PRIVATE ${OpenCV_INCLUDE_DIRS})