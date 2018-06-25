#include "ones_image_operation_class.h"

ONES_IMAGE_OPERATION::ONES_IMAGE_OPERATION()
{
	input_numeric_nodes_required = 3;

	NODE_SCALAR<char*> * node_width_identifier = new NODE_SCALAR<char*>("node_width");
	numeric_nodes_names_list.assignNodeValue(0, node_width_identifier);

	NODE_SCALAR<char*> * node_height_identifier = new NODE_SCALAR<char*>("node_height");
	numeric_nodes_names_list.assignNodeValue(1, node_height_identifier);

	NODE_SCALAR<char*> * node_fill_identifier = new NODE_SCALAR<char*>("node_fill_value");
	numeric_nodes_names_list.assignNodeValue(2, node_fill_identifier);

	NODE_SCALAR<double> * local_node_A = new NODE_SCALAR<double>(0.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<double> * local_node_B = new NODE_SCALAR<double>(0.0);
	local_numeric_nodes_list.assignNodeValue(1, local_node_B);
	numeric_nodes_list.assignNodeValue(1, local_numeric_nodes_list.getNodeValue(1));
	numeric_node_is_local_list.assignNodeValue(1, true);

	NODE_SCALAR<double> * local_node_C = new NODE_SCALAR<double>(1.0);
	local_numeric_nodes_list.assignNodeValue(2, local_node_C);
	numeric_nodes_list.assignNodeValue(2, local_numeric_nodes_list.getNodeValue(2));
	numeric_node_is_local_list.assignNodeValue(2, true);
}



ONES_IMAGE_OPERATION::ONES_IMAGE_OPERATION(const ONES_IMAGE_OPERATION & src_ones_image_operation)
{
	input_numeric_nodes_required = 3;

	NODE_SCALAR<char*> * node_width_identifier = new NODE_SCALAR<char*>("node_width");
	numeric_nodes_names_list.assignNodeValue(0, node_width_identifier);

	NODE_SCALAR<char*> * node_height_identifier = new NODE_SCALAR<char*>("node_height");
	numeric_nodes_names_list.assignNodeValue(1, node_height_identifier);

	NODE_SCALAR<char*> * node_fill_identifier = new NODE_SCALAR<char*>("node_fill_value");
	numeric_nodes_names_list.assignNodeValue(2, node_fill_identifier);

	NODE_SCALAR<double> * local_node_A = new NODE_SCALAR<double>(0.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<double> * local_node_B = new NODE_SCALAR<double>(0.0);
	local_numeric_nodes_list.assignNodeValue(1, local_node_B);
	numeric_nodes_list.assignNodeValue(1, local_numeric_nodes_list.getNodeValue(1));
	numeric_node_is_local_list.assignNodeValue(1, true);

	NODE_SCALAR<double> * local_node_C = new NODE_SCALAR<double>(1.0);
	local_numeric_nodes_list.assignNodeValue(2, local_node_C);
	numeric_nodes_list.assignNodeValue(2, local_numeric_nodes_list.getNodeValue(2));
	numeric_node_is_local_list.assignNodeValue(2, true);

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

}


void ONES_IMAGE_OPERATION::performOperation()
{
	const unsigned int width = (unsigned int)floor(numeric_nodes_list.getNodeValue(0)->getScalarValue());
	const unsigned int height = (unsigned int)floor(numeric_nodes_list.getNodeValue(1)->getScalarValue());

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
		*(dst_img->image_data + xy) = numeric_nodes_list.getNodeValue(2)->getScalarValue();
	}
}