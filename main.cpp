#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include <ncursesw/ncurses.h>
#include "calculator.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

void quit();

/**
 * Project: mandelbrot-zoom
 * Creator: deremer
 * Creation Date: Tue Sep 13 11:31:51 CEST 2022
 */
int main(int argc, char* argv[]) {
  bool asciiMode = false;

  for(int i = 0; i < argc; i++) {
    string arg(argv[i]);
    if (arg == "-a") {
      asciiMode = true;
    }
    else if (
      (arg == "-?") ||
      (arg == "/?") ||
      (arg == "help") ||
      (arg == "--help")
    ) {
      cout << "Hilfe kommt noch" << endl;
      return 0;
    }
  }

  // Init Curses ----------
  setlocale(LC_ALL, "");
  WINDOW* win = initscr();
  atexit(quit);
  curs_set(0);
  start_color();
  clear();
  noecho();
  cbreak();
  keypad(stdscr, true);
  //mousemask(BUTTON1_CLICKED, NULL); //ALL_MOUSE_EVENTS, NULL);

  // Draw Start Screen
  attron(A_BOLD | A_UNDERLINE);
  mvaddstr( 3, 5, "----------------");
  attroff(A_UNDERLINE);
  mvaddstr( 5, 7, "Key bindings:");
  attroff(A_BOLD);
  mvaddstr( 6, 9, ".");
  mvaddstr( 7, 9, ".");
  mvaddstr( 8, 9, ".");
  mvaddstr( 9, 9, ".");
  mvaddstr(10, 9, ".");
  attron(A_BOLD);
  mvaddstr(12, 7, "Press any key to start...");
  attroff(A_BOLD);
  getch();
  nodelay(win, true);

  int cols, rows;
  getmaxyx(stdscr, rows, cols);

  View* view = new View(cols, rows);

  int key = ' ';
  const char* values[13] = {
                             asciiMode ? " " : " ",
                             asciiMode ? "." : "░",
                             asciiMode ? "," : "░",
                             asciiMode ? "-" : "░",
                             asciiMode ? "~" : "▒",
                             asciiMode ? ":" : "▒",
                             asciiMode ? ";" : "▒",
                             asciiMode ? "=" : "▓",
                             asciiMode ? "!" : "▓",
                             asciiMode ? "*" : "▓",
                             asciiMode ? "#" : "█",
                             asciiMode ? "$" : "█",
                             asciiMode ? "@" : "█"
                           };

  while (key != 'q') {
    int w, h;
    if (cols < rows) {
      w = cols;
      h = w / 2;
    } else {
      h = rows;
      w = h * 2;
    }
    for(int y = 0; y < h; y++) {
      move(y, 0);
      for(int x = 0; x < w; x++) {
        int n = view->calculate(x, y);
        printw(values[n]);
      }
    }
    key = getch();
    switch(key) {
      case KEY_RESIZE:
          getmaxyx(stdscr, rows, cols);
          view->resize(cols, rows);
          erase();
        break;
      case 'h':
      case 'a':
          view->moveLeft();
        break;
      case 'w':
      case 'k':
          view->moveUp();
        break;
      case 's':
      case 'j':
          view->moveDown();
        break;
      case 'l':
      case 'd':
          view->moveRight();
        break;
      case '+':
          view->zoomIn();
        break;
      case '-':
          view->zoomOut();
        break;
    }
  }

  return 0;
}

void quit() {
  endwin();
}

#endif
