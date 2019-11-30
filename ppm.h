#ifndef __PPM_H__
#define __PPM_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct rgb {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

struct ppm {
	unsigned int width;
	unsigned int height;
	uint8_t depth;
	struct rgb *data;
};

/* data has to be an array of length 3 arries. width and height correspond to the 
 * images width and height. depth is the maximum colour value, upperbounded by 255
 */
struct ppm *create_ppm(uint8_t **data, unsigned int width, unsigned int height,
		uint8_t depth);

void destroy_ppm(struct ppm *image);

/* returns 0 on success.
 */
int save_ppm_to_file(struct ppm *image, const char *filename);

#endif
