#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "frame.h"
#include "ppm.h"
#include "arg.h"

#define PYTHAGOREAN(a, b) ((a * a) + (b * b))

unsigned int num_iterations(unsigned int max_iters, double z_r, double z_i,
		double r, double i)
{
	double tmp;

	unsigned int iters = 0;
	for (; iters < max_iters && PYTHAGOREAN(z_r, z_i) < 4; ++iters) {
		tmp = (z_r * z_r) - (z_i * z_i) + r;
		z_i = (2 * z_r * z_i) + i;
		z_r = tmp;
	}

	return iters;
}

void set_colour(unsigned int i, unsigned int max_iters, uint8_t pixel[])
{
	double t = (double)i / (double)max_iters;
	pixel[0] = (uint8_t)(9 * (1 - t) * t * t * t * 255);
	pixel[1] = (uint8_t)(15 * (1 - t) * (1 - t) * t * t * 255);
	pixel[2] = (uint8_t)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
}

double *create_axis(unsigned int len, double min, double max)
{
	double *axis = malloc(sizeof(*axis) * len);
	if (!axis)
		return NULL;

	double step = (max - min) / (double)len;

	for (unsigned int i = 0; i < len; ++i) {
		axis[i] = i * step + min;
	}

	return axis;
}

void *thread_mandel(void *data)
{
	struct thread_frame *t_f = data;
	struct frame *f = t_f->frame;

	unsigned int y = t_f->t_count * (f->height / f->nproc);
	unsigned int y_end = (t_f->t_count + 1) * (f->height / f->nproc);
	int iters;

	for (; y < y_end; ++y) {
		for (unsigned int x = 0; x < f->width; ++x) {
			iters = num_iterations(f->max_iters, 0, 0,
					t_f->r_axis[x], t_f->i_axis[y]);
			set_colour(iters, f->max_iters,
					f->bitmap[y * f->width + x]);
		}
	}
	return NULL;
}

void *thread_julia(void *data)
{
	struct thread_frame *t_f = data;
	struct frame *f = t_f->frame;

	unsigned int y = t_f->t_count * (f->height / f->nproc);
	unsigned int y_end = (t_f->t_count + 1) * (f->height / f->nproc);
	int iters;

	for (; y < y_end; ++y) {
		for (unsigned int x = 0; x < f->width; ++x) {
			iters = num_iterations(f->max_iters,
					t_f->r_axis[x], t_f->i_axis[y],
					f->c[0], f->c[1]);
			set_colour(iters, f->max_iters,
					f->bitmap[y * f->width + x]);
		}
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	struct program_vars vars = {800, 800, 50, -2.5, 1, NULL, 0, 0, 0, 0};

	for (int i = 1; i < argc; ++i) {
		if (!handle_args(argv[i], &vars))
			return -1;
	}

	if (vars.file_name == NULL)
		vars.file_name = "fractal.ppm";

	// to keep the proper aspect ratio
	double i_top = (vars.height * (vars.r_right - vars.r_left))
				/ (2 * vars.width);
	double i_bottom = -i_top;

	struct frame *f = create_frame(vars.width, vars.height,
			vars.r_left, vars.r_right, i_top, i_bottom,
			vars.max_iters, vars.c_r, vars.c_i);
	if (!f) {
		fprintf(stderr, "Out of memory error\n");
		return 1;
	}

	double *r_axis = create_axis(f->width, f->r_domain[0], f->r_domain[1]);
	double *i_axis = create_axis(f->height, f->i_domain[0], f->i_domain[1]);
	if (!r_axis || !i_axis) {
		fprintf(stderr, "Out of memory error\n");
		destroy_frame(f);
		return 1;
	}

	void *(*thread_colour)(void *) = vars.is_julia ? &thread_julia
			: &thread_mandel;

	pthread_t threads[f->nproc];
	struct thread_frame *t_frames[f->nproc];

	clock_t time = clock();

	for (int i = 0; i < f->nproc; ++i) {
		t_frames[i] = create_thread_frame(f, r_axis, i_axis, i);
		if (t_frames[i] == NULL) {
			fprintf(stderr, "Out of memory error\n");
			return 1;
		}
		if (pthread_create(threads + i, NULL,
				thread_colour, t_frames[i]) != 0) {
			fprintf(stderr, "Failed to create thread %d\n", i);
			return 1;
		}
	}

	for (int i = 0; i < f->nproc; ++i) {
		pthread_join(threads[i], NULL);
		free(t_frames[i]);
	}

	time = clock() - time;

	if (vars.show_time)
		fprintf(stdout, "Plotting took: %f seconds\n",
				(double)time / CLOCKS_PER_SEC);

	free(r_axis);
	free(i_axis);

	struct ppm *img = create_ppm(f->bitmap, f->width, f->height, 255);
	if (save_ppm_to_file(img, vars.file_name) != 0) {
		fprintf(stderr, "Failed to save image\n");
	} else {
		destroy_ppm(img);
		fprintf(stdout, "Image saved...\n");
	}

	destroy_frame(f);
	return 0;
}
