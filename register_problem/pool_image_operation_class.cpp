#include "pool_image_operation_class.h"

POOL_IMAGE_OPERATION::POOL_IMAGE_OPERATION()
{
	input_operations_required = 1;
	minimum_input_operations_required = 1;
	input_numeric_nodes_required = 0;

	NODE_SCALAR<char*> * node_pooling_kernel_width_identifier = new NODE_SCALAR<char*>("node_pooling_kernel_width");
	numeric_nodes_names_list.assignNodeValue(input_numeric_nodes_required, node_pooling_kernel_width_identifier);

	NODE_SCALAR<double> * local_node_A = new NODE_SCALAR<double>(2.0);
	local_numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_node_A);
	numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(input_numeric_nodes_required, true);

	NODE_SCALAR<double> * local_previous_node_A = new NODE_SCALAR<double>(2.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(input_numeric_nodes_required, local_previous_node_A);
	input_numeric_nodes_required++;


	NODE_SCALAR<char*> * node_pooling_kernel_height_identifier = new NODE_SCALAR<char*>("node_pooling_kernel_height");
	numeric_nodes_names_list.assignNodeValue(input_numeric_nodes_required, node_pooling_kernel_height_identifier);

	NODE_SCALAR<double> * local_node_B = new NODE_SCALAR<double>(2.0);
	local_numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_node_B);
	numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_numeric_nodes_list.getNodeValue(input_numeric_nodes_required));
	numeric_node_is_local_list.assignNodeValue(input_numeric_nodes_required, true);

	NODE_SCALAR<double> * local_previous_node_B = new NODE_SCALAR<double>(2.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(input_numeric_nodes_required, local_previous_node_B);
	input_numeric_nodes_required++;


	NODE_SCALAR<char*> * node_stride_x_identifier = new NODE_SCALAR<char*>("node_stride_x");
	numeric_nodes_names_list.assignNodeValue(input_numeric_nodes_required, node_stride_x_identifier);

	NODE_SCALAR<double> * local_node_C = new NODE_SCALAR<double>(2.0);
	local_numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_node_C);
	numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_numeric_nodes_list.getNodeValue(input_numeric_nodes_required));
	numeric_node_is_local_list.assignNodeValue(input_numeric_nodes_required, true);

	NODE_SCALAR<double> * local_previous_node_C = new NODE_SCALAR<double>(2.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(input_numeric_nodes_required, local_previous_node_C);
	input_numeric_nodes_required++;


	NODE_SCALAR<char*> * node_stride_y_identifier = new NODE_SCALAR<char*>("node_stride_y");
	numeric_nodes_names_list.assignNodeValue(input_numeric_nodes_required, node_stride_y_identifier);

	NODE_SCALAR<double> * local_node_D = new NODE_SCALAR<double>(2.0);
	local_numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_node_D);
	numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_numeric_nodes_list.getNodeValue(input_numeric_nodes_required));
	numeric_node_is_local_list.assignNodeValue(input_numeric_nodes_required, true);

	NODE_SCALAR<double> * local_previous_node_D = new NODE_SCALAR<double>(2.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(input_numeric_nodes_required, local_previous_node_D);
	input_numeric_nodes_required++;
}



POOL_IMAGE_OPERATION::POOL_IMAGE_OPERATION(const POOL_IMAGE_OPERATION & src_pool_image_operation)
{
	input_operations_required = 1;
	minimum_input_operations_required = 1;
	input_numeric_nodes_required = 0;

	NODE_SCALAR<char*> * node_pooling_kernel_width_identifier = new NODE_SCALAR<char*>("node_pooling_kernel_width");
	numeric_nodes_names_list.assignNodeValue(input_numeric_nodes_required, node_pooling_kernel_width_identifier);

	NODE_SCALAR<double> * local_node_A = new NODE_SCALAR<double>(2.0);
	local_numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_node_A);
	numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(input_numeric_nodes_required, true);

	NODE_SCALAR<double> * local_previous_node_A = new NODE_SCALAR<double>(2.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(input_numeric_nodes_required, local_previous_node_A);
	input_numeric_nodes_required++;


	NODE_SCALAR<char*> * node_pooling_kernel_height_identifier = new NODE_SCALAR<char*>("node_pooling_kernel_height");
	numeric_nodes_names_list.assignNodeValue(input_numeric_nodes_required, node_pooling_kernel_height_identifier);

	NODE_SCALAR<double> * local_node_B = new NODE_SCALAR<double>(2.0);
	local_numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_node_B);
	numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_numeric_nodes_list.getNodeValue(input_numeric_nodes_required));
	numeric_node_is_local_list.assignNodeValue(input_numeric_nodes_required, true);

	NODE_SCALAR<double> * local_previous_node_B = new NODE_SCALAR<double>(2.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(input_numeric_nodes_required, local_previous_node_B);
	input_numeric_nodes_required++;


	NODE_SCALAR<char*> * node_stride_x_identifier = new NODE_SCALAR<char*>("node_stride_x");
	numeric_nodes_names_list.assignNodeValue(input_numeric_nodes_required, node_stride_x_identifier);

	NODE_SCALAR<double> * local_node_C = new NODE_SCALAR<double>(1.0);
	local_numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_node_C);
	numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_numeric_nodes_list.getNodeValue(input_numeric_nodes_required));
	numeric_node_is_local_list.assignNodeValue(input_numeric_nodes_required, true);

	NODE_SCALAR<double> * local_previous_node_C = new NODE_SCALAR<double>(1.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(input_numeric_nodes_required, local_previous_node_C);
	input_numeric_nodes_required++;


	NODE_SCALAR<char*> * node_stride_y_identifier = new NODE_SCALAR<char*>("node_stride_y");
	numeric_nodes_names_list.assignNodeValue(input_numeric_nodes_required, node_stride_y_identifier);

	NODE_SCALAR<double> * local_node_D = new NODE_SCALAR<double>(1.0);
	local_numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_node_D);
	numeric_nodes_list.assignNodeValue(input_numeric_nodes_required, local_numeric_nodes_list.getNodeValue(input_numeric_nodes_required));
	numeric_node_is_local_list.assignNodeValue(input_numeric_nodes_required, true);

	NODE_SCALAR<double> * local_previous_node_D = new NODE_SCALAR<double>(1.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(input_numeric_nodes_required, local_previous_node_D);
	input_numeric_nodes_required++;


	copyFromImageOperation(src_pool_image_operation);
}



POOL_IMAGE_OPERATION POOL_IMAGE_OPERATION::operator=(const POOL_IMAGE_OPERATION & src_pool_image_operation)
{
	if (this != &src_pool_image_operation)
	{
		copyFromImageOperation(src_pool_image_operation);
	}

	return *this;
}

POOL_IMAGE_OPERATION::~POOL_IMAGE_OPERATION()
{
	// Nothing to deallocate
}


void POOL_IMAGE_OPERATION::performOperation()
{	
	const unsigned int pooling_kernel_width = (unsigned int)floor(numeric_nodes_list.getNodeValue(0)->getScalarValue());
	const unsigned int pooling_kernel_height = (unsigned int)floor(numeric_nodes_list.getNodeValue(1)->getScalarValue());
	const unsigned int stride_x = (unsigned int)floor(numeric_nodes_list.getNodeValue(2)->getScalarValue());
	const unsigned int stride_y = (unsigned int)floor(numeric_nodes_list.getNodeValue(3)->getScalarValue());

	const unsigned int integer_operations_in_x = (unsigned int)floor((double)(width_A - pooling_kernel_width) / (double)stride_x + 1);
	const unsigned int integer_operations_in_y = (unsigned int)floor((double)(height_A - pooling_kernel_height) / (double)stride_y + 1);

	const unsigned int operations_in_x = (unsigned int)ceil((double)(width_A - pooling_kernel_width) / (double)stride_x + 1);
	const unsigned int operations_in_y = (unsigned int)ceil((double)(height_A - pooling_kernel_height) / (double)stride_y + 1);

	const unsigned int offset_operation_x = operations_in_x - integer_operations_in_x;
	const unsigned int offset_operation_y = operations_in_y - integer_operations_in_y;
	
	const unsigned int computable_pooling_kernel_width = (width_A - stride_x * integer_operations_in_x);
	const unsigned int computable_pooling_kernel_height = (height_A - stride_y * integer_operations_in_y);

	if (dst_img)
	{
		if (dst_img->width != operations_in_x || dst_img->height != operations_in_y)
		{
			freeImageData(dst_img);
			dst_img = NULL;
		}
	}

	if (!dst_img)
	{
		dst_img = createVoidImage(operations_in_x, operations_in_y, IMG_DOUBLE);
	}
	
	// Perform the pooling only in the computable region:
	for (unsigned int y = 0; y < integer_operations_in_y; y++)
	{
		for (unsigned int x = 0; x < integer_operations_in_x; x++)
		{
			double d_intensity = std::numeric_limits<double>::lowest();

			for (unsigned int p_y = 0; p_y < pooling_kernel_height; p_y++)
			{
				for (unsigned int p_x = 0; p_x < pooling_kernel_width; p_x++)
				{
					const double a_intensity = *(src_img_A->image_data.double_image_data + y * stride_y * width_A + p_y * width_A + x * stride_x + p_x);
					if (d_intensity < a_intensity)
					{
						d_intensity = a_intensity;
					}
				}
			}
			
			*(dst_img->image_data.double_image_data + y * operations_in_x + x) = d_intensity;
		}
	}

	// Perform the pooling in the width reduced region at the right:
	if (offset_operation_x)
	{
		for (unsigned int y = 0; y < integer_operations_in_y; y++)
		{
			double d_intensity = std::numeric_limits<double>::lowest();

			for (unsigned int p_y = 0; p_y < pooling_kernel_height; p_y++)
			{
				for (unsigned int p_x = 0; p_x < computable_pooling_kernel_width; p_x++)
				{
					const double a_intensity = *(src_img_A->image_data.double_image_data + y * stride_y * width_A + p_y * width_A + integer_operations_in_x * stride_x + p_x);
					if (d_intensity < a_intensity)
					{
						d_intensity = a_intensity;
					}
				}
			}

			*(dst_img->image_data.double_image_data + y * operations_in_x + integer_operations_in_x) = d_intensity;
		}
	}

	// Perform the pooling in the height reduced region at the bottom:
	if (offset_operation_y)
	{
		double * a_img_data_ptr = src_img_A->image_data.double_image_data + integer_operations_in_y * stride_y * height_A;
		for (unsigned int x = 0; x < integer_operations_in_x; x++)
		{
			double d_intensity = std::numeric_limits<double>::lowest();

			for (unsigned int p_y = 0; p_y < computable_pooling_kernel_height; p_y++)
			{
				for (unsigned int p_x = 0; p_x < pooling_kernel_width; p_x++)
				{
					const double a_intensity = *(src_img_A->image_data.double_image_data + integer_operations_in_y * stride_y * width_A + p_y * width_A + x * stride_x + p_x);
					if (d_intensity < a_intensity)
					{
						d_intensity = a_intensity;
					}
				}
			}
			*(dst_img->image_data.double_image_data + integer_operations_in_y * operations_in_x + x) = d_intensity;
		}
	}

	// Perform the pooling in the height and width reduced region at the bottom right:
	if (offset_operation_x && offset_operation_y)
	{
		double * a_img_data_ptr = src_img_A->image_data.double_image_data + integer_operations_in_y * stride_y * height_A;
		double d_intensity = std::numeric_limits<double>::lowest();
		for (unsigned int p_y = 0; p_y < computable_pooling_kernel_height; p_y++)
		{
			for (unsigned int p_x = 0; p_x < computable_pooling_kernel_width; p_x++)
			{
				const double a_intensity = *(src_img_A->image_data.double_image_data + integer_operations_in_y * stride_y * width_A + p_y * width_A + integer_operations_in_x * stride_x + p_x);
				if (d_intensity < a_intensity)
				{
					d_intensity = a_intensity;
				}
			}
		}
		*(dst_img->image_data.double_image_data + integer_operations_in_y * operations_in_x + integer_operations_in_x) = d_intensity;
	}
}