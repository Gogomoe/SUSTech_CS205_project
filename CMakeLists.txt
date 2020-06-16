cmake_minimum_required(VERSION 3.10)
project(matrix)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(src DIR_SRC)
aux_source_directory(test DIR_TEST)

find_package(OpenCV REQUIRED)
include_directories(
        ${PROJECT_SOURCE_DIR}
        ${OpenCV_INCLUDE_DIRS}
)

add_executable(matrix ${DIR_SRC} ${DIR_TEST} src/Complex.cpp test/matrixArithmeticTest.cpp test/matrixTransportTest.cpp test/test.hpp)
target_link_libraries(matrix PRIVATE ${OpenCV_LIBS})

if(${OPENMP})
        find_package(OpenMP REQUIRED)
        if(OpenMP_CXX_FOUND)
                target_link_libraries(matrix PUBLIC OpenMP::OpenMP_CXX)
        endif()
endif()

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-O2 -march=native -ftree-vectorize -fopt-info-vec" _autovec_gcc)
if(_autovec_gcc)
        message(STATUS "Auto-Vectorization Enabled")
        target_compile_options(matrix PRIVATE -O2 -march=native -ftree-vectorize -fopt-info-vec)
else()
        message(STATUS "No suitable compiler flag found for vectorization")
endif()

target_compile_options(matrix PRIVATE -Wall)

include_directories(webserver/include)
SET(CMAKE_CXX_FLAGS -pthread)

aux_source_directory(webserver/src WEB_SRC)
add_executable(web_server ${WEB_SRC} src/matrix.cpp)
target_link_libraries(web_server PRIVATE ${OpenCV_LIBS})
