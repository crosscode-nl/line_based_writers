cmake_minimum_required(VERSION 3.8.2)
project(line_base_writers_tests
        VERSION 0.1.0
        LANGUAGES C CXX
        )

list(APPEND ${PROJECT_NAME}_SRC
        main.cpp
        version_tests.cpp
        line_based_writers_tests.cpp
        file_stream_factory_tests.cpp
)

list(APPEND ${PROJECT_NAME}_INCLUDE)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC} ${${PROJECT_NAME}_INCLUDE})
target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

include(cmake/doctest.cmake)
doctest_discover_tests(${PROJECT_NAME} TEST_SPEC *)

