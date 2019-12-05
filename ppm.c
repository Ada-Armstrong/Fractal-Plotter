#include "ppm.h"

// assumes data is an array of uint8_t arries
// (of length 3 for the three color channels)
struct rgb *convert_to_rgb(uint8_t **data, unsigned int width,
		unsigned int height, uint8_t depth)
{
	const unsigned int len = width * height;
	struct rgb *pixels = malloc(sizeof(*pixels) * len);
	if (!pixels)
		return NULL;
	for (unsigned int i = 0; i < len; ++i) {
		if ((pixels[i].red = data[i][0]) > depth
				|| (pixels[i].green = data[i][1]) > depth
				|| (pixels[i].blue = data[i][2]) > depth) {
			free(pixels);
			return NULL;
		}
	}
	return pixels;
}

void destroy_rgb(struct rgb *data)
{
	free(data);
}

struct ppm *create_ppm(uint8_t **data, unsigned int width, unsigned int height,
		uint8_t depth)
{
	struct ppm *image = malloc(sizeof(*image));
	if (!image)
		return NULL;
	image->data = convert_to_rgb(data, width, height, depth);
	if (!image->data) {
		free(image);
		return NULL;
	}
	image->width = width;
	image->height = height;
	image->depth = depth;
	return image;
}

void destroy_ppm(struct ppm *image)
{
	if (image->data)
		destroy_rgb(image->data);
	free(image);
}

// returns 0 on success
int save_ppm_to_file(struct ppm *image, const char *filename)
{
	FILE *f = fopen(filename, "w");
	if (!f)
		return -1;
	if (fprintf(f, "P3 %u %u %hhu\n", image->width, image->height,
				image->depth) == EOF) {
		fclose(f);
		return -1;
	}
	struct rgb pixel;
	for (unsigned int row = 0; row < image->height; ++row) {
		for (unsigned int col = 0; col < image->width; ++col) {
			pixel = image->data[(row * image->width) + col];
			if (fprintf(f, "%hhu ", pixel.red) == EOF
					|| fprintf(f, "%hhu ", pixel.green) == EOF
					|| fprintf(f, "%hhu ", pixel.blue) == EOF) {
				fclose(f);
				return -1;
			}
		}
		if (fputc('\n', f) == EOF) {
			fclose(f);
			return -1;
		}
	}
	fclose(f);
	return 0;
}

