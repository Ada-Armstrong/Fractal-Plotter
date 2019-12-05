# cfp - C Fractal Plotter

A command line utility to create ppm images of the Mandlebrot set and Julia sets

## Usage

cfp [OPTION]... [FILE]
	FILE is the file path to save the image, note defaults to fractal.ppm

### Options

--help
	print this page and exit

-t
	print the time to plot the fractal to stdin

-i[NUM]
	sets the maximum number of iterations for each point to NUM

-d[WIDTH]x[HEIGHT]
	change the width and height of the image to WIDTH and HEIGHT

-r[LEFT]x[RIGHT]
	change the plotting domain for the real line, the domain for the
	imaginary line is determined by the aspect ratio and the real domain

-j[REAL]x[IMAGINARY]
	plot a Julia set with c=REAL+IMAGINARYi, note without this option 
	defaults to plotting the Mandlebrot set

### Build

```
make
```
