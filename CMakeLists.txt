cmake_minimum_required(VERSION 3.25)

project(MacroMuse
        VERSION 0.1.0
        DESCRIPTION "A local recipe design assistant"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Enable testing" OFF)
option(BUILD_TUI "Build TUI" OFF)

if(BUILD_TESTS AND NOT "tests" IN_LIST VCPKG_MANIFEST_FEATURES)
    message(FATAL_ERROR
        "BUILD_TESTS=ON requires VCPKG_MANIFEST_FEATURES to include `tests`.")
endif()

if(BUILD_TUI AND NOT "tui" IN_LIST VCPKG_MANIFEST_FEATURES)
    message(FATAL_ERROR
        "BUILD_TUI=ON requires VCPKG_MANIFEST_FEATURES to include 'tui'.")
endif()

# Used in core
if(NOT DEFINED MACROMUSE_DATA_DIR)
  set(MACROMUSE_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")
endif()

add_subdirectory(core)

if(BUILD_TUI)
    add_subdirectory(ui/tui)
endif()

if(BUILD_TESTS)
    enable_testing()
    include(CTest)
    add_subdirectory(tests)
endif()