# Copyright (c) 2022 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19)
cmake_policy(VERSION 3.19)

set(MBED_OS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")

# Set default path for mbed_app.json5 (no force-write for override)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/mbed_app.json5)
set(MBED_APP_JSON_PATH mbed_app.json5 CACHE INTERNAL "")
endif()

# Set default path for custom_targets.json5 (no force-write for override)
set(CUSTOM_TARGETS_PATH custom_targets CACHE INTERNAL "")
if(EXISTS "${CUSTOM_TARGETS_PATH}/custom_targets.json5")
    set(CUSTOM_TARGETS_JSON_PATH ${CUSTOM_TARGETS_PATH}/custom_targets.json5 CACHE INTERNAL "")
endif()

# Load Mbed CE toolchain file and basic build system
include(${MBED_OS_PATH}/tools/cmake/app.cmake)

set(APP_PROJECT NU_MBED_CE_TEST_M480_WDT_RESET)
set(APP_TARGET nu-mbed-ce-test-m480-wdt-reset)

# Set up project name
project(${APP_PROJECT})

# Add Mbed OS library
add_subdirectory(${MBED_OS_PATH})

add_executable(${APP_TARGET})

target_sources(${APP_TARGET}
    PRIVATE
        main.cpp
)

target_link_libraries(${APP_TARGET}
    PRIVATE
        mbed-os
)

# Must call this for each target to set up bin file creation, code upload, etc
mbed_set_post_build(${APP_TARGET})

# Make sure this is the last line of the top-level build script
mbed_finalize_build()
