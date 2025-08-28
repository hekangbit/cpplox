cmake_minimum_required (VERSION 2.8)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-fsanitize=address -O0 -g -std=c++11 -Werror")
# set (CMAKE_CXX_FLAGS "-O0 -g -std=c++11 -Werror")
project (lox)

aux_source_directory(./src SRCS)

add_executable(${PROJECT_NAME} ${SRCS})

include_directories("${PROJECT_SOURCE_DIR}/inc")
