cmake_minimum_required(VERSION 3.17)
project(yan_zhuk_test_task)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 \
      -Wall -Wextra -Werror -O0 -g -fsanitize=address -fno-omit-frame-pointer \
      -pedantic \
      -Wshadow \
      -Wconversion \
      -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC \
      -fsanitize=undefined -fno-sanitize-recover")
endif ()

add_executable(yan_zhuk_test_task main.cpp lib/utils.cpp)