#include "sqroot_image_operation_class.h"

SQROOT_IMAGE_OPERATION::SQROOT_IMAGE_OPERATION()
{
	input_numeric_nodes_required = 1;
	NODE_SCALAR<char*> node_identifier("node_A");
	numeric_nodes_names_list.assignNodeValue(0, node_identifier);

	NODE_SCALAR<double> local_node_A(0.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, &local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);
}



SQROOT_IMAGE_OPERATION::SQROOT_IMAGE_OPERATION(const SQROOT_IMAGE_OPERATION & src_sum_image_operation)
{
	copyFromImageOperation(src_sum_image_operation);
}



SQROOT_IMAGE_OPERATION SQROOT_IMAGE_OPERATION::operator=(const SQROOT_IMAGE_OPERATION & src_sum_image_operation)
{
	if (this != &src_sum_image_operation)
	{
		copyFromImageOperation(src_sum_image_operation);
	}

	return *this;
}



SQROOT_IMAGE_OPERATION::~SQROOT_IMAGE_OPERATION()
{
	// Nothing to deallocate
}




void SQROOT_IMAGE_OPERATION::performOperation()
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
		case RBT_TARGET:
		case RBT_SOURCE:
			for (int y = roi_y_ini; y <= roi_y_end; y++)
			{
				for (int x = roi_x_ini; x <= roi_x_end; x++)
				{
					const double d_intensity = sqrt((numeric_nodes_list.getNodeValue(0)->getScalarValue()) * *(src_img_A->image_data + (y - ULa_y)* width_A + x - ULa_x));

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