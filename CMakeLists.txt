cmake_minimum_required (VERSION 2.8)

option(USE_ASAN "Enable AddressSanitizer" OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-O0 -g -std=c++11 -Werror")
if(USE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

project (lox)

aux_source_directory(./src SRCS)

add_executable(${PROJECT_NAME} ${SRCS})

include_directories("${PROJECT_SOURCE_DIR}/inc")
