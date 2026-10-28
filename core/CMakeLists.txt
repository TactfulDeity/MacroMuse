cmake_minimum_required(VERSION 3.25)

project(macromuser
  VERSION 0.1.0
  DESCRIPTION "the backend/core of MacroMuse"
  LANGUAGES CXX
)

# Define Data Directory in Binary and Copy Data Across
if(NOT EXISTS "${MACROMUSE_DATA_DIR}")
  message(FATAL_ERROR "MACROMUSE_DATA_DIR not found: ${MACROMUSE_DATA_DIR}")
endif()

set(MACROMUSER_BINARY_DATA_DIR "${CMAKE_CURRENT_BINARY_DIR}/data")
set(COPY_STAMP "${CMAKE_CURRENT_BINARY_DIR}/.copy_data.stamp")

file(GLOB_RECURSE MACROMUSE_DATA_FILES
  CONFIGURE_DEPENDS
  LIST_DIRECTORIES FALSE
  "${MACROMUSE_DATA_DIR}/*"
)

add_custom_command(
  OUTPUT "${COPY_STAMP}"

  COMMAND ${CMAKE_COMMAND} -E rm -rf "${MACROMUSER_BINARY_DATA_DIR}"
  COMMAND ${CMAKE_COMMAND} -E make_directory "${MACROMUSER_BINARY_DATA_DIR}"
  
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${MACROMUSE_DATA_DIR}"
    "${MACROMUSER_BINARY_DATA_DIR}"

  COMMAND ${CMAKE_COMMAND} -E touch "${COPY_STAMP}"

  DEPENDS ${MACROMUSE_DATA_FILES}

  COMMENT "Copying MacroMuse Data into build tree"
  VERBATIM
)

add_custom_target(COPY_DATA_TO_BINARY ALL
  DEPENDS "${COPY_STAMP}"
)

find_package(sol2 CONFIG REQUIRED)

add_library(macromuser SHARED)
add_library(macromuser::macromuser ALIAS macromuser)

add_dependencies(macromuser COPY_DATA_TO_BINARY)

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

set(MACROMUSER_PUBLIC_HEADER_FILES
  "${MACROMUSER_PUBLIC_HEADER_DIR}/models/macronutrients.hpp"
  "${MACROMUSER_PUBLIC_HEADER_DIR}/models/recipe.hpp"

  "${MACROMUSER_PUBLIC_HEADER_DIR}/parsers/csv_parser.hpp"
)

set(MACROMUSER_PRIVATE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/macromuser/internal")

# Generate a file with the path to data dir
set(MACROMUSER_FILEPATH_HEADER "${MACROMUSER_GENERATED_INCLUDE_DIR}/macromuser/filepaths.hpp")
configure_file(
    "${MACROMUSER_PRIVATE_INCLUDE_DIR}/filepaths.hpp.in"
    ${MACROMUSER_FILEPATH_HEADER}
    @ONLY
)

set(MACROMUSER_PRIVATE_HEADER_FILES
  "${MACROMUSER_PRIVATE_INCLUDE_DIR}/parsers/two_million_parser.hpp"
)

set(MACROMUSER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/macromuser/src")

set(MACROMUSER_SOURCE_FILES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"

  "${MACROMUSER_SOURCE_DIR}/parsers/two_million_parser.cpp"
)

target_sources(macromuser PRIVATE
    ${MACROMUSER_SOURCE_FILES}
    ${MACROMUSER_PUBLIC_HEADER_FILES}
    ${MACROMUSER_PRIVATE_HEADER_FILES}
    ${MACROMUSER_EXPORT_HEADER}
    ${MACROMUSER_FILEPATH_HEADER}
)

target_include_directories(macromuser
    PRIVATE $<BUILD_INTERFACE:${MACROMUSER_PRIVATE_INCLUDE_DIR}>
    PRIVATE $<BUILD_INTERFACE:${MACROMUSER_GENERATED_INCLUDE_DIR}>
    PUBLIC $<BUILD_INTERFACE:${MACROMUSER_PUBLIC_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)
