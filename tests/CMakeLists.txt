cmake_minimum_required(VERSION 3.25)

find_package(Catch2 CONFIG REQUIRED)

add_executable(macromuse_tests
    main.cpp
)

target_link_libraries(macromuse_tests PRIVATE
    Catch2::Catch2WithMain
    macromuser::macromuser
)

include(Catch)
catch_discover_tests(macromuse_tests)