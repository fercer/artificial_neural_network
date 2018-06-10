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
	if (!src_img_A)
	{
		printf("<<Error: The source image was not assigned>\n");
		return;
	}

	FILE * fp_img = fopen(image_name, "w");

	if (!fp_img)
	{
		printf("<Error: The file \"%s\" could not be created>\n", image_name);
		return;
	}

	// Print magic number:
	fprintf(fp_img, "P2\n");

	// Print the commentary:
	fprintf(fp_img, "#Created by FerCer\n");

	// Print the width and height:
	fprintf(fp_img, "%i %i\n", width_A, height_A);
	fprintf(fp_img, "255\n");

	for (unsigned int pix_position = 0; pix_position < (width_A*height_A); pix_position++)
	{
		fprintf(fp_img, "%u\n", (unsigned int)floor(255.0 * *(src_img_A->image_data + pix_position)));
	}

	fclose(fp_img);
}