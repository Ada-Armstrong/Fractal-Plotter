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

	int i = 0;
	for (; i < width * height; ++i) {
		f->bitmap[i] = malloc(sizeof(**(f->bitmap)) * 3);
		if (f->bitmap[i] == NULL)
			break;
	}

	if (i != width *height) {
		for (int j = 0; j < i; ++j) {
			free(f->bitmap[j]);
		}
		free(f->bitmap);
		free(f);
		return NULL;
	}

	return f;
}

void destroy_frame(struct frame *f)
{
	if (!f)
		return;
	if (f->bitmap) {
		for (int i = 0; i < f->width * f->height; ++i) {
			free(f->bitmap[i]);
		}
		free(f->bitmap);
	}

	free(f);
}
