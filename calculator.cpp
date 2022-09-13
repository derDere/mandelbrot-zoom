#ifndef CALCULATOR_CPP
#define CALCULATOR_CPP

#include "calculator.hpp"

using namespace std;

View::View(unsigned int cols, unsigned int rows) {
  this->center_x = 0;
  this->center_y = 0;
  this->size = INITIAL_SIZE;
  this->depth = INITIAL_DEPTH;
  this->infinity = INITIAL_INFINITY;
  this->resize(cols, rows);
}

View::~View() {
  this->center_x = 0;
  this->center_y = 0;
  this->size = 0;
  this->depth = 0;
  this->infinity = 0;
  this->unit_x = 0;
  this->unit_y = 0;
}

unsigned int View::calculate(unsigned int col, unsigned int row) {
  long double x = (this->center_x - this->size) + (this->unit_x * col);
  long double y = (this->center_y - this->size) + (this->unit_y * row);
  long double a = x;
  long double b = y;
  unsigned int n = 0;
  while (n < this->depth) {
    long double aa = (a * a) - (b * b);
    long double bb = 2 * a * b;
    a = aa + x;
    b = bb + y;
    long double abAbs = a + b;
    if (abAbs < 0) abAbs = -abAbs;
    if (abAbs > this->infinity) {
      long double nn = n / this->depth;
      nn *= 36;
      return 12 - ((unsigned int)nn % 13);
    }
    n++;
  }
  return 0;
}

void View::resize(unsigned int cols, unsigned int rows) {
  this->screen_size_cols = cols;
  this->screen_size_rows = rows;
  if (cols < rows) {
    this->unit_x = (this->size * 2) / (cols);
    this->unit_y = (this->size * 2) / (cols / 2);
  } else {
    this->unit_x = (this->size * 2) / (rows * 2);
    this->unit_y = (this->size * 2) / (rows);
  }
}

void View::moveLeft() {
  this->center_x -= this->unit_x;
}

void View::moveRight() {
  this->center_x += this->unit_x;
}

void View::moveUp() {
  this->center_y -= this->unit_y;
}

void View::moveDown() {
  this->center_y += this->unit_y;
}

void View::zoomIn() {
  if ((this->center_x + this->unit_y) == this->center_x) return;
  this->size *= ZOOM_STEP;
  this->depth /= (ZOOM_STEP * 1.1);
  this->infinity /= (ZOOM_STEP * 1.1);
  this->resize(this->screen_size_cols, this->screen_size_rows);
}

void View::zoomOut() {
  if (this->size >= 2) return;
  this->size /= ZOOM_STEP;
  this->depth *= (ZOOM_STEP * 1.1);
  this->infinity *= (ZOOM_STEP * 1.1);
  this->resize(this->screen_size_cols, this->screen_size_rows);
}

#endif
