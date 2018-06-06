#include "image_functions.h"

IMG_DATA * loadImagePGM(const char * filename)
{
	FILE * fp_img = fopen(filename, "r");
	char read_line[512];

	// Read magic number:
	fscanf(fp_img, "%s\n", read_line);

	// Read the commentary
	fgets(read_line, 512, fp_img);

	int width, height;
	fscanf(fp_img, "%i %i", &width, &height);

	
	int max_intensity;
	fscanf(fp_img, "%i", &max_intensity);
	
	IMG_DATA * new_img = createVoidImage(width, height);

	unsigned int pix_intensity;
	for (unsigned int pix_position = 0; pix_position < (width*height); pix_position++)
	{
		fscanf(fp_img, "%u", &pix_intensity);
		*(new_img->image_data + pix_position) = (double)(unsigned int)pix_intensity / (double)max_intensity;
	}

	fclose(fp_img);

	return new_img;
}



void saveImagePGM(const char * filename, IMG_DATA * src_img)
{
	const int width = src_img->width;
	const int height = src_img->height;

	FILE * fp_img = fopen(filename, "w");

	// Print magic number:
	fprintf(fp_img, "P2\n");

	// Print the commentary:
	fprintf(fp_img, "#Created by FerCer\n");

	// Print the width and height:
	fprintf(fp_img, "%i %i\n", width, height);
	
	const double min_intensity = src_img->min_value;
	const double max_intensity = src_img->max_value;
	fprintf(fp_img, "255\n");

	unsigned char pix_intensity;
	for (unsigned int pix_position = 0; pix_position < (width*height); pix_position++)
	{
		fprintf(fp_img, "%u\n", (unsigned int)(255.0 * (*(src_img->image_data + pix_position) - min_intensity) / (max_intensity - min_intensity)));
	}

	fclose(fp_img);
}



double bicubicInterpolation(IMG_DATA *src_img, const double x, const double y)
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




IMG_DATA * rotateBicubic(IMG_DATA * src_img, const double theta_11, const double theta_12, const double theta_21, const double theta_22)
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


double computeLoss(IMG_DATA * src_diff_img)
{
	ROI_BBOX * next_roi = src_diff_img->head_roi.next_roi;
	ROI_BBOX * current_roi;

	double cost = 0.0;

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
			for (unsigned int y = roi_y_ini; y < roi_y_end; y++)
			{
				for (unsigned int x = roi_x_ini; x < roi_x_end; x++)
				{
					const double d_intensity = *(src_diff_img->image_data + y * src_diff_img->width + x);
					cost += d_intensity*d_intensity;
				}
			}
			break;

		case RBT_AREA:
		case RBT_ROTATED:
		case RBT_TARGET:
		case RBT_SOURCE:
		case RBT_UNCOMPUTED:
			break;
		}
	}
	
	return cost / 2.0;
}



IMG_DATA * computeDerivativesX(IMG_DATA * src_img)
{
	const unsigned int src_width = src_img->width;
	const unsigned int src_height = src_img->height;

	// Extend the boundaries of the source image:
	double * exp_img = (double*)malloc((src_width + 2) * (src_height + 2) * sizeof(double));

	*(exp_img) = 0.0;
	*(exp_img + src_width + 1) = 0.0;
	*(exp_img + (src_height + 1) * (src_width + 2)) = 0.0;
	*(exp_img + (src_height + 2) * (src_width + 2) - 1) = 0.0;
	memcpy(exp_img + 1, src_img->image_data, src_width * sizeof(double));
	for (unsigned int y = 0; y < src_height; y++)
	{
		memcpy(exp_img + (y + 1)*(src_width + 2) + 1, src_img->image_data + y*src_width, src_width * sizeof(double));

		*(exp_img + (y + 1)*(src_width + 2)) = *(exp_img + (y + 1)*(src_width + 2) + 1);
		*(exp_img + (y + 1)*(src_width + 2) + src_width + 1) = *(exp_img + (y + 1)*(src_width + 2) + src_width);
	}
	memcpy(exp_img + (src_height + 1) * (src_width + 2) + 1, src_img->image_data + (src_height - 1) * src_width, src_width * sizeof(double));

	double max_dx_value = src_img->min_value;
	double max_dy_value = src_img->min_value;
	double min_dx_value = src_img->max_value;
	double min_dy_value = src_img->max_value;

	IMG_DATA * dst_dx_img = createVoidImage(src_width, src_height);

	// Perform the numeric derivatives:
	for (unsigned int y = 0; y < src_height; y++)
	{
		for (unsigned int x = 0; x < src_width; x++)
		{
			const double dx_value = (*(exp_img + (y + 1)*(src_width + 2) + x + 2) - *(exp_img + (y + 1)*(src_width + 2) + x)) / 2.0;
			if (max_dx_value < dx_value)
			{
				max_dx_value = dx_value;
			}
			if (min_dx_value > dx_value)
			{
				min_dx_value = dx_value;
			}

			*(dst_dx_img->image_data + y * src_width + x) = dx_value;
		}
	}

	dst_dx_img->min_value = min_dx_value;
	dst_dx_img->max_value = max_dx_value;

	free(exp_img);
	return dst_dx_img;
}



IMG_DATA * computeDerivativesY(IMG_DATA * src_img)
{
	const unsigned int src_width = src_img->width;
	const unsigned int src_height = src_img->height;

	// Extend the boundaries of the source image:
	double * exp_img = (double*)malloc((src_width + 2) * (src_height + 2) * sizeof(double));

	*(exp_img) = 0.0;
	*(exp_img + src_width + 1) = 0.0;
	*(exp_img + (src_height + 1) * (src_width + 2)) = 0.0;
	*(exp_img + (src_height + 2) * (src_width + 2) - 1) = 0.0;
	memcpy(exp_img + 1, src_img->image_data, src_width * sizeof(double));
	for (unsigned int y = 0; y < src_height; y++)
	{
		memcpy(exp_img + (y + 1)*(src_width + 2) + 1, src_img->image_data + y*src_width, src_width * sizeof(double));

		*(exp_img + (y + 1)*(src_width + 2)) = *(exp_img + (y + 1)*(src_width + 2) + 1);
		*(exp_img + (y + 1)*(src_width + 2) + src_width + 1) = *(exp_img + (y + 1)*(src_width + 2) + src_width);
	}
	memcpy(exp_img + (src_height + 1) * (src_width + 2) + 1, src_img->image_data + (src_height - 1) * src_width, src_width * sizeof(double));

	double max_dx_value = src_img->min_value;
	double max_dy_value = src_img->min_value;
	double min_dx_value = src_img->max_value;
	double min_dy_value = src_img->max_value;

	IMG_DATA * dst_dy_img = createVoidImage(src_width, src_height);

	// Perform the numeric derivatives:
	for (unsigned int y = 0; y < src_height; y++)
	{
		for (unsigned int x = 0; x < src_width; x++)
		{
			const double dy_value = (*(exp_img + (y + 2)*(src_width + 2) + x + 1) - *(exp_img + y*(src_width + 2) + x + 1)) / 2.0;
			if (max_dy_value < dy_value)
			{
				max_dy_value = dy_value;
			}
			if (min_dy_value > dy_value)
			{
				min_dy_value = dy_value;
			}

			*(dst_dy_img->image_data + y * src_width + x) = dy_value;
		}
	}

	dst_dy_img->min_value = min_dy_value;
	dst_dy_img->max_value = max_dy_value;

	free(exp_img);

	return dst_dy_img;
}



IMG_DATA * createVoidImage(const unsigned int src_width, const unsigned int src_height)
{
	IMG_DATA * new_img = (IMG_DATA*)malloc(sizeof(IMG_DATA));

	new_img->width = src_width;
	new_img->height = src_height;

	new_img->max_value = 1.0;
	new_img->min_value = 0.0;

	new_img->head_roi.next_roi = NULL;
	new_img->tail_roi = &new_img->head_roi;
	
	new_img->head_roi.ROI_type = RBT_AREA;
	new_img->head_roi.UL_x = 0;
	new_img->head_roi.UL_y = 0;
	new_img->head_roi.UR_x = src_width - 1;
	new_img->head_roi.UR_y = 0;
	new_img->head_roi.LR_x = src_width - 1;
	new_img->head_roi.LR_y = src_height - 1;
	new_img->head_roi.LL_x = 0;
	new_img->head_roi.LL_y = src_height - 1;

	new_img->image_data = (double*)calloc(src_height * src_width, sizeof(double));
	
	return new_img;
}



IMG_DATA * diffImage(IMG_DATA * src_img, IMG_DATA * trg_img, const double delta_x, const double delta_y)
{
	// Compute the differebce only on the intersection of the both images:
	const unsigned int src_width = src_img->width;
	const unsigned int src_height = src_img->height;
	const unsigned int trg_width = trg_img->width;
	const unsigned int trg_height = trg_img->height;

	const int src_half_width_left = (int)floor((double)src_width / 2.0);
	const int src_half_height_upper = (int)floor((double)src_height / 2.0);

	int ULs_x = -src_half_width_left - (int)floor(delta_x);
	int ULs_y = -src_half_height_upper - (int)floor(delta_y);
	const int LRs_x = ULs_x + src_width;
	const int LRs_y = ULs_y + src_height;

	int ULt_x = -(int)floor((double)trg_width / 2.0);
	int ULt_y = -(int)floor((double)trg_height / 2.0);
	const int LRt_x = ULt_x + trg_width;
	const int LRt_y = ULt_y + trg_height;

	// Compute intersection ROI positions:
	const int ULi_x = (ULs_x > ULt_x) ? ULs_x : ULt_x;
	const int ULi_y = (ULs_y > ULt_y) ? ULs_y : ULt_y;

	POSITION_NODE * x_posititons_root = newPositionLeaf(ULs_x);
	POSITION_NODE * y_posititons_root = newPositionLeaf(ULs_y);
	
	addPositionLeaf(x_posititons_root, ULs_x + src_width);
	addPositionLeaf(y_posititons_root, ULs_y + src_height);
	
	addPositionLeaf(x_posititons_root, ULt_x);
	addPositionLeaf(y_posititons_root, ULt_y);

	addPositionLeaf(x_posititons_root, ULt_x + trg_width);
	addPositionLeaf(y_posititons_root, ULt_y + trg_height);

	unsigned int x_positions_count = x_posititons_root->tree_depth;
	unsigned int y_positions_count = y_posititons_root->tree_depth;

	int * x_positions = dumpPositionsTree(x_posititons_root);
	int * y_positions = dumpPositionsTree(y_posititons_root);

	freePositionsTree(x_posititons_root);
	freePositionsTree(y_posititons_root);
	
	const unsigned int computable_width = *(x_positions + x_positions_count-1) - *(x_positions);
	const unsigned int computable_height = *(y_positions + y_positions_count-1) - *(y_positions);

	IMG_DATA * diff_img = createVoidImage(computable_width, computable_height);

	// Compute the ROIs:
	for (unsigned int x_positions_index = 0; x_positions_index < (x_positions_count - 1); x_positions_index++)
	{
		for (unsigned int y_positions_index = 0; y_positions_index < (y_positions_count - 1); y_positions_index++)
		{
			const int ULroi_x = *(x_positions + x_positions_index);
			const int ULroi_y = *(y_positions + y_positions_index);
			const int URroi_x = *(x_positions + x_positions_index + 1);
			const int URroi_y = *(y_positions + y_positions_index);
			const int LRroi_x = *(x_positions + x_positions_index + 1);
			const int LRroi_y = *(y_positions + y_positions_index + 1);
			const int LLroi_x = *(x_positions + x_positions_index);
			const int LLroi_y = *(y_positions + y_positions_index + 1);

			const int center_x = (int)floor((double)(ULroi_x + LRroi_x) / 2.0);
			const int center_y = (int)floor((double)(ULroi_y + LRroi_y) / 2.0);

			int current_roi_type_int = (int)RBT_UNCOMPUTED;

			// Verify if the current roi is inside the source image:
			if (ULs_x < center_x && ULs_y < center_y && center_x < LRs_x && center_y < LRs_y)
			{
				current_roi_type_int += (int)RBT_SOURCE;
			}

			// Verify if the current roi is inside the target image:
			if (ULt_x < center_x && ULt_y < center_y && center_x < LRt_x && center_y < LRt_y)
			{
				current_roi_type_int += (int)RBT_TARGET;
			}

			addImageROI(diff_img, 
				(ROI_BBOX_TYPE)current_roi_type_int, 
				ULroi_x - *(x_positions),
				ULroi_y - *(y_positions),
				URroi_x - *(x_positions),
				URroi_y - *(y_positions),
				LRroi_x - *(x_positions),
				LRroi_y - *(y_positions),
				LLroi_x - *(x_positions),
				LLroi_y - *(y_positions));
		}
	}

	ULs_x -= *(x_positions);
	ULt_x -= *(x_positions);

	ULs_y -= *(y_positions);
	ULt_y -= *(y_positions);


	addImageROI(diff_img,
		RBT_AREA,
		ULs_x,
		ULs_y,
		ULs_x + src_width,
		ULs_y,
		ULs_x + src_width,
		ULs_y + src_height,
		ULs_x,
		ULs_y + src_width);

	free(x_positions);
	free(y_positions);

	double max_value = -1e-3;
	double min_value = 1e-3;

	ROI_BBOX * next_roi = diff_img->head_roi.next_roi;
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
			for (unsigned int y = roi_y_ini; y < roi_y_end; y++)
			{
				for (unsigned int x = roi_x_ini; x < roi_x_end; x++)
				{
					const double t_intensity = *(trg_img->image_data + (y - ULt_y)* trg_width + x - ULt_x);
					const double s_intensity = *(src_img->image_data + (y - ULs_y)* src_width + x - ULs_x);
					const double d_intensity = t_intensity - s_intensity;

					*(diff_img->image_data + y * computable_width + x) = d_intensity;
					if (min_value > d_intensity)
					{
						min_value = d_intensity;
					}

					if (max_value < d_intensity)
					{
						max_value = d_intensity;
					}
				}
			}
			break;

		case RBT_TARGET:
			for (unsigned int y = roi_y_ini; y < roi_y_end; y++)
			{
				for (unsigned int x = roi_x_ini; x < roi_x_end; x++)
				{
					const double t_intensity = *(trg_img->image_data + (y - ULt_y)* trg_width + x - ULt_x);
					const double d_intensity = t_intensity;

					*(diff_img->image_data + y * computable_width + x) = d_intensity;
					if (min_value > d_intensity)
					{
						min_value = d_intensity;
					}

					if (max_value < d_intensity)
					{
						max_value = d_intensity;
					}
				}
			}
			break;

		case RBT_SOURCE:
			for (unsigned int y = roi_y_ini; y < roi_y_end; y++)
			{
				for (unsigned int x = roi_x_ini; x < roi_x_end; x++)
				{
					const double s_intensity = *(src_img->image_data + (y - ULs_y)* src_width + x - ULs_x);
					const double d_intensity = -s_intensity;

					*(diff_img->image_data + y * computable_width + x) = d_intensity;
					if (min_value > d_intensity)
					{
						min_value = d_intensity;
					}

					if (max_value < d_intensity)
					{
						max_value = d_intensity;
					}
				}
			}
			break;

		case RBT_AREA:
		case RBT_ROTATED:
		case RBT_UNCOMPUTED:
			break;
		}
	}
	
	diff_img->max_value = max_value;
	diff_img->min_value = min_value;

	return diff_img;
}


void freeImageData(IMG_DATA * src_img_data_ptr)
{
	free(src_img_data_ptr->image_data);

	ROI_BBOX * next_roi_node = src_img_data_ptr->head_roi.next_roi;
	ROI_BBOX * current_roi_node;

	while (next_roi_node)
	{
		current_roi_node = next_roi_node;
		next_roi_node = current_roi_node->next_roi;

		free(current_roi_node);
	}

	free(src_img_data_ptr);
}



void addImageROI(IMG_DATA * src_image_data_ptr,
	const ROI_BBOX_TYPE new_roi_type,
	const unsigned int src_UL_x,
	const unsigned int src_UL_y,
	const unsigned int src_UR_x,
	const unsigned int src_UR_y,
	const unsigned int src_LR_x,
	const unsigned int src_LR_y,
	const unsigned int src_LL_x,
	const unsigned int src_LL_y)
{
	src_image_data_ptr->tail_roi->next_roi = (ROI_BBOX*)malloc(sizeof(ROI_BBOX));
	src_image_data_ptr->tail_roi->next_roi->ROI_type = new_roi_type;

	src_image_data_ptr->tail_roi = src_image_data_ptr->tail_roi->next_roi;

	src_image_data_ptr->tail_roi->UL_x = src_UL_x;
	src_image_data_ptr->tail_roi->UL_y = src_UL_y;
	src_image_data_ptr->tail_roi->UR_x = src_UR_x;
	src_image_data_ptr->tail_roi->UR_y = src_UR_y;
	src_image_data_ptr->tail_roi->LR_x = src_LR_x;
	src_image_data_ptr->tail_roi->LR_y = src_LR_y;
	src_image_data_ptr->tail_roi->LL_x = src_LL_x;
	src_image_data_ptr->tail_roi->LL_y = src_LL_y;

	src_image_data_ptr->tail_roi->next_roi = NULL;
}



int addPositionLeaf(POSITION_NODE * src_current_leaf, const int src_new_position)
{
	int position_was_added = 0;
	if (src_new_position < src_current_leaf->position)
	{
		if (!src_current_leaf->left_leaf) // Left base case:
		{
			src_current_leaf->left_leaf = newPositionLeaf(src_new_position);
			// The position was succesfully assigned to a leaf:
			src_current_leaf->count_left_leaves = src_current_leaf->count_left_leaves + 1;
			src_current_leaf->tree_depth = src_current_leaf->tree_depth + 1;
			position_was_added = 1;
		}
		else // Recursion to the left nodes:
		{
			// Pass the position to the folliwing leaf to verify if the position is added:
			position_was_added = addPositionLeaf(src_current_leaf->left_leaf, src_new_position);
			src_current_leaf->count_left_leaves = src_current_leaf->count_left_leaves + position_was_added;
			src_current_leaf->tree_depth = src_current_leaf->tree_depth + position_was_added;
		}
	}
	else if(src_new_position > src_current_leaf->position)
	{
		if (!src_current_leaf->right_leaf) // Right base case:
		{
			src_current_leaf->right_leaf = newPositionLeaf(src_new_position);

			// The position was succesfully assigned to a leaf:
			src_current_leaf->tree_depth = src_current_leaf->tree_depth + 1;
			position_was_added = 1;
		}
		else // Recursion to the left nodes:
		{
			// Pass the position to the folliwing leaf to verify if the position is added:
			position_was_added = addPositionLeaf(src_current_leaf->right_leaf, src_new_position);
			src_current_leaf->tree_depth = src_current_leaf->tree_depth + position_was_added;
		}
	}

	// If the new position already exists, it is ommited:
	return position_was_added;
}



POSITION_NODE * newPositionLeaf(const unsigned int src_new_position)
{
	POSITION_NODE* new_leaf = (POSITION_NODE*)malloc(sizeof(POSITION_NODE));
	new_leaf->position = src_new_position;
	new_leaf->left_leaf = NULL;
	new_leaf->right_leaf = NULL;
	new_leaf->count_left_leaves = 0;
	new_leaf->tree_depth = 1;

	return new_leaf;
}



void freePositionsTree(POSITION_NODE * src_tree_root)
{
	if (src_tree_root->left_leaf)
	{
		freePositionsTree(src_tree_root->left_leaf);
	}

	if (src_tree_root->right_leaf)
	{
		freePositionsTree(src_tree_root->right_leaf);
	}

	// Base case:
	free(src_tree_root);
}



int * dumpPositionsTree(POSITION_NODE * tree_root)
{
	int * positions_array = (int*)malloc(tree_root->tree_depth * sizeof(int));
	dumpPositionLeaf(tree_root, 0, positions_array);
	return positions_array;
}



void dumpPositionLeaf(POSITION_NODE * src_current_leaf, const unsigned int src_count_positions_left, int * dst_positions_array)
{
	if (src_current_leaf->left_leaf)
	{
		dumpPositionLeaf(src_current_leaf->left_leaf, src_count_positions_left, dst_positions_array);
	}

	if (src_current_leaf->right_leaf)
	{
		dumpPositionLeaf(src_current_leaf->right_leaf, src_count_positions_left + src_current_leaf->count_left_leaves + 1, dst_positions_array);
	}

	// Base case:
	*(dst_positions_array + src_count_positions_left + src_current_leaf->count_left_leaves) = src_current_leaf->position;
}



IMG_DATA * filterImage(IMG_DATA * src_img, IMG_DATA * src_kernel)
{
	double * communication_work_array;
	int information_integer;

	/* Copy the iput data into a zero padded array of 2 powered dimension to a faster Fourier Transform */
	const unsigned int width = src_img->width;
	const unsigned int kernel_width = src_kernel->width;

	const unsigned int height = src_img->height;
	const unsigned int kernel_height = src_kernel->height;

	/* Offset for the zero padded image */
	const unsigned int offset_y = (unsigned int)floor((double)kernel_height / 2.0);
	const unsigned int offset_x = (unsigned int)floor((double)kernel_width / 2.0);
	
	const unsigned int nearest_2p_dim = (unsigned int)pow(2, ceil(log2(((height + offset_y) > (width + offset_x)) ? (height + offset_y) : (width + offset_x))));
	
	/* Communication variables for Fourier transform (The size of the communication array was taken from the example of the library usage) */
	communication_work_array = (double*)malloc((4 * nearest_2p_dim + 6 * nearest_2p_dim + 300) * sizeof(double));

	/* Zero pad the input images: */
	doublecomplex * fft_zp_img = (doublecomplex*)malloc((nearest_2p_dim / 2 + 1)*nearest_2p_dim * sizeof(doublecomplex));

	IMG_DATA * zp_temp = createVoidImage(nearest_2p_dim, nearest_2p_dim);
	
	for (unsigned int i = 0; i < height; i++)
	{
		memcpy(zp_temp->image_data + i * nearest_2p_dim,
			src_img->image_data + i * width, width * sizeof(double));
	}
	
	/* Initialize the function: */
	dzfft2d(0, nearest_2p_dim, nearest_2p_dim, zp_temp->image_data, fft_zp_img, communication_work_array, &information_integer);

	/* Perform the Fourier Transform: */
	dzfft2d(1, nearest_2p_dim, nearest_2p_dim, zp_temp->image_data, fft_zp_img, communication_work_array, &information_integer);

	/* Zero pad the kernel: */
	doublecomplex * fft_zp_kernel = (doublecomplex*)malloc((nearest_2p_dim / 2 + 1)*nearest_2p_dim * sizeof(doublecomplex));

	memset(zp_temp->image_data, 0, nearest_2p_dim * nearest_2p_dim * sizeof(double));

	for (unsigned int i = 0; i < kernel_height; i++)
	{
		memcpy(zp_temp->image_data + i * nearest_2p_dim,
			src_kernel->image_data + i * kernel_width, kernel_width * sizeof(double));
	}

	/* Initialize the function: */
	dzfft2d(0, nearest_2p_dim, nearest_2p_dim, zp_temp->image_data, fft_zp_kernel, communication_work_array, &information_integer);

	/* Perform the Fourier Transform: */
	dzfft2d(1, nearest_2p_dim, nearest_2p_dim, zp_temp->image_data, fft_zp_kernel, communication_work_array, &information_integer);
		
	doublecomplex * fft_resp_to_kernel = (doublecomplex*)malloc(nearest_2p_dim * (nearest_2p_dim / 2 + 1) * sizeof(doublecomplex));

	/* Convolve Kernel: */
	for (unsigned int i = 0; i < nearest_2p_dim*(nearest_2p_dim / 2 + 1); i++)
	{
		const double real_result =
			(fft_zp_kernel + i)->real * (fft_zp_img + i)->real -
			(fft_zp_kernel + i)->imag * (fft_zp_img + i)->imag;

		const double imag_result =
			(fft_zp_kernel + i)->real * (fft_zp_img + i)->imag +
			(fft_zp_kernel + i)->imag * (fft_zp_img + i)->real;

		(fft_resp_to_kernel + i)->real = real_result;
		(fft_resp_to_kernel + i)->imag = imag_result;
	}
	
	/* Initialize the function: */
	zdfft2d(0, nearest_2p_dim, nearest_2p_dim, fft_resp_to_kernel,
		zp_temp->image_data, communication_work_array, &information_integer);

	/* Perform the Fourier Transform: */
	zdfft2d(2, nearest_2p_dim, nearest_2p_dim, fft_resp_to_kernel, 
		zp_temp->image_data, communication_work_array, &information_integer);

	free(fft_zp_img);
	free(fft_zp_kernel);
	free(fft_resp_to_kernel);
	free(communication_work_array);
	
	IMG_DATA * filtering_res = createVoidImage(width, height);
	double * image_data_ptr = filtering_res->image_data;
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++, image_data_ptr++)
		{
			*image_data_ptr = *(zp_temp->image_data + (i + offset_y) * nearest_2p_dim + offset_x + j);
		}
	}

	freeImageData(zp_temp);

	filtering_res->max_value = 1.0;
	filtering_res->min_value = 1.0;

	return filtering_res;
}



double computeImageMax(IMG_DATA * src_img)
{
	double max_value = -1e13;

	for (unsigned int xy = 0; xy < src_img->height * src_img->width; xy++)
	{
		if (max_value < *(src_img->image_data + xy))
		{
			max_value = *(src_img->image_data + xy);
		}
	}

	src_img->max_value = max_value;
	return max_value;
}



double computeImageMin(IMG_DATA * src_img)
{
	double min_value = 1e13;

	for (unsigned int xy = 0; xy < src_img->height * src_img->width; xy++)
	{
		if (min_value > *(src_img->image_data + xy))
		{
			min_value = *(src_img->image_data + xy);
		}
	}

	src_img->min_value = min_value;
	return min_value;
}