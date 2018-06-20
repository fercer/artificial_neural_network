#include "ones_image_operation_class.h"

ONES_IMAGE_OPERATION::ONES_IMAGE_OPERATION()
{
	width_parameter.setScalarValue(0.0);
	height_parameter.setScalarValue(0.0);
	fill_parameter.setScalarValue(1.0);

	numeric_parameters_nodes_list.assignNodeValue(0, &width_parameter);
	numeric_parameters_nodes_list.assignNodeValue(1, &height_parameter);
	numeric_parameters_nodes_list.assignNodeValue(2, &fill_parameter);
}



ONES_IMAGE_OPERATION::ONES_IMAGE_OPERATION(const ONES_IMAGE_OPERATION & src_ones_image_operation)
{
	copyFromImageOperation(src_ones_image_operation);

	/* Verify if the parameters are connected to an outer node pointer,
	or if them are connected to the default nodes of the source:
	*/
	this->numeric_parameters_nodes_list = src_ones_image_operation.numeric_parameters_nodes_list;

	NODE_SCALAR<double> * src_width_parameter_pointer = this->numeric_parameters_nodes_list.getNodeValue(0);
	NODE_SCALAR<double> * src_height_parameter_pointer = this->numeric_parameters_nodes_list.getNodeValue(1);
	NODE_SCALAR<double> * src_fill_parameter_pointer = this->numeric_parameters_nodes_list.getNodeValue(2);

	if (src_width_parameter_pointer == &src_ones_image_operation.width_parameter)
	{
		this->width_parameter.setScalarValue(src_width_parameter_pointer->getScalarValue());
		this->numeric_parameters_nodes_list.assignNodeValue(0, &this->width_parameter);
	}

	if (src_height_parameter_pointer == &src_ones_image_operation.height_parameter)
	{
		this->height_parameter.setScalarValue(src_height_parameter_pointer->getScalarValue());
		this->numeric_parameters_nodes_list.assignNodeValue(0, &this->height_parameter);
	}

	if (src_fill_parameter_pointer == &src_ones_image_operation.fill_parameter)
	{
		this->fill_parameter.setScalarValue(src_fill_parameter_pointer->getScalarValue());
		this->numeric_parameters_nodes_list.assignNodeValue(0, &this->fill_parameter);
	}
}



ONES_IMAGE_OPERATION ONES_IMAGE_OPERATION::operator=(const ONES_IMAGE_OPERATION & src_ones_image_operation)
{
	if (this != &src_ones_image_operation)
	{
		copyFromImageOperation(src_ones_image_operation);

		/* Verify if the parameters are connected to an outer node pointer,
		or if them are connected to the default nodes of the source:
		*/
		this->numeric_parameters_nodes_list = src_ones_image_operation.numeric_parameters_nodes_list;

		NODE_SCALAR<double> * src_width_parameter_pointer = this->numeric_parameters_nodes_list.getNodeValue(0);
		NODE_SCALAR<double> * src_height_parameter_pointer = this->numeric_parameters_nodes_list.getNodeValue(1);
		NODE_SCALAR<double> * src_fill_parameter_pointer = this->numeric_parameters_nodes_list.getNodeValue(2);

		if (src_width_parameter_pointer == &src_ones_image_operation.width_parameter)
		{
			this->width_parameter.setScalarValue(src_width_parameter_pointer->getScalarValue());
			this->numeric_parameters_nodes_list.assignNodeValue(0, &this->width_parameter);
		}

		if (src_height_parameter_pointer == &src_ones_image_operation.height_parameter)
		{
			this->height_parameter.setScalarValue(src_height_parameter_pointer->getScalarValue());
			this->numeric_parameters_nodes_list.assignNodeValue(0, &this->height_parameter);
		}

		if (src_fill_parameter_pointer == &src_ones_image_operation.fill_parameter)
		{
			this->fill_parameter.setScalarValue(src_fill_parameter_pointer->getScalarValue());
			this->numeric_parameters_nodes_list.assignNodeValue(0, &this->fill_parameter);
		}
	}

	return *this;
}



ONES_IMAGE_OPERATION::~ONES_IMAGE_OPERATION()
{
	// Nothing to deallocate
}



void ONES_IMAGE_OPERATION::setWidth(const unsigned int src_width)
{
	width_parameter.setScalarValue((double)src_width);
	parameters_have_changed = true;
}

void ONES_IMAGE_OPERATION::setWidth(NODE_SCALAR<double>* src_width_node)
{
	if (src_width_node)
	{
		numeric_parameters_nodes_list.assignNodeValue(0, src_width_node);
		parameters_have_changed = true;
	}
	else
	{
		width_parameter.setScalarValue(numeric_parameters_nodes_list.getNodeValue(0)->getScalarValue());
		numeric_parameters_nodes_list.assignNodeValue(0, &width_parameter);
	}
}



void ONES_IMAGE_OPERATION::setHeight(const unsigned int src_height)
{
	height_parameter.setScalarValue((double)src_height);
	parameters_have_changed = true;
}


void ONES_IMAGE_OPERATION::setHeight(NODE_SCALAR<double>* src_height_node)
{
	if (src_height_node)
	{
		numeric_parameters_nodes_list.assignNodeValue(1, src_height_node);
		parameters_have_changed = true;
	}
	else
	{
		height_parameter.setScalarValue(numeric_parameters_nodes_list.getNodeValue(1)->getScalarValue());
		numeric_parameters_nodes_list.assignNodeValue(1, &height_parameter);
	}
}



void ONES_IMAGE_OPERATION::setFillValue(const double src_fill_value)
{
	fill_parameter.setScalarValue(src_fill_value);
	parameters_have_changed = true;
}


void ONES_IMAGE_OPERATION::setFillValue(NODE_SCALAR<double>* src_fill_value_node)
{
	if (src_fill_value_node)
	{
		numeric_parameters_nodes_list.assignNodeValue(2, src_fill_value_node);
		parameters_have_changed = true;
	}
	else
	{
		fill_parameter.setScalarValue(numeric_parameters_nodes_list.getNodeValue(2)->getScalarValue());
		numeric_parameters_nodes_list.assignNodeValue(0, &fill_parameter);
	}
}



void ONES_IMAGE_OPERATION::performOperation()
{
	const unsigned int width = (unsigned int)floor(numeric_parameters_nodes_list.getNodeValue(0)->getScalarValue());
	const unsigned int height = (unsigned int)floor(numeric_parameters_nodes_list.getNodeValue(1)->getScalarValue());

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
		*(dst_img->image_data + xy) = numeric_parameters_nodes_list.getNodeValue(2)->getScalarValue();
	}
}