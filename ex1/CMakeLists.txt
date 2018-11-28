cmake_minimum_required(VERSION 3.6)
project(ex1)

set(-Wall -Wextra -Wvla CMAKE_C_STANDARD 99)

set(SOURCE_FILES ex1.c)
add_executable(ex1 ${SOURCE_FILES})
target_link_libraries(ex1 m)
