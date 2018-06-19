#include "div_image_operation_class.h"

DIV_IMAGE_OPERATION::DIV_IMAGE_OPERATION()
{
	parameter_A.setScalarValue(1.0);
	parameter_B.setScalarValue(1.0);

	numeric_parameters_nodes_list.assignNodeValue(0, &parameter_A);
	numeric_parameters_nodes_list.assignNodeValue(1, &parameter_B);
}



DIV_IMAGE_OPERATION::DIV_IMAGE_OPERATION(const DIV_IMAGE_OPERATION & src_div_image_operation)
{
	copyFromImageOperation(src_div_image_operation);

	/* Verify if the parameters are connected to an outer node pointer,
	or if them are connected to the default nodes of the source:
	*/
	this->numeric_parameters_nodes_list = src_div_image_operation.numeric_parameters_nodes_list;

	NODE_SCALAR<double> * src_parameter_A_pointer = this->numeric_parameters_nodes_list.getNodeValue(0);
	NODE_SCALAR<double> * src_parameter_B_pointer = this->numeric_parameters_nodes_list.getNodeValue(1);

	if (src_parameter_A_pointer == &src_div_image_operation.parameter_A)
	{
		this->parameter_A.setScalarValue(src_parameter_A_pointer->getScalarValue());
		this->numeric_parameters_nodes_list.assignNodeValue(0, &this->parameter_A);
	}

	if (src_parameter_B_pointer == &src_div_image_operation.parameter_B)
	{
		this->parameter_B.setScalarValue(src_parameter_B_pointer->getScalarValue());
		this->numeric_parameters_nodes_list.assignNodeValue(1, &this->parameter_B);
	}
}



DIV_IMAGE_OPERATION DIV_IMAGE_OPERATION::operator=(const DIV_IMAGE_OPERATION & src_div_image_operation)
{
	if (this != &src_div_image_operation)
	{
		copyFromImageOperation(src_div_image_operation);

		/* Verify if the parameters are connected to an outer node pointer,
		or if them are connected to the default nodes of the source:
		*/
		this->numeric_parameters_nodes_list = src_div_image_operation.numeric_parameters_nodes_list;

		NODE_SCALAR<double> * src_parameter_A_pointer = this->numeric_parameters_nodes_list.getNodeValue(0);
		NODE_SCALAR<double> * src_parameter_B_pointer = this->numeric_parameters_nodes_list.getNodeValue(1);

		if (src_parameter_A_pointer == &src_div_image_operation.parameter_A)
		{
			this->parameter_A.setScalarValue(src_parameter_A_pointer->getScalarValue());
			this->numeric_parameters_nodes_list.assignNodeValue(0, &this->parameter_A);
		}

		if (src_parameter_B_pointer == &src_div_image_operation.parameter_B)
		{
			this->parameter_B.setScalarValue(src_parameter_B_pointer->getScalarValue());
			this->numeric_parameters_nodes_list.assignNodeValue(1, &this->parameter_B);
		}
	}

	return *this;
}



DIV_IMAGE_OPERATION::~DIV_IMAGE_OPERATION()
{
	// Nothing to deallocate
}

void DIV_IMAGE_OPERATION::setParameterA(const double src_parameter_A)
{
	parameter_A.setScalarValue(src_parameter_A);
	parameters_have_changed = true;
}

void DIV_IMAGE_OPERATION::setParameterA(NODE_SCALAR<double>* src_node_A)
{
	numeric_parameters_nodes_list.assignNodeValue(0, src_node_A);
	parameters_have_changed = true;
}

void DIV_IMAGE_OPERATION::setParameterB(const double src_parameter_B)
{
	parameter_B.setScalarValue(src_parameter_B);
	parameters_have_changed = true;
}

void DIV_IMAGE_OPERATION::setParameterB(NODE_SCALAR<double>* src_node_B)
{
	numeric_parameters_nodes_list.assignNodeValue(1, src_node_B);
	parameters_have_changed = true;
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
						numeric_parameters_nodes_list.getNodeValue(1)->getScalarValue() * *(src_img_B->image_data + (y - ULb_y)* width_B + x - ULb_x) *
						numeric_parameters_nodes_list.getNodeValue(0)->getScalarValue() * *(src_img_A->image_data + (y - ULa_y)* width_A + x - ULa_x);

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