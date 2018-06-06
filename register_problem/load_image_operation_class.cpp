#include "load_image_operation_class.h"

LOAD_IMAGE_OPERATION::LOAD_IMAGE_OPERATION()
{
	// Nothing to define
}



LOAD_IMAGE_OPERATION::LOAD_IMAGE_OPERATION(const LOAD_IMAGE_OPERATION & src_load_image_operation)
{
	copyFromImageOperation(src_load_image_operation);
}



LOAD_IMAGE_OPERATION LOAD_IMAGE_OPERATION::operator=(const LOAD_IMAGE_OPERATION & src_load_image_operation)
{
	if (this != &src_load_image_operation)
	{
		copyFromImageOperation(src_load_image_operation);
	}

	return *this;
}



LOAD_IMAGE_OPERATION::~LOAD_IMAGE_OPERATION()
{
	// Nothing to deallocate
}



void LOAD_IMAGE_OPERATION::performOperation()
{
	FILE * fp_img = fopen(image_name, "r");
	if (!fp_img)
	{
		return;
	}

	char read_line[512];

	// Read magic number:
	fscanf(fp_img, "%s\n", read_line);

	// Read the commentary
	fgets(read_line, 512, fp_img);

	int width, height;
	fscanf(fp_img, "%i %i", &width, &height);


	int max_intensity;
	fscanf(fp_img, "%i", &max_intensity);

	dst_img = createVoidImage(width, height);

	unsigned int pix_intensity;
	for (unsigned int pix_position = 0; pix_position < (width*height); pix_position++)
	{
		fscanf(fp_img, "%u", &pix_intensity);
		*(dst_img->image_data + pix_position) = (double)(unsigned int)pix_intensity / (double)max_intensity;
	}

	fclose(fp_img);
}