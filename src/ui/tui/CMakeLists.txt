cmake_minimum_required(VERSION 3.25)

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

add_executable(macromuse_tui
    tui_app.cpp
)

target_link_libraries(macromuse_tui PRIVATE
    Curses::Curses
)