#ifndef __FRACTAL_ARG_H__
#define __FRACTAL_ARG_H__

#include <stdio.h>
#include <string.h>
#include "tokenize.h"

struct program_vars {
	unsigned int width;
	unsigned int height;
	unsigned int max_iters;
	char *file_name;
	int show_time;
};

int handle_args(char *arg, struct program_vars *vars);

#endif
