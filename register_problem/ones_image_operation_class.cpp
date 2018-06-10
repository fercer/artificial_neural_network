#include "ones_image_operation_class.h"

ONES_IMAGE_OPERATION::ONES_IMAGE_OPERATION()
{
	parameter_A = 1.0;
	parameter_B = 1.0;
	parameter_C = 1.0;
}



ONES_IMAGE_OPERATION::ONES_IMAGE_OPERATION(const ONES_IMAGE_OPERATION & src_ones_image_operation)
{
	copyFromImageOperation(src_ones_image_operation);
}



ONES_IMAGE_OPERATION ONES_IMAGE_OPERATION::operator=(const ONES_IMAGE_OPERATION & src_ones_image_operation)
{
	if (this != &src_ones_image_operation)
	{
		copyFromImageOperation(src_ones_image_operation);
	}

	return *this;
}

ONES_IMAGE_OPERATION::~ONES_IMAGE_OPERATION()
{
	// Nothing to deallocate
}



void ONES_IMAGE_OPERATION::performOperation()
{
	const unsigned int width = (unsigned int)floor(parameter_A);
	const unsigned int height = (unsigned int)floor(parameter_B);

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

	const int half_width = width / 2;
	const int half_height = height / 2;
	
	dst_img->head_roi.ROI_type = RBT_AREA;
	dst_img->head_roi.UL_x = -half_width;
	dst_img->head_roi.UL_y = -half_height;
	dst_img->head_roi.UR_x = -half_width + width - 1;
	dst_img->head_roi.UR_y = -half_height;
	dst_img->head_roi.LR_x = -half_width + width - 1;
	dst_img->head_roi.LR_y = -half_height + height - 1;
	dst_img->head_roi.LL_x = -half_width;
	dst_img->head_roi.LL_y = -half_height + height - 1;

	
	for (unsigned int xy = 0; xy < height*width; xy++)
	{
		*(dst_img->image_data + xy) = parameter_C;
	}
}