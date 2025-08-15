#include "ppm.h"

// assumes data is an array of uint8_t arries
// (of length 3 for the three color channels)
struct rgb_ppm *convert_to_rgb_ppm(uint8_t **data, unsigned int width,
		unsigned int height, uint8_t depth)
{
	const unsigned int len = width * height;
	struct rgb_ppm *pixels = malloc(sizeof(*pixels) * len);
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

void destroy_rgb_ppm(struct rgb_ppm *data)
{
	free(data);
}

struct ppm *create_ppm(uint8_t **data, unsigned int width, unsigned int height,
		uint8_t depth)
{
	struct ppm *image = malloc(sizeof(*image));
	if (!image)
		return NULL;
	image->data = convert_to_rgb_ppm(data, width, height, depth);
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
		destroy_rgb_ppm(image->data);
	free(image);
}

static inline int write_pixel(FILE *f, struct rgb_ppm pixel)
{
	uint8_t data[3] = {pixel.red, pixel.green, pixel.blue};
	if (fwrite(data, sizeof(*data), sizeof(data), f) != sizeof(data))
		return 0;

	return 1;
}

int save_ppm_to_file(struct ppm *image, const char *filename)
{
	FILE *f = fopen(filename, "wb");
	if (!f)
		return -1;
	// ppm header
	if (fprintf(f, "P6 %u %u %hhu\n", image->width, image->height,
				image->depth) < 0) {
		fclose(f);
		return -1;
	}

	struct rgb_ppm pixel;

	for (unsigned int row = 0; row < image->height; ++row) {
		for (unsigned int col = 0; col < image->width; ++col) {
			pixel = image->data[(row * image->width) + col];
			if (!write_pixel(f, pixel)) {
				fclose(f);
				return -1;
			}
		}
	}

	fclose(f);
	return 0;
}
