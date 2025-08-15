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
	// for julia sets
	double c[2];
	// thread vars
	int nproc;
	int t_count;
};

struct thread_frame {
	struct frame *frame;
	double *r_axis;
	double *i_axis;
	int t_count;
};

struct frame *create_frame(unsigned int width, unsigned int height,
		double r_left, double r_right,
		double i_bottom, double i_top,
		unsigned int max_iters, double c_r, double c_i);

void destroy_frame(struct frame *f);

struct thread_frame *create_thread_frame(struct frame *f,
		double *r_axis, double *i_axis, int t_count);

#endif
