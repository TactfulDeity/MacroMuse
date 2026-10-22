#ifdef __MINGW32__
  #include <ncurses/ncurses.h>
#else
  #include <ncurses.h>
#endif

#include <iostream>

namespace {
class NcursesSession {
public:
  NcursesSession() {
    // Initalize the library
    initscr();

    // Do not echo user output
    noecho();

    // Do not buffer user input, retain Ctrl-Z and Ctrl-C functions
    cbreak();

    // Do not buffer any user input. Present all input to program
    //raw();

    // Enable extended character {e.g. F-keys, numpad} input
    keypad(stdscr, TRUE);

    // Change cursor appearance. 0 invisible, 1 normal, 2 strong
    curs_set(1);
  }

  ~NcursesSession() {
    // Main ncurses cleanup
    endwin();
  }

  int tui() {
    if(has_colors() == FALSE) {
        endwin();
        std::cout << "Your terminal doesn't support colors." << std::endl;
        return 1;
    }

    start_color();
    // fore and background colors
    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    attron(COLOR_PAIR(1));

    int x,y;
    getmaxyx(stdscr, y, x);

    y = y * 0.5;
    x = (x * 0.5) - 6;

    mvwprintw(stdscr, x, y, "Hello World!");
    mvwprintw(stdscr, 2, 2, "Hello World!");
    refresh();

    attroff(COLOR_PAIR(1));

    // Wait for key press before exiting
    getch();

    return 0;
  }
};
}

int main() {
  NcursesSession ncurses_session{};

  return ncurses_session.tui();
}