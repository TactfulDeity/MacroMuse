cmake_minimum_required(VERSION 3.25)

project(macromuser
        VERSION 0.1.0
        DESCRIPTION "the backend/core of MacroMuse"
        LANGUAGES CXX)

find_package(sol2 CONFIG REQUIRED)

add_library(macromuser SHARED)
add_library(macromuser::macromuser ALIAS macromuser)

target_link_libraries(macromuser PRIVATE
    sol2::sol2
)

# Define generated include directories
set(MACROMUSER_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated/include")
set(MACROMUSER_EXPORT_HEADER "${MACROMUSER_GENERATED_INCLUDE_DIR}/macromuser/macromuser_export.hpp")

include(GenerateExportHeader)
generate_export_header(macromuser 
    EXPORT_FILE_NAME ${MACROMUSER_EXPORT_HEADER}
)

# Define publicly accessible headers to link against
set(MACROMUSER_PUBLIC_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(MACROMUSER_PUBLIC_HEADER_DIR "${MACROMUSER_PUBLIC_INCLUDE_DIR}/macromuser")

set(MACROMUSER_HEADER_FILES
  ${MACROMUSER_PUBLIC_HEADER_DIR}/models/Macronutrients.hpp
  ${MACROMUSER_PUBLIC_HEADER_DIR}/models/Recipe.hpp
)

set(MACROMUSER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(MACROMUSER_SOURCE_FILES
  ${MACROMUSER_SOURCE_DIR}/main.cpp
)

target_sources(macromuser PRIVATE
    ${MACROMUSER_SOURCE_FILES}
    ${MACROMUSER_HEADER_FILES}
    ${MACROMUSER_EXPORT_HEADER}
)

target_include_directories(macromuser PUBLIC
    $<BUILD_INTERFACE:${MACROMUSER_PUBLIC_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${MACROMUSER_GENERATED_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)
