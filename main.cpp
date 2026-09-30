#ifndef MAIN_CPP
#define MAIN_CPP

#include <cctype>
#include <cstring>
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
std::string getWasdName();
std::string getHjklName();
View* view;
enum KeyboardLayout{
  QWERTY,
  COLEMAK,
  DVORAK
};
/**
 * Project: mandelbrot-zoom
 * Creator: deremer
 * Creation Date: Tue Sep 13 11:31:51 CEST 2022
 */
 KeyboardLayout keyboardLayout = QWERTY;
  std::string keyboardLayoutNames[3] = {"QWERTY", "Colemak", "Dvorak"};
  char keys[size(keyboardLayoutNames)][8] = {
    {'w','a', 's', 'd', 'h', 'j', 'k', 'l'},
    {'w','a', 'r', 's', 'h', 'n', 'e', 'i'},
    {'<','a', 'o', 'e', 'd', 'h', 't', 'n'}
  };

  std::string getWasdName(){
    std::string wasdName = "";
    for(int i = 0; i <= 3; i++){
      wasdName += toupper(keys[keyboardLayout][i]);
    }
    return wasdName;
  }

  std::string getHjklName(){
    std::string hjklName = "";
    for(int i = 4; i <= 7; i++){
      hjklName += toupper(keys[keyboardLayout][i]);
    }
    return hjklName;
  }

int main(int argc, char* argv[]) {
  bool asciiMode = false;
  long double arg_x = 0, arg_y = 0;
  bool arg_x_set = false;  

  for(int i = 0; i < argc; i++) {
    string arg(argv[i]);
    if (arg == "-a") {
      asciiMode = true;
    }
    else if(arg == "-c"){
        keyboardLayout = COLEMAK;
    }
    else if(arg == "-d"){
        keyboardLayout = DVORAK;
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
           << "  -c          Use Colemak keybindings." << endl
           << "  -d          Use Dvorak keybindings." << endl
           << endl
           << "Arguments:" << endl
           << "  center_x    Sets the center value on the X axis." << endl
           << "  center_y    Sets the center value of the Y axis." << endl
           << endl
           << "About:" << endl
           << "  This application provides a zoomable/movable view of the mandelbrot set." << endl
           << endl
           << "Key bindings:" << endl
           << "  WASD (WARS, <AOE)        Move around" << endl
           << "  HJKL (HNEI, DHTN)        Move around" << endl
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
  mvaddstr( 6, 9, (getWasdName()+"  Move around").c_str());
  mvaddstr( 7, 9, (getHjklName()+"  Move around").c_str());
  mvaddstr( 8, 9, "+     Zoom in");
  mvaddstr( 9, 9, "-     Zoom out");
  mvaddstr(10, 9, "q     Quit");
  mvaddstr(11, 9, ("Keyboard: "+keyboardLayoutNames[keyboardLayout]).c_str());
  attron(A_BOLD);
  mvaddstr(13, 7, "Press any key to start...");
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
    //case statements do not support variables, replacing with else if
    
      if(key == KEY_RESIZE){
          getmaxyx(stdscr, rows, cols);
          view->resize(cols, rows);
          erase();
      }
      else if(key == keys[keyboardLayout][4] || key == keys[keyboardLayout][1])
          view->moveLeft();
      else if(key == keys[keyboardLayout][6] || key == keys[keyboardLayout][0])
          view->moveUp();
      else if(key == keys[keyboardLayout][5] || key == keys[keyboardLayout][2])
          view->moveDown();
      else if(key == keys[keyboardLayout][7] || key == keys[keyboardLayout][3])
          view->moveRight();
      else if(key == '+')
          view->zoomIn();
      else if(key == '-')
          view->zoomOut();
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
