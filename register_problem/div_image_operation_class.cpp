#include "div_image_operation_class.h"

DIV_IMAGE_OPERATION::DIV_IMAGE_OPERATION()
{
	input_operations_required = 2;
	minimum_input_operations_required = 1;

	input_numeric_nodes_required = 2;
	NODE_SCALAR<char*> * node_A_identifier = new NODE_SCALAR<char*>("node_A");
	numeric_nodes_names_list.assignNodeValue(0, node_A_identifier);

	NODE_SCALAR<char*> * node_B_identifier = new NODE_SCALAR<char*>("node_B");
	numeric_nodes_names_list.assignNodeValue(1, node_B_identifier);

	NODE_SCALAR<double> * local_node_A = new NODE_SCALAR<double>(1.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<double> * local_node_B = new NODE_SCALAR<double>(1.0);
	local_numeric_nodes_list.assignNodeValue(1, local_node_B);
	numeric_nodes_list.assignNodeValue(1, local_numeric_nodes_list.getNodeValue(1));
	numeric_node_is_local_list.assignNodeValue(1, true);

	NODE_SCALAR<double> * local_previous_node_A = new NODE_SCALAR<double>(1.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(0, local_previous_node_A);

	NODE_SCALAR<double> * local_previous_node_B = new NODE_SCALAR<double>(1.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(1, local_previous_node_B);
}



DIV_IMAGE_OPERATION::DIV_IMAGE_OPERATION(const DIV_IMAGE_OPERATION & src_div_image_operation)
{
	input_operations_required = 2;
	minimum_input_operations_required = 1;

	input_numeric_nodes_required = 2;
	NODE_SCALAR<char*> * node_A_identifier = new NODE_SCALAR<char*>("node_A");
	numeric_nodes_names_list.assignNodeValue(0, node_A_identifier);

	NODE_SCALAR<char*> * node_B_identifier = new NODE_SCALAR<char*>("node_B");
	numeric_nodes_names_list.assignNodeValue(1, node_B_identifier);

	NODE_SCALAR<double> * local_node_A = new NODE_SCALAR<double>(1.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<double> * local_node_B = new NODE_SCALAR<double>(1.0);
	local_numeric_nodes_list.assignNodeValue(1, local_node_B);
	numeric_nodes_list.assignNodeValue(1, local_numeric_nodes_list.getNodeValue(1));
	numeric_node_is_local_list.assignNodeValue(1, true);

	NODE_SCALAR<double> * local_previous_node_A = new NODE_SCALAR<double>(1.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(0, local_previous_node_A);

	NODE_SCALAR<double> * local_previous_node_B = new NODE_SCALAR<double>(1.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(1, local_previous_node_B);

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
						numeric_nodes_list.getNodeValue(1)->getScalarValue() * *(src_img_B->image_data + (y - ULb_y)* width_B + x - ULb_x) *
						numeric_nodes_list.getNodeValue(0)->getScalarValue() * *(src_img_A->image_data + (y - ULa_y)* width_A + x - ULa_x);

					*(dst_img->image_data + (y - ULg_y) * computable_width + x - ULg_x) = d_intensity;
				}
			}
			break;
		case RBT_SOURCE:
		case RBT_TARGET:
		case RBT_AREA:
		case RBT_UNCOMPUTED:
			break;
		}
	}
}