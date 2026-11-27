cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED)
project(app-heartbeat-s3-r1.0-dts)

target_sources(app PRIVATE src/main.cpp)
