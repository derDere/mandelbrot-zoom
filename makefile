SRC = $(wildcard *.cpp)
APP = "mandelbrot-zoom"

All: $(APP)
	@echo Done

$(APP): $(SRC)
	g++ -o $(APP) $(SRC) -lncursesw

test: $(APP)
	./$(APP) debug
