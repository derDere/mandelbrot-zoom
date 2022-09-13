#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#define ZOOM_STEP 0.9
#define INITIAL_SIZE 2
#define INITIAL_DEPTH 20
#define INITIAL_INFINITY 16

class View {
  private:
    long double center_x;
    long double center_y;
    long double size;
    long double depth;
    long double infinity;
    long double unit_x;
    long double unit_y;
    unsigned int screen_size_cols;
    unsigned int screen_size_rows;

  public:
    View(unsigned int, unsigned int);
    ~View();

    unsigned int calculate(unsigned int, unsigned int);
    void resize(unsigned int, unsigned int);
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    void zoomIn();
    void zoomOut();
};

#endif
