#include "rotate_image_operation_class.h"

ROTATE_IMAGE_OPERATION::ROTATE_IMAGE_OPERATION()
{
	// Nothing to define
}



ROTATE_IMAGE_OPERATION::ROTATE_IMAGE_OPERATION(const ROTATE_IMAGE_OPERATION & src_rotate_image_operation)
{
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
	// Nothing to deallocate
}



void ROTATE_IMAGE_OPERATION::performOperation()
{
	double half_src_height, half_src_width;

	half_src_height = (double)(height_A - 1) / 2.0;
	half_src_width = (double)(width_A - 1) / 2.0;

	const double rotation_matrix_determinant = parameter_A*parameter_D - parameter_B*parameter_C;
	const double inv_parameter_A = parameter_D / rotation_matrix_determinant;
	const double inv_parameter_B = -parameter_C / rotation_matrix_determinant;
	const double inv_parameter_C = -parameter_B / rotation_matrix_determinant;
	const double inv_parameter_D = parameter_A / rotation_matrix_determinant;

	/* Rotate the ROI corners of the source image to figure out the size fo the destination image */
	const double UL_x = half_src_width +
		inv_parameter_A * ((double)src_img_A->head_roi.UL_x) +
		inv_parameter_C * ((double)src_img_A->head_roi.UL_y);

	const double UL_y = half_src_height +
		inv_parameter_B * ((double)src_img_A->head_roi.UL_x) +
		inv_parameter_D * ((double)src_img_A->head_roi.UL_y);

	const double UR_x = half_src_width +
		inv_parameter_A * ((double)src_img_A->head_roi.UR_x ) +
		inv_parameter_C * ((double)src_img_A->head_roi.UR_y );

	const double UR_y = half_src_height +
		inv_parameter_B * ((double)src_img_A->head_roi.UR_x ) +
		inv_parameter_D * ((double)src_img_A->head_roi.UR_y );

	const double LR_x = half_src_width +
		inv_parameter_A * ((double)src_img_A->head_roi.LR_x ) +
		inv_parameter_C * ((double)src_img_A->head_roi.LR_y );

	const double LR_y = half_src_height +
		inv_parameter_B * ((double)src_img_A->head_roi.LR_x ) +
		inv_parameter_D * ((double)src_img_A->head_roi.LR_y );

	const double LL_x = half_src_width +
		inv_parameter_A * ((double)src_img_A->head_roi.LL_x ) +
		inv_parameter_C * ((double)src_img_A->head_roi.LL_y );

	const double LL_y = half_src_height +
		inv_parameter_B * ((double)src_img_A->head_roi.LL_x ) +
		inv_parameter_D * ((double)src_img_A->head_roi.LL_y );


	// Define bounding box size:
	const unsigned int max_width = (fabs(UL_x - LR_x) > fabs(UR_x - LL_x) ? 
		fabs(UL_x - LR_x) : fabs(UR_x - LL_x)) + 1;

	const double half_max_width = ((double)max_width - 1) / 2;

	const unsigned int max_height = (fabs(UL_y - LR_y) > fabs(UR_y - LL_y) ? 
		fabs(UL_y - LR_y) : fabs(UR_y - LL_y)) + 1;

	const double half_max_height = ((double)max_height - 1) / 2;
	
	if (dst_img->image_data)
	{
		if ((dst_img->width != max_width) || (dst_img->height != max_height))
		{
			free(dst_img->image_data);

			dst_img->width = max_width;
			dst_img->height = max_height;
			dst_img->image_data = (double*)calloc(max_width * max_height, sizeof(double));
		}
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

		dst_img->head_roi.ROI_type = RBT_AREA;
		dst_img->head_roi.UL_x = -half_max_width;
		dst_img->head_roi.UL_y = -half_max_height;
		dst_img->head_roi.UR_x = -half_max_width + max_width;
		dst_img->head_roi.UR_y = -half_max_height;
		dst_img->head_roi.LR_x = -half_max_width + max_width;
		dst_img->head_roi.LR_x = -half_max_height + max_height;
		dst_img->head_roi.LL_x = -half_max_width;
		dst_img->head_roi.LL_y = -half_max_height + max_height;
	}

	addImageROI(dst_img, RBT_ROTATED,
		UL_x, UL_y,
		UR_x, UR_y,
		LR_y, LR_y,
		LL_y, LL_y);

	/* Copy the source image to a temporal zero padded matrix. */
	IMG_DATA * src_temp = (IMG_DATA*)malloc(sizeof(IMG_DATA));
	src_temp->image_data = (double*)calloc((height_A + 3) * (width_A + 3), sizeof(double));
	src_temp->width = width_A + 3;
	src_temp->height = height_A + 3;

	for (unsigned int i = 0; i < height_A; i++)
	{
		memcpy(src_temp->image_data + (width_A + 3) * (i + 1),
			src_img_A->image_data + i*width_A, width_A * sizeof(double));
	}

	double interpolation_result;
	double max_value = -1e3;
	double min_value = 1e3;
	for (int i = 0; i < max_height; i++)
	{
		for (int j = 0; j < max_width; j++)
		{
			const double src_x = parameter_A * ((double)j - half_max_width) + parameter_C * ((double)i - half_max_height) + half_src_width;
			const double src_y = parameter_B * ((double)j - half_max_width) + parameter_D * ((double)i - half_max_height) + half_src_height;

			if (src_x < 1 || src_x >= width_A || src_y < 1 || src_y >= height_A)
			{
				*(dst_img->image_data + max_width*i + j) = 0.0;
			}
			else
			{
				interpolation_result = bicubicInterpolation(src_temp, src_x, src_y);

				*(dst_img->image_data + max_width*i + j) = interpolation_result;
			}
		}
	}

	dst_img->min_value = 1.0;
	dst_img->max_value = 0.0;

	free(src_temp->image_data);
	free(src_temp);
}




double ROTATE_IMAGE_OPERATION::bicubicInterpolation(IMG_DATA *src_img, const double x, const double y)
{
	const unsigned int src_width = src_img->width;
	const unsigned int src_height = src_img->height;

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
		(*(src_img->image_data + (y_int - 1) * src_width + x_int - 1) * uy_1 +
			*(src_img->image_data + y_int * src_width + x_int - 1) * uy0 +
			*(src_img->image_data + (y_int + 1) * src_width + x_int - 1) * uy1 +
			*(src_img->image_data + (y_int + 2) * src_width + x_int - 1) * uy2) * ux_1 +
			(*(src_img->image_data + (y_int - 1) * src_width + x_int) * uy_1 +
				*(src_img->image_data + y_int * src_width + x_int) * uy0 +
				*(src_img->image_data + (y_int + 1) * src_width + x_int) * uy1 +
				*(src_img->image_data + (y_int + 2) * src_width + x_int) * uy2) * ux0 +
				(*(src_img->image_data + (y_int - 1) * src_width + x_int + 1) * uy_1 +
					*(src_img->image_data + y_int * src_width + x_int + 1) * uy0 +
					*(src_img->image_data + (y_int + 1) * src_width + x_int + 1) * uy1 +
					*(src_img->image_data + (y_int + 2) * src_width + x_int + 1) * uy2) * ux1 +
					(*(src_img->image_data + (y_int - 1) * src_width + x_int + 2) * uy_1 +
						*(src_img->image_data + y_int * src_width + x_int + 2) * uy0 +
						*(src_img->image_data + (y_int + 1) * src_width + x_int + 2) * uy1 +
						*(src_img->image_data + (y_int + 2) * src_width + x_int + 2) * uy2) * ux2;
}
