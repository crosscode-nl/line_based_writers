# line_based_writers
[![GitHub version](https://badge.fury.io/gh/crosscode-nl%2Fline_based_writers.svg)](https://badge.fury.io/gh/crosscode-nl%2Fline_based_writers)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/crosscode-nl/line_based_writers.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/crosscode-nl/line_based_writers/context:cpp)
[![Build Status](https://travis-ci.com/crosscode-nl/line_based_writers.svg?branch=main)](https://travis-ci.com/crosscode-nl/line_based_writers) 
[![Build status](https://ci.appveyor.com/api/projects/status/hcvtjdkwkpfjtj22?svg=true)](https://ci.appveyor.com/project/crosscode-nl/line-based-writers) 
[![Standard](https://img.shields.io/badge/c%2B%2B-17/20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B#Standardization)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

Note: I really want lgtm to work, but they seem to support a really old version of CMake. I have to check in the future if they started supporting a more modern version of CMake.
 

## Introduction

Line based writers provide template classes for writing lines from [influxdblpexporter](https://github.com/crosscode-nl/influxdblpexporter) to files. 
These template classes are also intended for use with other line based protocols and exporters. 

It supports writing to files and automatically segmenting files based on line count. File name generation is base on templates. 

## Installation

There are multiple ways to add this library to your project. There are too many tools for C++ to describe them all. 

I will describe two methods, both for CMake users. 

The first method will download the source code and build against it, the second will use a system installed version of 
the library.

The first method works best in CI/CD pipelines without to much hassle. This is therefore the preferred method.

### CMake FetchContent (Preferred)

This method emulates dependency management of more modern languages and build systems like Rust+Cargo and Go as best as 
it can.

Consider the following example project: 

```cmake
cmake_minimum_required(VERSION 3.17)
project(example)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
```

This will build a C++17 project with a main.cpp file.

Now to add this library to the project you can use the 
[FetchContent CMake module](https://cmake.org/cmake/help/v3.18/module/FetchContent.html). 

The minimal requirement is to add: 

```cmake
include(FetchContent)
FetchContent_Declare(
        line_based_writers
        GIT_REPOSITORY https://github.com/crosscode-nl/line_based_writers
        GIT_TAG v1.3.0
)
FetchContent_MakeAvailable(line_based_writers)
target_link_libraries(${PROJECT_NAME} line_based_writers)
```

This will result in the following CMakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.17)
project(example)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

include(FetchContent)
FetchContent_Declare(
        line_based_writers
        GIT_REPOSITORY https://github.com/crosscode-nl/line_based_writers
        GIT_TAG v1.3.0
)
FetchContent_MakeAvailable(line_based_writers)
target_link_libraries(${PROJECT_NAME} line_based_writers)
```

You can also put the first block in a `cmake/line_based_writers.cmake` file and include it as following: 

```cmake
cmake_minimum_required(VERSION 3.17)
project(example)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

include(cmake/line_based_writers.cmake)
```
 
Now see the examples to learn how to use this library.

*TIP: Use `GIT_TAG` in `FetchContent_Declare` to pin a certain version to get reproducible builds.*

### CMake Find_Package

This method will use CMake's find_package.

The first step is to install the library on the system.

#### Installing onto the system

This method requires line_based_writers to be build and installed on the system.

```bash
git clone https://github.com/crosscode-nl/line_based_writers
mkdir line_based_writers-build
cd line_based_writers-build
cmake ../line_based_writers -DCMAKE_BUILD_TYPE=Release
sudo make install 
```

Or if you want to install it somewhere in you home directory, for example: 

```bash
git clone https://github.com/crosscode-nl/line_based_writers
mkdir line_based_writers-build
cd line_based_writers-build
cmake ../line_based_writers -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/home/developer/libraries
make install 
```

#### Using the installed library

Consider the following example project: 

```cmake
cmake_minimum_required(VERSION 3.17)
project(example)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
```

To use an installed library you just have to add: 

```cmake
find_package(LineBasedWriters 1.3.0 REQUIRED)
target_link_libraries(${PROJECT_NAME} LineBasedWriters::line_based_writers)
```

This will result in the following CMakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.17)
project(example)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

find_package(LineBasedWriters 1.3.0 REQUIRED)
target_link_libraries(${PROJECT_NAME} LineBasedWriters::line_based_writers)
```

When you installed the library in a different location then you have to add 
`-DCMAKE_PREFIX_PATH=/home/developer/libraries` to your 
[CMake command](https://cmake.org/cmake/help/latest/variable/CMAKE_PREFIX_PATH.html). 

## Examples

Examine the following examples to learn how to use this library.

* [example01.cpp](examples/example01.cpp) This is a minimal example and explains some **important key concepts**.

## License

MIT License

Copyright (c) 2020 CrossCode / P. Vollebregt

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit 
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the 
Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR 
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
