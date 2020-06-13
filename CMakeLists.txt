cmake_minimum_required(VERSION 3.10)
project(CS205_project)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(src DIR_SRC)
aux_source_directory(test DIR_TEST)

find_package(OpenCV REQUIRED)
include_directories(
        ${PROJECT_SOURCE_DIR}
        ${OpenCV_INCLUDE_DIRS}
)

add_executable(CS205_project ${DIR_SRC} ${DIR_TEST})
target_link_libraries(CS205_project PRIVATE ${OpenCV_LIBS})