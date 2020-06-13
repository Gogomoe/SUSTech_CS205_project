cmake_minimum_required(VERSION 3.15)
project(CS205_project)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(src DIR_SRC)

find_package(OpenCV REQUIRED)
include_directories(
        ${PROJECT_SOURCE_DIR}
        "/usr/local/include/opencv4/opencv2"
)

add_executable(CS205_project ${DIR_SRC})
target_link_libraries(CS205_project ${OpenCV_LIBS})