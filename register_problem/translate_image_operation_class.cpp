#include "translate_image_operation_class.h"

TRANSLATE_IMAGE_OPERATION::TRANSLATE_IMAGE_OPERATION()
{
	input_numeric_nodes_required = 2;
	NODE_SCALAR<char*> node_identifier("node_dx");
	numeric_nodes_names_list.assignNodeValue(0, node_identifier);

	node_identifier.setScalarValue("node_dy");
	numeric_nodes_names_list.assignNodeValue(1, node_identifier);

	NODE_SCALAR<double> local_node_A(0.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, &local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<double> local_node_B(0.0);
	local_numeric_nodes_list.assignNodeValue(1, local_node_B);
	numeric_nodes_list.assignNodeValue(1, &local_numeric_nodes_list.getNodeValue(1));
	numeric_node_is_local_list.assignNodeValue(1, true);
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
	dst_img->head_roi.UL_x += (unsigned int)floor((numeric_nodes_list.getNodeValue(0))->getScalarValue());
	dst_img->head_roi.UL_y += (unsigned int)floor((numeric_nodes_list.getNodeValue(1))->getScalarValue());
	dst_img->head_roi.UR_x += (unsigned int)floor((numeric_nodes_list.getNodeValue(0))->getScalarValue());
	dst_img->head_roi.UR_y += (unsigned int)floor((numeric_nodes_list.getNodeValue(1))->getScalarValue());
	dst_img->head_roi.LR_x += (unsigned int)floor((numeric_nodes_list.getNodeValue(0))->getScalarValue());
	dst_img->head_roi.LR_y += (unsigned int)floor((numeric_nodes_list.getNodeValue(1))->getScalarValue());
	dst_img->head_roi.LL_x += (unsigned int)floor((numeric_nodes_list.getNodeValue(0))->getScalarValue());
	dst_img->head_roi.LL_y += (unsigned int)floor((numeric_nodes_list.getNodeValue(1))->getScalarValue());

	ROI_BBOX * next_roi_node = dst_img->head_roi.next_roi;
	ROI_BBOX * current_roi_node;

	while (next_roi_node)
	{
		current_roi_node = next_roi_node;
		next_roi_node = current_roi_node->next_roi;

		current_roi_node->UL_x += (unsigned int)floor((numeric_nodes_list.getNodeValue(0))->getScalarValue());
		current_roi_node->UL_y += (unsigned int)floor((numeric_nodes_list.getNodeValue(1))->getScalarValue());
		current_roi_node->UR_x += (unsigned int)floor((numeric_nodes_list.getNodeValue(0))->getScalarValue());
		current_roi_node->UR_y += (unsigned int)floor((numeric_nodes_list.getNodeValue(1))->getScalarValue());
		current_roi_node->LR_x += (unsigned int)floor((numeric_nodes_list.getNodeValue(0))->getScalarValue());
		current_roi_node->LR_y += (unsigned int)floor((numeric_nodes_list.getNodeValue(1))->getScalarValue());
		current_roi_node->LL_x += (unsigned int)floor((numeric_nodes_list.getNodeValue(0))->getScalarValue());
		current_roi_node->LL_y += (unsigned int)floor((numeric_nodes_list.getNodeValue(1))->getScalarValue());
	}
}
