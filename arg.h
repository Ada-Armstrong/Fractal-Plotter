#ifndef __FRACTAL_ARG_H__
#define __FRACTAL_ARG_H__

#include <stdio.h>
#include <string.h>
#include "tokenize.h"

struct program_vars {
	unsigned int width;
	unsigned int height;
	unsigned int max_iters;
	double r_left;
	double r_right;
	char *file_name;
	int show_time;
	// constant if plotting a julia set
	int is_julia;
	double c_r;
	double c_i;
};

int handle_args(char *arg, struct program_vars *vars);

#endif
