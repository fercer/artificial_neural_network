#include "translate_image_operation_class.h"

TRANSLATE_IMAGE_OPERATION::TRANSLATE_IMAGE_OPERATION()
{
	parameter_dx.setScalarValue(0.0);
	parameter_dy.setScalarValue(0.0);

	numeric_parameters_nodes_list.assignNodeValue(0, &parameter_dx);
	numeric_parameters_nodes_list.assignNodeValue(1, &parameter_dy);
}



TRANSLATE_IMAGE_OPERATION::TRANSLATE_IMAGE_OPERATION(const TRANSLATE_IMAGE_OPERATION & src_translate_image_operation)
{
	copyFromImageOperation(src_translate_image_operation);

	/* Verify if the parameters are connected to an outer node pointer,
	or if them are connected to the default nodes of the source:
	*/
	this->numeric_parameters_nodes_list = src_translate_image_operation.numeric_parameters_nodes_list;

	NODE_SCALAR<double> * src_parameter_dx_pointer = this->numeric_parameters_nodes_list.getNodeValue(0);
	NODE_SCALAR<double> * src_parameter_dy_pointer = this->numeric_parameters_nodes_list.getNodeValue(1);

	if (src_parameter_dx_pointer == &src_translate_image_operation.parameter_dx)
	{
		this->parameter_dx.setScalarValue(src_parameter_dx_pointer->getScalarValue());
		this->numeric_parameters_nodes_list.assignNodeValue(0, &this->parameter_dx);
	}

	if (src_parameter_dy_pointer == &src_translate_image_operation.parameter_dy)
	{
		this->parameter_dy.setScalarValue(src_parameter_dy_pointer->getScalarValue());
		this->numeric_parameters_nodes_list.assignNodeValue(1, &this->parameter_dy);
	}
}



TRANSLATE_IMAGE_OPERATION TRANSLATE_IMAGE_OPERATION::operator=(const TRANSLATE_IMAGE_OPERATION & src_translate_image_operation)
{
	if (this != &src_translate_image_operation)
	{
		copyFromImageOperation(src_translate_image_operation);

		/* Verify if the parameters are connected to an outer node pointer,
		or if them are connected to the default nodes of the source:
		*/
		this->numeric_parameters_nodes_list = src_translate_image_operation.numeric_parameters_nodes_list;

		NODE_SCALAR<double> * src_parameter_dx_pointer = this->numeric_parameters_nodes_list.getNodeValue(0);
		NODE_SCALAR<double> * src_parameter_dy_pointer = this->numeric_parameters_nodes_list.getNodeValue(1);

		if (src_parameter_dx_pointer == &src_translate_image_operation.parameter_dx)
		{
			this->parameter_dx.setScalarValue(src_parameter_dx_pointer->getScalarValue());
			this->numeric_parameters_nodes_list.assignNodeValue(0, &this->parameter_dx);
		}

		if (src_parameter_dy_pointer == &src_translate_image_operation.parameter_dy)
		{
			this->parameter_dy.setScalarValue(src_parameter_dy_pointer->getScalarValue());
			this->numeric_parameters_nodes_list.assignNodeValue(1, &this->parameter_dy);
		}
	}

	return *this;
}



TRANSLATE_IMAGE_OPERATION::~TRANSLATE_IMAGE_OPERATION()
{
	// Nothing to deallocate
}



void TRANSLATE_IMAGE_OPERATION::setDeltaX(const double src_delta_x)
{
	parameter_dx.setScalarValue(src_delta_x);
	parameters_have_changed = true;
}

void TRANSLATE_IMAGE_OPERATION::setDeltaX(NODE_SCALAR<double>* src_delta_x_node)
{
	if (src_delta_x_node)
	{
		numeric_parameters_nodes_list.assignNodeValue(0, src_delta_x_node);
		parameters_have_changed = true;
	}
	else
	{
		parameter_dx.setScalarValue(numeric_parameters_nodes_list.getNodeValue(0)->getScalarValue());
		numeric_parameters_nodes_list.assignNodeValue(0, &parameter_dx);
	}
}


void TRANSLATE_IMAGE_OPERATION::setDeltaY(const double src_delta_y)
{
	parameter_dy.setScalarValue(src_delta_y);
	parameters_have_changed = true;
}


void TRANSLATE_IMAGE_OPERATION::setDeltaY(NODE_SCALAR<double>* src_delta_y_node)
{
	if (src_delta_y_node)
	{
		numeric_parameters_nodes_list.assignNodeValue(1, src_delta_y_node);
		parameters_have_changed = true;
	}
	else
	{
		parameter_dy.setScalarValue(numeric_parameters_nodes_list.getNodeValue(1)->getScalarValue());
		numeric_parameters_nodes_list.assignNodeValue(1, &parameter_dy);
	}
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
	dst_img->head_roi.UL_x += (numeric_parameters_nodes_list.getNodeValue(0))->getScalarValue();
	dst_img->head_roi.UL_y += (numeric_parameters_nodes_list.getNodeValue(1))->getScalarValue();
	dst_img->head_roi.UR_x += (numeric_parameters_nodes_list.getNodeValue(0))->getScalarValue();
	dst_img->head_roi.UR_y += (numeric_parameters_nodes_list.getNodeValue(1))->getScalarValue();
	dst_img->head_roi.LR_x += (numeric_parameters_nodes_list.getNodeValue(0))->getScalarValue();
	dst_img->head_roi.LR_y += (numeric_parameters_nodes_list.getNodeValue(1))->getScalarValue();
	dst_img->head_roi.LL_x += (numeric_parameters_nodes_list.getNodeValue(0))->getScalarValue();
	dst_img->head_roi.LL_y += (numeric_parameters_nodes_list.getNodeValue(1))->getScalarValue();

	ROI_BBOX * next_roi_node = dst_img->head_roi.next_roi;
	ROI_BBOX * current_roi_node;

	while (next_roi_node)
	{
		current_roi_node = next_roi_node;
		next_roi_node = current_roi_node->next_roi;

		current_roi_node->UL_x += (numeric_parameters_nodes_list.getNodeValue(0))->getScalarValue();
		current_roi_node->UL_y += (numeric_parameters_nodes_list.getNodeValue(1))->getScalarValue();;
		current_roi_node->UR_x += (numeric_parameters_nodes_list.getNodeValue(0))->getScalarValue();
		current_roi_node->UR_y += (numeric_parameters_nodes_list.getNodeValue(1))->getScalarValue();;
		current_roi_node->LR_x += (numeric_parameters_nodes_list.getNodeValue(0))->getScalarValue();
		current_roi_node->LR_y += (numeric_parameters_nodes_list.getNodeValue(1))->getScalarValue();;
		current_roi_node->LL_x += (numeric_parameters_nodes_list.getNodeValue(0))->getScalarValue();
		current_roi_node->LL_y += (numeric_parameters_nodes_list.getNodeValue(1))->getScalarValue();;
	}
}
