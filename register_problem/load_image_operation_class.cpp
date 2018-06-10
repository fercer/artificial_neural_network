#include "load_image_operation_class.h"

LOAD_IMAGE_OPERATION::LOAD_IMAGE_OPERATION()
{

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


	if (!dst_img)
	{
		dst_img = createVoidImage(width, height);
	}


	if ((dst_img->width != width) || (dst_img->height != height))
	{
		if (dst_img->image_data)
		{
			free(dst_img->image_data);
		}
		dst_img->width = width;
		dst_img->height = height;
		dst_img->image_data = (double*)calloc(width * height, sizeof(double));
	}


	if (dst_img->head_roi.next_roi)
	{
		ROI_BBOX * next_roi_node = dst_img->head_roi.next_roi;
		ROI_BBOX * current_roi_node;

		while (next_roi_node)
		{
			current_roi_node = next_roi_node;
			next_roi_node = current_roi_node->next_roi;

			free(current_roi_node);
		}

		dst_img->head_roi.next_roi = NULL;
		dst_img->tail_roi = &dst_img->head_roi;
	}

	unsigned int pix_intensity;
	for (unsigned int pix_position = 0; pix_position < (width*height); pix_position++)
	{
		fscanf(fp_img, "%u", &pix_intensity);
		*(dst_img->image_data + pix_position) = (double)(unsigned int)pix_intensity / (double)max_intensity;
	}

	fclose(fp_img);
}