#include "register_problem.h"

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



IMG_DATA * presetProblem(IMG_DATA * src_img, IMG_DATA * trg_img)
{
	const unsigned int src_width = src_img->width;
	const unsigned int src_height = src_img->height;

	const unsigned int trg_width = trg_img->width;
	const unsigned int trg_height = trg_img->height;

	const unsigned int trg_width_2_left = (unsigned int)floor((double)trg_width / 2.0);
	const unsigned int trg_height_2_upper = (unsigned int)floor((double)trg_height / 2.0);

	const unsigned int src_width_2_left = (unsigned int)floor((double)src_width / 2.0);
	const unsigned int src_height_2_upper = (unsigned int)floor((double)src_height / 2.0);

	const unsigned int offset_left = trg_width_2_left - src_width_2_left;
	const unsigned int offset_upper = trg_height_2_upper - src_height_2_upper;

	IMG_DATA * dst_img = (IMG_DATA*)malloc(sizeof(IMG_DATA));
	dst_img->image_data = (double*)calloc(trg_width * trg_height, sizeof(double));
	dst_img->width = trg_width;
	dst_img->height = trg_height;
	dst_img->min_value = src_img->min_value;
	dst_img->max_value = src_img->max_value;

	dst_img->UL_x = 0;
	dst_img->UL_y = 0;
	dst_img->UR_x = trg_width - 1;
	dst_img->UR_y = 0;
	dst_img->LR_x = trg_width - 1;
	dst_img->LR_y = trg_height - 1;
	dst_img->LL_x = 0;
	dst_img->LL_y = trg_height - 1;

	for (unsigned int y = 0; y < src_height; y++)
	{
		memcpy(dst_img->image_data + (y + offset_upper)*trg_width + offset_left, src_img->image_data + y*src_width, src_width * sizeof(double));
	}

	return dst_img;
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




IMG_DATA * rotateBicubic(IMG_DATA * src_img, const double theta_11, const double theta_21, const double theta_12, const double theta_22)
{
	const unsigned int src_width = src_img->width;
	const unsigned int src_height = src_img->height;
	double half_src_height, half_src_width;

	half_src_height = (double)(src_height - 1) / 2.0;
	half_src_width = (double)(src_width - 1) / 2.0;
	
	/* Rotate the ROI corners of the source image to figure out the size fo the destination image */
	const double UL_x = theta_11 * ((double)src_img->UL_x - half_src_width) + theta_21 * ((double)src_img->UL_y - half_src_height) + half_src_width;
	const double UL_y = theta_12 * ((double)src_img->UL_x - half_src_width) + theta_22 * ((double)src_img->UL_y - half_src_height) + half_src_height;

	const double UR_x = theta_11 * ((double)src_img->UR_x - half_src_width) + theta_21 * ((double)src_img->UR_y - half_src_height) + half_src_width;
	const double UR_y = theta_12 * ((double)src_img->UR_x - half_src_width) + theta_22 * ((double)src_img->UR_y - half_src_height) + half_src_height;

	const double LR_x = theta_11 * ((double)src_img->LR_x - half_src_width) + theta_21 * ((double)src_img->LR_y - half_src_height) + half_src_width;
	const double LR_y = theta_12 * ((double)src_img->LR_x - half_src_width) + theta_22 * ((double)src_img->LR_y - half_src_height) + half_src_height;

	const double LL_x = theta_11 * ((double)src_img->LL_x - half_src_width) + theta_21 * ((double)src_img->LL_y - half_src_height) + half_src_width;
	const double LL_y = theta_12 * ((double)src_img->LL_x - half_src_width) + theta_22 * ((double)src_img->LL_y - half_src_height) + half_src_height;

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

	IMG_DATA * dst_img = (IMG_DATA*)malloc(sizeof(IMG_DATA));
	dst_img->width = max_width;
	dst_img->height = max_height;
	dst_img->UL_x = (unsigned int)(UL_x - leftmost_corner);
	dst_img->UL_y = (unsigned int)(UL_y - topmost_corner);
	dst_img->UR_x = (unsigned int)(UR_x - leftmost_corner);
	dst_img->UR_y = (unsigned int)(UR_y - topmost_corner);
	dst_img->LR_x = (unsigned int)(LR_x - leftmost_corner);
	dst_img->LR_y = (unsigned int)(LR_y - topmost_corner);
	dst_img->LL_x = (unsigned int)(LL_x - leftmost_corner);
	dst_img->LL_y = (unsigned int)(LL_y - topmost_corner);
	dst_img->image_data = (double*)calloc(max_height * max_width, sizeof(double));

	//half_src_height = (double)(max_height - 1) / 2.0;
	//half_src_width = (double)(max_width - 1) / 2.0;

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
	double max_value = -1000.0;
	double min_value = 1000.0;
	for (int i = 0; i < max_height; i++)
	{
		for (int j = 0; j < max_width; j++)
		{
			const double src_x = theta_11 * ((double)j - half_max_width) + theta_12 * ((double)i - half_max_height) + half_src_width;
			const double src_y = theta_21 * ((double)j - half_max_width) + theta_22 * ((double)i - half_max_height) + half_src_height;

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


double computeLoss(IMG_DATA * src_img, IMG_DATA * trg_img, const double delta_x, const double delta_y)
{
	// Compute the loss only on the intersection of the both images:
	const unsigned int src_width = src_img->width;
	const unsigned int src_height = src_img->height;
	const unsigned int trg_width = trg_img->width;
	const unsigned int trg_height = trg_img->height;

	const unsigned int src_half_width_left = (unsigned int)floor((double)src_width / 2.0);
	const unsigned int src_half_height_upper = (unsigned int)floor((double)src_height / 2.0);

	const unsigned int ULs_x = -src_half_width_left + (unsigned int)floor(delta_x);
	const unsigned int ULs_y = -src_half_height_upper + (unsigned int)floor(delta_y);
	const unsigned int LRs_x = ULs_x + src_width;
	const unsigned int LRs_y = ULs_y + src_height;

	const unsigned int ULt_x = -(unsigned int)floor((double)trg_width / 2.0);
	const unsigned int ULt_y = -(unsigned int)floor((double)trg_height / 2.0);
	const unsigned int LRt_x = ULt_x + trg_width;
	const unsigned int LRt_y = ULt_y + trg_height;

	unsigned int xs_ini, xs_end;
	unsigned int ys_ini, ys_end;
	unsigned int xt_ini, yt_ini;

	if (ULs_x > ULt_x)
	{
		xs_ini = 0;
		xt_ini = ULs_x - ULt_x;
	}
	else
	{
		xs_ini = ULt_x - ULs_x;
		xt_ini = 0;
	}

	if (ULs_y > ULt_y)
	{
		ys_ini = 0;
		yt_ini = ULs_y - ULt_y;
	}
	else
	{
		ys_ini = ULt_y - ULs_y;
		yt_ini = 0;
	}

	if (LRs_x < LRt_x)
	{
		xs_end = src_width - 1; // To make it inclusive
	}
	else
	{
		xs_end = LRt_x - ULs_x - 1;
	}

	if (LRs_y < LRt_y)
	{
		ys_end = src_height - 1; // To make it inclusive
	}
	else
	{
		ys_end = LRt_y - ULs_y - 1;
	}

	const unsigned int computable_width = xs_end - xs_ini;
	const unsigned int computable_height = ys_end - ys_ini;
	const double out_limit_penalty = src_width * src_height - computable_width * computable_height;

	double cost = 0.0;
	for (unsigned int y = 0; y <= computable_height; y++)
	{
		for (unsigned int x = 0; x <= computable_width; x++)
		{
			const double s_intensity = *(src_img->image_data + (y + ys_ini) * src_img->width + x + xs_ini);
			const double t_intensity = *(trg_img->image_data + (y + yt_ini) * trg_img->width + x + xt_ini);
			const double differences = t_intensity - s_intensity;
			cost += differences *  differences;
		}
	}

	return cost / 2.0;
}



double computeLossPerPixel(const unsigned int x, const unsigned int y, IMG_DATA * src_img, IMG_DATA * trg_img, const double delta_x, const double delta_y, IMG_DATA * src_dx_img, IMG_DATA * src_dy_img, double * error_derivatives, double ** error_contributions, const unsigned int xs_ini, const unsigned int ys_ini, const unsigned int xt_ini, const unsigned int yt_ini)
{	
	const double s_intensity = *(src_img->image_data + (y + ys_ini) * src_img->width + x + xs_ini);
	const double t_intensity = *(trg_img->image_data + (y + yt_ini) * trg_img->width + x + xt_ini);
	const double differences = t_intensity - s_intensity;
	const double cost = differences *  differences;

	return cost / 2.0;
	
	*(error_derivatives) = differences;

	// Contribution to the error corresponding to the theta parameters:
	**(error_contributions) = -*(src_dx_img->image_data + (y + ys_ini) * src_img->width + x + xs_ini) * x;
	**(error_contributions+1) = -*(src_dx_img->image_data + (y + ys_ini) * src_img->width + x + xs_ini) * y;
	**(error_contributions+2) = -*(src_dx_img->image_data + (y + ys_ini) * src_img->width + x + xs_ini);
	**(error_contributions+3) = -*(src_dy_img->image_data + (y + ys_ini) * src_img->width + x + xs_ini) * x;
	**(error_contributions+4) = -*(src_dy_img->image_data + (y + ys_ini) * src_img->width + x + xs_ini) * y;
	**(error_contributions+5) = -*(src_dy_img->image_data + (y + ys_ini) * src_img->width + x + xs_ini);

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

	new_img->UL_x = 0;
	new_img->UL_y = 0;
	new_img->UR_x = src_width - 1;
	new_img->UR_y = 0;
	new_img->LR_x = src_width - 1;
	new_img->LR_y = src_height - 1;
	new_img->LL_x = 0;
	new_img->LL_y = src_height - 1;

	new_img->image_data = (double*)calloc(src_height * src_width, sizeof(double));
	
	return new_img;
}