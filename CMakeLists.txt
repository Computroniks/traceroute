# SPDX-FileCopyrightText: 2023 Matthew Nickson <mnickson@sidingsmedia.com>
# SPDX-License-Identifier: MIT


cmake_minimum_required(VERSION 3.10)
project(traceroute VERSION 0.1.0)


add_executable(traceroute)
target_sources(
    traceroute PRIVATE 
    src/main.cc
    src/config.cc
    src/trace.cc
)

target_include_directories(traceroute PUBLIC include/)

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(traceroute ${Boost_LIBRARIES})
