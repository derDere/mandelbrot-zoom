#ifndef MAIN_CPP
#define MAIN_CPP

#include <iostream>
#include <limits>
#include <iomanip>
#include <cstdlib>
#include <ncursesw/ncurses.h>
#include "calculator.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

void quit();

View* view;

/**
 * Project: mandelbrot-zoom
 * Creator: deremer
 * Creation Date: Tue Sep 13 11:31:51 CEST 2022
 */
int main(int argc, char* argv[]) {
  bool asciiMode = false;
  long double arg_x = 0, arg_y = 0;
  bool arg_x_set = false;

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
      cout << "Usage: mandelbrot-zoom [options] [center_x] [center_y]" << endl
           << endl
           << "Options:" << endl
           << "  -a          Activates ASCII mode. Use this mode if you" << endl
           << "              are having trouble displaying blocks in your terminal." << endl
           << endl
           << "Arguments:" << endl
           << "  center_x    Sets the center value on the X axis." << endl
           << "  center_y    Sets the center value of the Y axis." << endl
           << endl
           << "About:" << endl
           << "  This application provides a zoomable/movable view of the mandelbrot set." << endl
           << endl
           << "Key bindings:" << endl
           << "  WASD        Move around" << endl
           << "  HJKL        Move around" << endl
           << "  +           Zoom in" << endl
           << "  -           Zoom out" << endl
           << "  q           Quit" << endl
           << endl;
      return 0;
    } else {
      try {
        long double val = stold(arg);
        if (!arg_x_set) {
          arg_x = val;
          arg_x_set = true;
        } else {
          arg_y = val;
        }
      } catch (const exception& ex) {
        // No action needed invalid args will be ignored
      }
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
  mvaddstr( 3, 5, "Mandelbrot Zoom");
  attroff(A_UNDERLINE);
  mvaddstr( 5, 7, "Key bindings:");
  attroff(A_BOLD);
  mvaddstr( 6, 9, "WASD  Move around");
  mvaddstr( 7, 9, "HJKL  Move around");
  mvaddstr( 8, 9, "+     Zoom in");
  mvaddstr( 9, 9, "-     Zoom out");
  mvaddstr(10, 9, "q     Quit");
  attron(A_BOLD);
  mvaddstr(12, 7, "Press any key to start...");
  attroff(A_BOLD);
  getch();
  nodelay(win, true);

  int cols, rows;
  getmaxyx(stdscr, rows, cols);

  view = new View(cols, rows);
  view->center_x = arg_x;
  view->center_y = arg_y;

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

  cout << "Zoomed center at: "
       << setprecision(numeric_limits<long double>::max_digits10)
       << view->center_x << " " << view->center_y << endl;
}

#endif
