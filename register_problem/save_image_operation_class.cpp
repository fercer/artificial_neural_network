#include "save_image_operation_class.h"

SAVE_IMAGE_OPERATION::SAVE_IMAGE_OPERATION()
{
	// Nothing to define
}



SAVE_IMAGE_OPERATION::SAVE_IMAGE_OPERATION(const SAVE_IMAGE_OPERATION & src_save_image_operation)
{
	copyFromImageOperation(src_save_image_operation);
}



SAVE_IMAGE_OPERATION SAVE_IMAGE_OPERATION::operator=(const SAVE_IMAGE_OPERATION & src_save_image_operation)
{
	if (this != &src_save_image_operation)
	{
		copyFromImageOperation(src_save_image_operation);
	}

	return *this;
}



SAVE_IMAGE_OPERATION::~SAVE_IMAGE_OPERATION()
{
	// Nothing to deallocate
}



void SAVE_IMAGE_OPERATION::performOperation()
{
	FILE * fp_img = fopen(image_name, "w");

	// Print magic number:
	fprintf(fp_img, "P2\n");

	// Print the commentary:
	fprintf(fp_img, "#Created by FerCer\n");

	// Print the width and height:
	fprintf(fp_img, "%i %i\n", width_A, height_A);

	const double min_intensity = src_img_A->min_value;
	const double max_intensity = src_img_A->max_value;
	fprintf(fp_img, "255\n");

	unsigned char pix_intensity;
	for (unsigned int pix_position = 0; pix_position < (width_A*height_A); pix_position++)
	{
		fprintf(fp_img, "%u\n", (unsigned int)(255.0 * (*(src_img_A->image_data + pix_position) - min_intensity) / (max_intensity - min_intensity)));
	}

	fclose(fp_img);
}