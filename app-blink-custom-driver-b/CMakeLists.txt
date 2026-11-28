cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED)

# Pull in the custom Kconfig (adds CONFIG_SENSOR_OUR_DRIVER)
set(KCONFIG_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/Kconfig)

project(app-blink-s3-wroom-1-n8r2-devkitc-rev1.0)

target_sources(app PRIVATE src/main.cpp)

# Add driver source directly into Zephyr-OS drivers
zephyr_library_sources_ifdef(CONFIG_SENSOR_OUR_DRIVER src/our_driver.c)