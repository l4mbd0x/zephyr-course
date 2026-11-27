cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED)
project(app-blink-s3-wroom-1-n8r2-devkitc-rev1.0-kconfig-choice)

target_sources(app PRIVATE src/main.cpp)
