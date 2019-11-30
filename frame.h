#ifndef __FRACTAL_FRAME_H__
#define __FRACTAL_FRAME_H__

#include <stdlib.h>
#include <stdint.h>

struct frame {
	unsigned int width;
	unsigned int height;
	double r_domain[2];
	double i_domain[2];
	unsigned int max_iters;
	uint8_t **bitmap;
};

struct frame *create_frame(unsigned int width, unsigned int height,
		double r_left, double r_right,
		double i_bottom, double i_top,
		unsigned int max_iters);

void destroy_frame(struct frame *f);

#endif
