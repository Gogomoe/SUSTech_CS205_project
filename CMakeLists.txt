cmake_minimum_required(VERSION 3.15)
project(CS205_project)

set(CMAKE_CXX_STANDARD 14)

aux_source_directory(src DIR_SRC)

add_executable(CS205_project ${DIR_SRC})