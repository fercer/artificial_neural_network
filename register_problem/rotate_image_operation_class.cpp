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
	const unsigned int src_width = src_img->width;
	const unsigned int src_height = src_img->height;
	double half_src_height, half_src_width;

	half_src_height = (double)(src_height - 1) / 2.0;
	half_src_width = (double)(src_width - 1) / 2.0;

	const double rotation_matrix_determinant = theta_11*theta_22 - theta_12*theta_21;
	const double inv_theta_11 = theta_22 / rotation_matrix_determinant;
	const double inv_theta_12 = -theta_21 / rotation_matrix_determinant;
	const double inv_theta_21 = -theta_12 / rotation_matrix_determinant;
	const double inv_theta_22 = theta_11 / rotation_matrix_determinant;

	/* Rotate the ROI corners of the source image to figure out the size fo the destination image */
	const double UL_x = half_src_width +
		inv_theta_11 * ((double)src_img->head_roi.UL_x - half_src_width) +
		inv_theta_21 * ((double)src_img->head_roi.UL_y - half_src_height);

	const double UL_y = half_src_height +
		inv_theta_12 * ((double)src_img->head_roi.UL_x - half_src_width) +
		inv_theta_22 * ((double)src_img->head_roi.UL_y - half_src_height);

	const double UR_x = half_src_width +
		inv_theta_11 * ((double)src_img->head_roi.UR_x - half_src_width) +
		inv_theta_21 * ((double)src_img->head_roi.UR_y - half_src_height);

	const double UR_y = half_src_height +
		inv_theta_12 * ((double)src_img->head_roi.UR_x - half_src_width) +
		inv_theta_22 * ((double)src_img->head_roi.UR_y - half_src_height);

	const double LR_x = half_src_width +
		inv_theta_11 * ((double)src_img->head_roi.LR_x - half_src_width) +
		inv_theta_21 * ((double)src_img->head_roi.LR_y - half_src_height);

	const double LR_y = half_src_height +
		inv_theta_12 * ((double)src_img->head_roi.LR_x - half_src_width) +
		inv_theta_22 * ((double)src_img->head_roi.LR_y - half_src_height);

	const double LL_x = half_src_width +
		inv_theta_11 * ((double)src_img->head_roi.LL_x - half_src_width) +
		inv_theta_21 * ((double)src_img->head_roi.LL_y - half_src_height);

	const double LL_y = half_src_height +
		inv_theta_12 * ((double)src_img->head_roi.LL_x - half_src_width) +
		inv_theta_22 * ((double)src_img->head_roi.LL_y - half_src_height);


	// Define bounding box size:
	const unsigned int max_width = (fabs(UL_x - LR_x) > fabs(UR_x - LL_x) ? fabs(UL_x - LR_x) : fabs(UR_x - LL_x)) + 1;
	double half_max_width = ((double)max_width - 1) / 2;
	const unsigned int max_height = (fabs(UL_y - LR_y) > fabs(UR_y - LL_y) ? fabs(UL_y - LR_y) : fabs(UR_y - LL_y)) + 1;
	double half_max_height = ((double)max_height - 1) / 2;

	double leftmost_corner = UL_x;
	if (leftmost_corner > UR_x)
	{
		leftmost_corner = UR_x;
	}

	if (leftmost_corner > LR_x)
	{
		leftmost_corner = LR_x;
	}

	if (leftmost_corner > LL_x)
	{
		leftmost_corner = LL_x;
	}

	double topmost_corner = UL_y;
	if (topmost_corner > UR_y)
	{
		topmost_corner = UR_y;
	}

	if (topmost_corner > LR_y)
	{
		topmost_corner = LR_y;
	}

	if (topmost_corner > LL_y)
	{
		topmost_corner = LL_y;
	}

	IMG_DATA * dst_img = createVoidImage(max_width, max_height);
	addImageROI(dst_img,
		RBT_ROTATED,
		(unsigned int)(UL_x - leftmost_corner),
		(unsigned int)(UL_y - topmost_corner),
		(unsigned int)(UR_x - leftmost_corner),
		(unsigned int)(UR_y - topmost_corner),
		(unsigned int)(LR_x - leftmost_corner),
		(unsigned int)(LR_y - topmost_corner),
		(unsigned int)(LL_x - leftmost_corner),
		(unsigned int)(LL_y - topmost_corner));

	/* Copy the source image to a temporal zero padded matrix. */
	IMG_DATA * src_temp = (IMG_DATA*)malloc(sizeof(IMG_DATA));
	src_temp->image_data = (double*)calloc((src_height + 3) * (src_width + 3), sizeof(double));
	src_temp->width = src_width + 3;
	src_temp->height = src_height + 3;
	src_temp->min_value = src_img->min_value;
	src_temp->max_value = src_img->max_value;

	for (unsigned int i = 0; i < src_height; i++)
	{
		memcpy(src_temp->image_data + (src_width + 3) * (i + 1),
			src_img->image_data + i*src_width,
			src_width * sizeof(double));
	}

	double interpolation_result;
	double max_value = -1e3;
	double min_value = 1e3;
	for (int i = 0; i < max_height; i++)
	{
		for (int j = 0; j < max_width; j++)
		{
			const double src_x = theta_11 * ((double)j - half_max_width) + theta_21 * ((double)i - half_max_height) + half_src_width;
			const double src_y = theta_12 * ((double)j - half_max_width) + theta_22 * ((double)i - half_max_height) + half_src_height;

			if (src_x < 1 || src_x >= src_width || src_y < 1 || src_y >= src_height)
			{
				*(dst_img->image_data + max_width*i + j) = 0.0;
			}
			else
			{
				interpolation_result = bicubicInterpolation(src_temp, src_x, src_y);

				*(dst_img->image_data + max_width*i + j) = interpolation_result;
				if (max_value < interpolation_result)
				{
					max_value = interpolation_result;
				}
				if (min_value > interpolation_result)
				{
					min_value = interpolation_result;
				}
			}
		}
	}

	dst_img->min_value = min_value;
	dst_img->max_value = max_value;

	free(src_temp->image_data);
	free(src_temp);

	return dst_img;
}