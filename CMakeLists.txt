cmake_minimum_required(VERSION 3.21)
project(ft_irc)

set(CMAKE_CXX_STANDARD 98)

FILE(GLOB SOURCES
#        ${CMAKE_CURRENT_SOURCE_DIR}/srcs/main.cpp
#        ${CMAKE_CURRENT_SOURCE_DIR}/srcs/Server.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/srcs/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/srcs/*.hpp
        )

add_executable(ft_irc
        ${SOURCES})
