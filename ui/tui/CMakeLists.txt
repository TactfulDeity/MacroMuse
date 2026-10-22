cmake_minimum_required(VERSION 3.25)

project(macromuse
        VERSION 0.1.0
        DESCRIPTION "The TUI frontend for MacroMuse"
        LANGUAGES CXX
)

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)

if(Curses_FOUND AND NOT TARGET Curses::Curses)
  add_library(Curses::Curses INTERFACE IMPORTED)
  set_target_properties(
    Curses::Curses
    PROPERTIES
      INTERFACE_LINK_LIBRARIES "${CURSES_LIBRARIES}"
      INTERFACE_INCLUDE_DIRECTORIES "${CURSES_INCLUDE_DIRS}"
  )
endif()

add_executable(macromuse)

target_sources(macromuse PRIVATE
  tui_app.cpp
)

target_link_libraries(macromuse PRIVATE
    Curses::Curses
    macromuser::macromuser
)

# Windows top level dump
# set(DEST_DIR ".")

if(UNIX)
  set(DEST_DIR "bin")
endif()

install(TARGETS macromuse
  COMPONENT Applications
  RUNTIME DESTINATION ${DEST_DIR}
  LIBRARY DESTINATION ${DEST_DIR}
  ARCHIVE DESTINATION ${DEST_DIR}
  BUNDLE DESTINATION ${DEST_DIR}
)
