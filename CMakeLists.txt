cmake_minimum_required(VERSION 3.2...4.0)

project(http_server C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(http_server
    http_server.c
    files.c
)
