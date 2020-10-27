cmake_minimum_required(VERSION 3.8.2)
project(line_based_writers_benchmarks)

include(cmake/googlebenchmark.cmake)

file(GLOB BENCHMARKS_SRC "src/*.cpp")

add_executable(${PROJECT_NAME} ${BENCHMARKS_SRC})
target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME} ${3RDPARTY_LIBRARIES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

