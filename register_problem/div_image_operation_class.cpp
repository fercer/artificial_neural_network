#include "div_image_operation_class.h"

DIV_IMAGE_OPERATION::DIV_IMAGE_OPERATION()
{
	parameter_A = 1.0;
	parameter_B = 1.0;
}



DIV_IMAGE_OPERATION::DIV_IMAGE_OPERATION(const DIV_IMAGE_OPERATION & src_div_image_operation)
{
	copyFromImageOperation(src_div_image_operation);
}



DIV_IMAGE_OPERATION DIV_IMAGE_OPERATION::operator=(const DIV_IMAGE_OPERATION & src_div_image_operation)
{
	if (this != &src_div_image_operation)
	{
		copyFromImageOperation(src_div_image_operation);
	}

	return *this;
}



DIV_IMAGE_OPERATION::~DIV_IMAGE_OPERATION()
{
	// Nothing to deallocate
}



void DIV_IMAGE_OPERATION::performOperation()
{
	computeActiveROI();
	
	ROI_BBOX * next_roi = dst_img->head_roi.next_roi;
	ROI_BBOX * current_roi;

	while (next_roi)
	{
		current_roi = next_roi;
		next_roi = current_roi->next_roi;

		const unsigned int roi_x_ini = current_roi->UL_x;
		const unsigned int roi_x_end = current_roi->LR_x;
		const unsigned int roi_y_ini = current_roi->UL_y;
		const unsigned int roi_y_end = current_roi->LR_y;

		switch (current_roi->ROI_type)
		{
		case RBT_INTERSECTION:
			for (int y = roi_y_ini; y <= roi_y_end; y++)
			{
				for (int x = roi_x_ini; x <= roi_x_end; x++)
				{
					const double d_intensity = 
						parameter_B * *(src_img_B->image_data + (y - ULb_y)* width_B + x - ULb_x) * 
						parameter_A * *(src_img_A->image_data + (y - ULa_y)* width_A + x - ULa_x);

					*(dst_img->image_data + (y - ULg_y) * computable_width + x - ULg_x) = d_intensity;
				}
			}
			break;

		case RBT_TARGET:
			for (int y = roi_y_ini; y <= roi_y_end; y++)
			{
				for (int x = roi_x_ini; x <= roi_x_end; x++)
				{
					const double d_intensity = parameter_B * *(src_img_B->image_data + (y - ULb_y)* width_B + x - ULb_x);

					*(dst_img->image_data + (y - ULg_y) * computable_width + x - ULg_x) = d_intensity;
				}
			}
			break;

		case RBT_SOURCE:
			for (int y = roi_y_ini; y <= roi_y_end; y++)
			{
				for (int x = roi_x_ini; x <= roi_x_end; x++)
				{
					const double d_intensity = parameter_A * *(src_img_A->image_data + (y - ULa_y)* width_A + x - ULa_x);

					*(dst_img->image_data + (y - ULg_y) * computable_width + x - ULg_x) = d_intensity;
				}
			}
			break;

		case RBT_AREA:
		case RBT_UNCOMPUTED:
			break;
		}
	}
}