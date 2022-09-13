# Mandelbrot Zoom
A terminal based mandelbrot set renderer. You can zoom in and out and move around the set.

![Mandelbrot Zomm Normal Screenshot](https://raw.githubusercontent.com/derDere/mandelbrot-zoom/main/mandel-zoom-n.png)
![Mandelbrot Zoom Ascii Screenshot](https://raw.githubusercontent.com/derDere/mandelbrot-zoom/main/mandel-zoom-a.png)

## Instalation
Enter the folowing command into your terminal:
```sh
git clone https://github.com/derDere/mandelbrot-zoom.git
cd mandelbrot-zoom
sudo make install
cd ..
rm -rf mandelbrot-zoom
```

## Usage
To start the renderer simply type the following command:
```sh
mandelbrot-zoom
```

If your terminal has problems displaying the block characters you can start the renderer in ASCII-mode:
```sh
mandelbrot-zoom -a
```

## Key bindings
While the renderer is running you can use the following keybindings:
 - Use (WASD) or (HJKL) to move around the set.
 - Zoom in and out using (+) or (-)
 - You can quit using (q)


## Uninstall
Run the following command:
```sh
sudo rm /usr/local/bin/mandelbrot-zoom
```

Or if you still have the repository, you can use the following command in its folder:
```sh
sudo make uninstall
```
