#include "frame.h"

struct frame *create_frame(unsigned int width, unsigned int height,
		double r_left, double r_right,
		double i_bottom, double i_top,
		unsigned int max_iters)
{
	struct frame *f = malloc(sizeof(*f));
	if (!f)
		return NULL;

	f->width = width;
	f->height = height;

	f->r_domain[0] = r_left;
	f->r_domain[1] = r_right;

	f->i_domain[0] = i_bottom;
	f->i_domain[1] = i_top;

	f->max_iters = max_iters;

	f->bitmap = malloc(sizeof(*(f->bitmap)) * f->width * f->height);
	if (!f->bitmap) {
		free(f);
		return NULL;
	}

	unsigned int i = 0;
	for (; i < width * height; ++i) {
		f->bitmap[i] = malloc(sizeof(**(f->bitmap)) * 3);
		if (f->bitmap[i] == NULL)
			break;
	}

	if (i != width *height) {
		for (unsigned int j = 0; j < i; ++j) {
			free(f->bitmap[j]);
		}
		free(f->bitmap);
		free(f);
		return NULL;
	}

	f->nproc = get_nprocs();

	return f;
}

void destroy_frame(struct frame *f)
{
	if (!f)
		return;
	if (f->bitmap) {
		for (unsigned int i = 0; i < f->width * f->height; ++i) {
			free(f->bitmap[i]);
		}
		free(f->bitmap);
	}

	free(f);
}

struct thread_frame *create_thread_frame(struct frame *f,
		double *r_axis, double *i_axis, int t_count)
{
	struct thread_frame *t_f = malloc(sizeof(*t_f));
	if (!f)
		return NULL;

	t_f->frame = f;
	t_f->r_axis = r_axis;
	t_f->i_axis = i_axis;
	t_f->t_count = t_count;

	return t_f;
}
