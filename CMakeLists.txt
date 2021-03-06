cmake_minimum_required(VERSION 3.10)
project(matrix)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(src DIR_SRC)
aux_source_directory(test DIR_TEST)

find_package(OpenCV REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(
        ${PROJECT_SOURCE_DIR}
        ${OpenCV_INCLUDE_DIRS}
)

add_executable(matrix ${DIR_SRC} ${DIR_TEST} src/Complex.cpp test/matrixArithmeticTest.cpp test/matrixTransportTest.cpp test/test.hpp)
target_link_libraries(matrix PRIVATE ${OpenCV_LIBS})
target_link_libraries(matrix PRIVATE Eigen3::Eigen)

if(${DEBUG})
        target_compile_options(matrix PRIVATE -g)
else()
        if(${OPENMP})
                find_package(OpenMP REQUIRED)
                if(OpenMP_CXX_FOUND)
                        target_link_libraries(matrix PUBLIC OpenMP::OpenMP_CXX)
                endif()
        endif()

        include(CheckCXXCompilerFlag)
        check_cxx_compiler_flag("-O2 -march=native -ftree-vectorize -fopt-info-vec-optimized" _autovec_gcc)
        check_cxx_compiler_flag("-O2 -march=native -Rpass-analysis=loop-vectorize -Rpass=loop-vectorize -Rpass-missed=loop-vectorize" _autovec_clang)
        if(_autovec_gcc)
                message(STATUS "Auto-Vectorization Enabled")
                target_compile_options(matrix PRIVATE -O2 -march=native -ftree-vectorize -fopt-info-vec-optimized)
        elseif(_autovec_clang)
                message(STATUS "Auto-Vectorization Enabled")
                target_compile_options(matrix PRIVATE -O2 -march=native -Rpass=loop-vectorize -Rpass-analysis=loop-vectorize)
        else()
                message(STATUS "No suitable compiler flag found for vectorization")
        endif()
endif()

target_compile_options(matrix PRIVATE -Wall)

include_directories(webserver/include)
SET(CMAKE_CXX_FLAGS -pthread)

aux_source_directory(webserver/src WEB_SRC)
add_executable(web_server ${WEB_SRC} src/matrix.cpp)
target_link_libraries(web_server PRIVATE ${OpenCV_LIBS})
target_link_libraries(web_server PRIVATE Eigen3::Eigen)
