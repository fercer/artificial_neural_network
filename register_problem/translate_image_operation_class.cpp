#include "translate_image_operation_class.h"

TRANSLATE_IMAGE_OPERATION::TRANSLATE_IMAGE_OPERATION()
{
	// Nothing to define
}



TRANSLATE_IMAGE_OPERATION::TRANSLATE_IMAGE_OPERATION(const TRANSLATE_IMAGE_OPERATION & src_translate_image_operation)
{
	copyFromImageOperation(src_translate_image_operation);
}



TRANSLATE_IMAGE_OPERATION TRANSLATE_IMAGE_OPERATION::operator=(const TRANSLATE_IMAGE_OPERATION & src_translate_image_operation)
{
	if (this != &src_translate_image_operation)
	{
		copyFromImageOperation(src_translate_image_operation);
	}

	return *this;
}



TRANSLATE_IMAGE_OPERATION::~TRANSLATE_IMAGE_OPERATION()
{
	// Nothing to deallocate
}



void TRANSLATE_IMAGE_OPERATION::performOperation()
{
	// Copy all the image data:
	if (dst_img)
	{
		copyImageData(src_img_A, dst_img);
	}
	else
	{
		dst_img = createFromImageData(src_img_A);
	}
	
	// Modify only the ROI boundaries:
	/// Modify the area ROI:
	dst_img->head_roi.UL_x += parameter_A;
	dst_img->head_roi.UL_y += parameter_B;
	dst_img->head_roi.UR_x += parameter_A;
	dst_img->head_roi.UR_y += parameter_B;
	dst_img->head_roi.LR_x += parameter_A;
	dst_img->head_roi.LR_y += parameter_B;
	dst_img->head_roi.LL_x += parameter_A;
	dst_img->head_roi.LL_y += parameter_B;

	ROI_BBOX * next_roi_node = dst_img->head_roi.next_roi;
	ROI_BBOX * current_roi_node;

	while (next_roi_node)
	{
		current_roi_node = next_roi_node;
		next_roi_node = current_roi_node->next_roi;

		current_roi_node->UL_x += parameter_A;
		current_roi_node->UL_y += parameter_B;
		current_roi_node->UR_x += parameter_A;
		current_roi_node->UR_y += parameter_B;
		current_roi_node->LR_x += parameter_A;
		current_roi_node->LR_y += parameter_B;
		current_roi_node->LL_x += parameter_A;
		current_roi_node->LL_y += parameter_B;
	}
}
