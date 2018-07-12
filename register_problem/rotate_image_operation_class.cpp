#include "rotate_image_operation_class.h"

ROTATE_IMAGE_OPERATION::ROTATE_IMAGE_OPERATION()
{
	input_operations_required = 1;
	minimum_input_operations_required = 1;

	input_numeric_nodes_required = 4;
	NODE_SCALAR<char*> * node_11_identifier = new NODE_SCALAR<char*>("node_11");
	numeric_nodes_names_list.assignNodeValue(0, node_11_identifier);

	NODE_SCALAR<char*> * node_12_identifier = new NODE_SCALAR<char*>("node_12");
	numeric_nodes_names_list.assignNodeValue(1, node_12_identifier);

	NODE_SCALAR<char*> * node_21_identifier = new NODE_SCALAR<char*>("node_21");
	numeric_nodes_names_list.assignNodeValue(2, node_21_identifier);

	NODE_SCALAR<char*> * node_22_identifier = new NODE_SCALAR<char*>("node_22");
	numeric_nodes_names_list.assignNodeValue(3, node_22_identifier);

	NODE_SCALAR<double> * local_node_A = new NODE_SCALAR<double>(1.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<double> * local_node_B = new NODE_SCALAR<double>(0.0);
	local_numeric_nodes_list.assignNodeValue(1, local_node_B);
	numeric_nodes_list.assignNodeValue(1, local_numeric_nodes_list.getNodeValue(1));
	numeric_node_is_local_list.assignNodeValue(1, true);

	NODE_SCALAR<double> * local_node_C = new NODE_SCALAR<double>(0.0);
	local_numeric_nodes_list.assignNodeValue(2, local_node_C);
	numeric_nodes_list.assignNodeValue(2, local_numeric_nodes_list.getNodeValue(2));
	numeric_node_is_local_list.assignNodeValue(2, true);

	NODE_SCALAR<double> * local_node_D = new NODE_SCALAR<double>(1.0);
	local_numeric_nodes_list.assignNodeValue(3, local_node_D);
	numeric_nodes_list.assignNodeValue(3, local_numeric_nodes_list.getNodeValue(3));
	numeric_node_is_local_list.assignNodeValue(3, true);
	
	NODE_SCALAR<double> * local_previous_node_A = new NODE_SCALAR<double>(1.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(0, local_previous_node_A);

	NODE_SCALAR<double> * local_previous_node_B = new NODE_SCALAR<double>(0.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(1, local_previous_node_B);
	
	NODE_SCALAR<double> * local_previous_node_C = new NODE_SCALAR<double>(0.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(2, local_previous_node_C);

	NODE_SCALAR<double> * local_previous_node_D = new NODE_SCALAR<double>(1.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(3, local_previous_node_D);
}



ROTATE_IMAGE_OPERATION::ROTATE_IMAGE_OPERATION(const ROTATE_IMAGE_OPERATION & src_rotate_image_operation)
{
	input_operations_required = 1;
	minimum_input_operations_required = 1;

	input_numeric_nodes_required = 4;
	NODE_SCALAR<char*> * node_11_identifier = new NODE_SCALAR<char*>("node_11");
	numeric_nodes_names_list.assignNodeValue(0, node_11_identifier);

	NODE_SCALAR<char*> * node_12_identifier = new NODE_SCALAR<char*>("node_12");
	numeric_nodes_names_list.assignNodeValue(1, node_12_identifier);

	NODE_SCALAR<char*> * node_21_identifier = new NODE_SCALAR<char*>("node_21");
	numeric_nodes_names_list.assignNodeValue(2, node_21_identifier);

	NODE_SCALAR<char*> * node_22_identifier = new NODE_SCALAR<char*>("node_22");
	numeric_nodes_names_list.assignNodeValue(3, node_22_identifier);

	NODE_SCALAR<double> * local_node_A = new NODE_SCALAR<double>(1.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<double> * local_node_B = new NODE_SCALAR<double>(0.0);
	local_numeric_nodes_list.assignNodeValue(1, local_node_B);
	numeric_nodes_list.assignNodeValue(1, local_numeric_nodes_list.getNodeValue(1));
	numeric_node_is_local_list.assignNodeValue(1, true);

	NODE_SCALAR<double> * local_node_C = new NODE_SCALAR<double>(0.0);
	local_numeric_nodes_list.assignNodeValue(2, local_node_C);
	numeric_nodes_list.assignNodeValue(2, local_numeric_nodes_list.getNodeValue(2));
	numeric_node_is_local_list.assignNodeValue(2, true);

	NODE_SCALAR<double> * local_node_D = new NODE_SCALAR<double>(1.0);
	local_numeric_nodes_list.assignNodeValue(3, local_node_D);
	numeric_nodes_list.assignNodeValue(3, local_numeric_nodes_list.getNodeValue(3));
	numeric_node_is_local_list.assignNodeValue(3, true);

	NODE_SCALAR<double> * local_previous_node_A = new NODE_SCALAR<double>(1.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(0, local_previous_node_A);

	NODE_SCALAR<double> * local_previous_node_B = new NODE_SCALAR<double>(0.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(1, local_previous_node_B);

	NODE_SCALAR<double> * local_previous_node_C = new NODE_SCALAR<double>(0.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(2, local_previous_node_C);

	NODE_SCALAR<double> * local_previous_node_D = new NODE_SCALAR<double>(1.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(3, local_previous_node_D);

	copyFromImageOperation(src_rotate_image_operation);
}



ROTATE_IMAGE_OPERATION ROTATE_IMAGE_OPERATION::operator=(const ROTATE_IMAGE_OPERATION & src_rotate_image_operation)
{
	if (this != &src_rotate_image_operation)
	{
		copyFromImageOperation(src_rotate_image_operation);
	}

	return *this;
}



ROTATE_IMAGE_OPERATION::~ROTATE_IMAGE_OPERATION()
{

}



void ROTATE_IMAGE_OPERATION::performOperation()
{
	const double half_src_width = width_A / 2;
	const double half_src_height = height_A / 2;

	const double rotation_matrix_determinant =
		numeric_nodes_list.getNodeValue(0)->getScalarValue() * numeric_nodes_list.getNodeValue(3)->getScalarValue() -
		numeric_nodes_list.getNodeValue(1)->getScalarValue() * numeric_nodes_list.getNodeValue(2)->getScalarValue();

	const double inv_parameter_A = numeric_nodes_list.getNodeValue(3)->getScalarValue() / rotation_matrix_determinant;
	const double inv_parameter_B = -numeric_nodes_list.getNodeValue(1)->getScalarValue() / rotation_matrix_determinant;
	const double inv_parameter_C = -numeric_nodes_list.getNodeValue(2)->getScalarValue() / rotation_matrix_determinant;
	const double inv_parameter_D = numeric_nodes_list.getNodeValue(0)->getScalarValue() / rotation_matrix_determinant;
	
	/* Rotate the ROI corners of the source image to figure out the size fo the destination image */
	const double UL_x = 
		inv_parameter_A * (-half_src_width) +
		inv_parameter_C * (-half_src_height);

	const double UL_y =
		inv_parameter_B * (-half_src_width) +
		inv_parameter_D * (-half_src_height);

	const double UR_x =
		inv_parameter_A * (width_A - half_src_width) +
		inv_parameter_C * (-half_src_height);

	const double UR_y =
		inv_parameter_B * (width_A - half_src_width) +
		inv_parameter_D * (-half_src_height);

	const double LR_x =
		inv_parameter_A * (width_A - half_src_width) +
		inv_parameter_C * (height_A - half_src_height);

	const double LR_y =
		inv_parameter_B * (width_A - half_src_width) +
		inv_parameter_D * (height_A - half_src_height);

	const double LL_x =
		inv_parameter_A * (-half_src_width) +
		inv_parameter_C * (height_A - half_src_height);

	const double LL_y =
		inv_parameter_B * (-half_src_width) +
		inv_parameter_D * (height_A - half_src_height);

	POSITION_NODE * x_posititons_root = newPositionLeaf((int)UL_x);
	POSITION_NODE * y_posititons_root = newPositionLeaf((int)UL_y);
	addPositionLeaf(x_posititons_root, (int)UR_x);
	addPositionLeaf(y_posititons_root, (int)UR_y);
	addPositionLeaf(x_posititons_root, (int)LR_x);
	addPositionLeaf(y_posititons_root, (int)LR_y);
	addPositionLeaf(x_posititons_root, (int)LL_x);
	addPositionLeaf(y_posititons_root, (int)LL_y);

	const unsigned int x_positions_count = x_posititons_root->tree_depth;
	const unsigned int y_positions_count = y_posititons_root->tree_depth;
	int * x_positions = dumpPositionsTree(x_posititons_root);
	int * y_positions = dumpPositionsTree(y_posititons_root);

	freePositionsTree(x_posititons_root);
	freePositionsTree(y_posititons_root);

	// Make the ROI area grater for the resukting rotated image:
	*(x_positions) = *(x_positions);// -1;
	*(y_positions) = *(y_positions);// -1;
	*(x_positions + x_positions_count - 1) = *(x_positions + x_positions_count - 1);// +1;
	*(y_positions + y_positions_count - 1) = *(y_positions + y_positions_count - 1);// +1;

	// Define bounding box size:
	const unsigned int max_width = (unsigned int)(*(x_positions + x_positions_count - 1) - *(x_positions)) + 1;
	const unsigned int max_height = (unsigned int)(*(y_positions + y_positions_count - 1) - *(y_positions)) + 1;

	const unsigned int half_max_width = max_width / 2;
	const unsigned int half_max_height = max_height / 2;

	if (!dst_img)
	{
		dst_img = createVoidImage(max_width, max_height);
	}
	
	if ((dst_img->width != max_width) || (dst_img->height != max_height))
	{
		if (dst_img->image_data_type == IMG_UNSET)
		{
			dst_img->image_data_type = IMG_DOUBLE;
		}
		else if (dst_img->image_data.double_image_data)
		{
			free(dst_img->image_data.double_image_data);
		}
		dst_img->width = max_width;
		dst_img->height = max_height;
		dst_img->image_data.double_image_data = (double*)calloc(max_width * max_height, sizeof(double));
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

	const int center_src_x = (src_img_A->head_roi.UL_x + src_img_A->head_roi.LR_x) / 2;
	const int center_src_y = (src_img_A->head_roi.UL_y + src_img_A->head_roi.LR_y) / 2;

	dst_img->head_roi.ROI_type = RBT_AREA;
	dst_img->head_roi.UL_x = *(x_positions) + center_src_x;
	dst_img->head_roi.UL_y = *(y_positions) + center_src_y;
	dst_img->head_roi.UR_x = *(x_positions + x_positions_count - 1) + center_src_x;
	dst_img->head_roi.UR_y = *(y_positions) + center_src_y;
	dst_img->head_roi.LR_x = *(x_positions + x_positions_count - 1) + center_src_x;
	dst_img->head_roi.LR_y = *(y_positions + y_positions_count - 1) + center_src_y;
	dst_img->head_roi.LL_x = *(x_positions) + center_src_x;
	dst_img->head_roi.LL_y = *(y_positions + y_positions_count - 1) + center_src_y;

	free(x_positions);
	free(y_positions);
	
	/* Copy the source image to a temporal zero padded matrix. */
	double * src_temp_image_data = (double*)calloc((height_A + 3) * (width_A + 3), sizeof(double));

	for (unsigned int i = 0; i < height_A; i++)
	{
		memcpy(src_temp_image_data + (width_A + 3) * (i + 1) + 1,
			src_img_A->image_data.double_image_data + i*width_A, width_A * sizeof(double));
	}
	FILE * fp_points = fopen("points.dat", "w");
	double interpolation_result;
	for (int i = 0; i < (int)max_height; i++)
	{
		for (int j = 0; j < (int)max_width; j++)
		{
			const double src_x = half_src_width +
				numeric_nodes_list.getNodeValue(0)->getScalarValue() * ((double)j - half_max_width) + numeric_nodes_list.getNodeValue(2)->getScalarValue() * ((double)i - half_max_height);
			const double src_y = half_src_height +
				numeric_nodes_list.getNodeValue(1)->getScalarValue() * ((double)j - half_max_width) + numeric_nodes_list.getNodeValue(3)->getScalarValue() * ((double)i - half_max_height);

			if ((src_x < 1) ||
				(src_x > width_A) ||
				(src_y < 1) ||
				(src_y > height_A))
			{
				*(dst_img->image_data.double_image_data + max_width*i + j) = 0.0;
			}
			else
			{
				interpolation_result = bicubicInterpolation(src_temp_image_data, src_x, src_y);
				fprintf(fp_points, "%f %f\n", src_x, src_y);
				*(dst_img->image_data.double_image_data + max_width*i + j) = interpolation_result;
			}
		}
	}

	fclose(fp_points);
	free(src_temp_image_data);
}




double ROTATE_IMAGE_OPERATION::bicubicInterpolation(double * src_img, const double x, const double y)
{
	const int x_int = (int)floor(x);
	const int y_int = (int)floor(y);

	const double s0_x = x - (double)x_int;
	const double s_1_x = 1.0 + s0_x;
	const double s1_x = 1.0 - s0_x;
	const double s2_x = 2.0 - s0_x;

	const double s0_y = y - (double)y_int;
	const double s_1_y = 1.0 + s0_y;
	const double s1_y = 1.0 - s0_y;
	const double s2_y = 2.0 - s0_y;

	/* Compute coefficients for the x-axis interpolation */
	const double ux_1 = -0.5 * s_1_x*s_1_x*s_1_x + 2.5 * s_1_x * s_1_x - 4.0 * s_1_x + 2.0;
	const double ux0 = 1.5 * s0_x*s0_x*s0_x - 2.5 * s0_x*s0_x + 1.0;
	const double ux1 = 1.5 * s1_x*s1_x*s1_x - 2.5 * s1_x*s1_x + 1.0;
	const double ux2 = -0.5 * s2_x*s2_x*s2_x + 2.5 * s2_x * s2_x - 4.0 * s2_x + 2.0;

	/* Compute coefficients for the y-axis interpolation */
	const double uy_1 = -0.5 * s_1_y*s_1_y*s_1_y + 2.5 * s_1_y * s_1_y - 4.0 * s_1_y + 2.0;
	const double uy0 = 1.5 * s0_y*s0_y*s0_y - 2.5 * s0_y*s0_y + 1.0;
	const double uy1 = 1.5 * s1_y*s1_y*s1_y - 2.5 * s1_y*s1_y + 1.0;
	const double uy2 = -0.5 * s2_y*s2_y*s2_y + 2.5 * s2_y * s2_y - 4.0 * s2_y + 2.0;

	return
		(*(src_img + (y_int - 1) * (width_A + 3) + x_int - 1) * uy_1 +
			*(src_img + y_int * (width_A + 3) + x_int - 1) * uy0 +
			*(src_img + (y_int + 1) * (width_A + 3) + x_int - 1) * uy1 +
			*(src_img + (y_int + 2) * (width_A + 3) + x_int - 1) * uy2) * ux_1 +
			(*(src_img + (y_int - 1) * (width_A + 3) + x_int) * uy_1 +
				*(src_img + y_int * (width_A + 3) + x_int) * uy0 +
				*(src_img + (y_int + 1) * (width_A + 3) + x_int) * uy1 +
				*(src_img + (y_int + 2) * (width_A + 3) + x_int) * uy2) * ux0 +
				(*(src_img + (y_int - 1) * (width_A + 3) + x_int + 1) * uy_1 +
					*(src_img + y_int * (width_A + 3) + x_int + 1) * uy0 +
					*(src_img + (y_int + 1) * (width_A + 3) + x_int + 1) * uy1 +
					*(src_img + (y_int + 2) * (width_A + 3) + x_int + 1) * uy2) * ux1 +
					(*(src_img + (y_int - 1) * (width_A + 3) + x_int + 2) * uy_1 +
						*(src_img + y_int * (width_A + 3) + x_int + 2) * uy0 +
						*(src_img + (y_int + 1) * (width_A + 3) + x_int + 2) * uy1 +
						*(src_img + (y_int + 2) * (width_A + 3) + x_int + 2) * uy2) * ux2;
}
