#include "sum_image_operation_class.h"

SUM_IMAGE_OPERATION::SUM_IMAGE_OPERATION()
{
	parameter_A.setScalarValue(1.0);
	parameter_B.setScalarValue(1.0);

	numeric_parameters_nodes_list.assignNodeValue(0, &parameter_A);
	numeric_parameters_nodes_list.assignNodeValue(1, &parameter_B);
}



SUM_IMAGE_OPERATION::SUM_IMAGE_OPERATION(const SUM_IMAGE_OPERATION & src_sum_image_operation)
{
	copyFromImageOperation(src_sum_image_operation);
}



SUM_IMAGE_OPERATION SUM_IMAGE_OPERATION::operator=(const SUM_IMAGE_OPERATION & src_sum_image_operation)
{
	if (this != &src_sum_image_operation)
	{
		copyFromImageOperation(src_sum_image_operation);
	}

	return *this;
}

SUM_IMAGE_OPERATION::~SUM_IMAGE_OPERATION()
{
	// Nothing to deallocate
}

void SUM_IMAGE_OPERATION::assignParameterA(const double src_parameter_A)
{
	parameter_A.setScalarValue(src_parameter_A);
}

void SUM_IMAGE_OPERATION::assignParameterB(const double src_parameter_B)
{
	parameter_B.setScalarValue(src_parameter_B);
}



void SUM_IMAGE_OPERATION::performOperation()
{
	computeActiveROI();

	ROI_BBOX * next_roi = dst_img->head_roi.next_roi;
	ROI_BBOX * current_roi;

	while (next_roi)
	{
		current_roi = next_roi;
		next_roi = current_roi->next_roi;

		const int roi_x_ini = current_roi->UL_x;
		const int roi_x_end = current_roi->LR_x;
		const int roi_y_ini = current_roi->UL_y;
		const int roi_y_end = current_roi->LR_y;

		switch (current_roi->ROI_type)
		{
		case RBT_INTERSECTION:
			for (int y = roi_y_ini; y <= roi_y_end; y++)
			{
				for (int x = roi_x_ini; x <= roi_x_end; x++)
				{
					const double d_intensity = 
						(numeric_parameters_nodes_list.getNodeValue(1)->getScalarValue()) * *(src_img_B->image_data + (y - ULb_y)* width_B + x - ULb_x) + 
						(numeric_parameters_nodes_list.getNodeValue(0)->getScalarValue()) * *(src_img_A->image_data + (y - ULa_y)* width_A + x - ULa_x);

					*(dst_img->image_data + (y - ULg_y) * computable_width + x - ULg_x) = d_intensity;
				}
			}
			break;

		case RBT_TARGET:
			for (int y = roi_y_ini; y <= roi_y_end; y++)
			{
				for (int x = roi_x_ini; x <= roi_x_end; x++)
				{
					const double d_intensity = (numeric_parameters_nodes_list.getNodeValue(1)->getScalarValue()) * *(src_img_B->image_data + (y - ULb_y)* width_B + x - ULb_x);

					*(dst_img->image_data + (y - ULg_y) * computable_width + x - ULg_x) = d_intensity;
				}
			}
			break;

		case RBT_SOURCE:
			for (int y = roi_y_ini; y <= roi_y_end; y++)
			{
				for (int x = roi_x_ini; x <= roi_x_end; x++)
				{
					const double d_intensity = (numeric_parameters_nodes_list.getNodeValue(0)->getScalarValue()) * *(src_img_A->image_data + (y - ULa_y) * width_A + x - ULa_x);

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