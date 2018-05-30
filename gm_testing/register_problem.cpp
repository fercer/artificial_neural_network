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

	IMG_DATA * new_img = (IMG_DATA*)malloc(sizeof(IMG_DATA));
	new_img->image_data = (double*)malloc(width*height*sizeof(double));
	new_img->width = width;
	new_img->height = height;

	int max_intensity;
	fscanf(fp_img, "%i", &max_intensity);
	new_img->min_value = 0.0;
	new_img->max_value = 1.0;

	unsigned int pix_intensity;
	for (unsigned int pix_position = 0; pix_position < (width*height); pix_position++)
	{
		fscanf(fp_img, "%i", &pix_intensity);
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
	
	const double min_intensity = src_img->max_value;
	const double max_intensity = src_img->min_value;
	fprintf(fp_img, "255\n");

	unsigned char pix_intensity;
	for (unsigned int pix_position = 0; pix_position < (width*height); pix_position++)
	{
		fprintf(fp_img, "%i\n", (unsigned int)(255.0 * (*(src_img->image_data + pix_position) - min_intensity) / (max_intensity - min_intensity)));
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




IMG_DATA * rotateBicubic(IMG_DATA * src_img, const double theta)
{
	const unsigned int src_width = src_img->width;
	const unsigned int src_height = src_img->height;

	double c_theta, s_theta;
	double half_src_height, half_src_width;


	c_theta = cos(theta / 180.0 * MY_PI);
	s_theta = sin(theta / 180.0 * MY_PI);

	half_src_height = (double)(src_height - 1) / 2.0;
	half_src_width = (double)(src_width - 1) / 2.0;

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

	
	/* Rotate the ROI corners of the source image to figure out the size fo the destination image */
	const UL_x = c_theta * ((double)src_img->UL_x - half_src_width) - s_theta * ((double)src_img->UL_y - half_src_height) + half_src_width;
	const double UL_y = s_theta * ((double)src_img->UL_x - half_src_width) + c_theta * ((double)src_img->UL_y - half_src_height) + half_src_height;

	const double UR_x = c_theta * ((double)src_img->UR_x - half_src_width) - s_theta * ((double)src_img->UR_y - half_src_height) + half_src_width;
	const double UR_y = s_theta * ((double)src_img->UR_x - half_src_width) + c_theta * ((double)src_img->UR_y - half_src_height) + half_src_height;

	const double LR_x = c_theta * ((double)src_img->LR_x - half_src_width) - s_theta * ((double)src_img->LR_y - half_src_height) + half_src_width;
	const double LR_y = s_theta * ((double)src_img->LR_x - half_src_width) + c_theta * ((double)src_img->LR_y - half_src_height) + half_src_height;

	const double LL_x = c_theta * ((double)src_img->LR_x - half_src_width) - s_theta * ((double)src_img->LR_y - half_src_height) + half_src_width;
	const double LL_y = s_theta * ((double)src_img->LR_x - half_src_width) + c_theta * ((double)src_img->LR_y - half_src_height) + half_src_height;


	double interpolation_result;
	double max_value =  - 2.0 * src_img->max_value;
	double min_value = - 2.0 * src_img->max_value;
	for (int i = 0; i < src_height; i++)
	{
		for (int j = 0; j < src_width; j++)
		{
			const double src_x = c_theta * ((double)j - half_src_width) - s_theta * ((double)i - half_src_height) + half_src_width;
			const double src_y = s_theta * ((double)j - half_src_width) + c_theta * ((double)i - half_src_height) + half_src_height;

			if (src_x < 1 || src_x >= src_width || src_y < 1 || src_y >= src_height)
			{
				*(dst_img->image_data + src_width*i + j) = 0.0;
			}
			else
			{
				interpolation_result = bicubicInterpolation(src_temp, src_x, src_y);

				*(dst_img->image_data + src_width*i + j) = interpolation_result;
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
}


double computeLoss(double * src_img, double * trg_img)
{
	const unsigned int src_width = (unsigned int)*(src_img);
	const unsigned int src_height = (unsigned int)*(src_img + 1);

	double cost = 0.0;
	for (unsigned int pix_position = 0; pix_position < (src_width * src_height); pix_position++)
	{
		const double differences = *(trg_img + 4 + pix_position) - *(src_img + 4 + pix_position);
		cost += differences *  differences;
	}

	return cost / 2.0;
}



double computeLossPerPixel(const unsigned int pix_position, double * src_img, double * trg_img, const double theta, double * src_dx_img, double * src_dy_img, double * error_derivatives, double ** error_contributions)
{
	const unsigned int src_width = (unsigned int)*(src_img);
	const unsigned int src_height = (unsigned int)*(src_img + 1);

	const double ctheta = cos(theta);
	const double stheta = sin(theta);

	double cost = 0.0;
	double x, y;

	y = floor((double)pix_position / (double)src_width);
	x = (double)(pix_position % src_width);

	*(error_derivatives) = *(trg_img + 4 + pix_position) - *(src_img + 4 + pix_position);
	cost += *(error_derivatives) * *(error_derivatives);

	// Contribution to the error corresponding to the theta parameter:
	**(error_contributions) =
		-*(src_dx_img + 4 + pix_position) * (-x*stheta - y*ctheta)
		- *(src_dy_img + 4 + pix_position) * (x*ctheta - y*stheta);

	// Contribution to the error corresponding to the delta x parameter:
	**(error_contributions + 1) = -*(src_dx_img + 4 + pix_position);

	// Contribution to the error corresponding to the delta y parameter:
	**(error_contributions + 2) = -*(src_dy_img + 4 + pix_position);

	return cost / 2.0;
}



void computeDerivatives(double * src_img, double * dst_dx_img, double * dst_dy_img)
{
	const unsigned int src_width = (unsigned int)*(src_img);
	const unsigned int src_height = (unsigned int)*(src_img + 1);

	// Extend the boundaries of the source image:
	double * exp_img = (double*)malloc((src_width + 2) * (src_height + 2) * sizeof(double));

	*(exp_img) = 0.0;
	*(exp_img + src_width + 1) = 0.0;
	*(exp_img + (src_height + 1) * (src_width + 2)) = 0.0;
	*(exp_img + (src_height + 2) * (src_width + 2) - 1) = 0.0;
	memcpy(exp_img + 1, src_img + 4, src_width * sizeof(double));
	for (unsigned int y = 0; y < src_height; y++)
	{
		memcpy(exp_img + (y + 1)*(src_width + 2) + 1, src_img + 4 + y*src_width, src_width * sizeof(double));

		*(exp_img + (y + 1)*(src_width + 2)) = *(exp_img + (y + 1)*(src_width + 2) + 1);
		*(exp_img + (y + 1)*(src_width + 2) + src_width + 1) = *(exp_img + (y + 1)*(src_width + 2) + src_width);
	}
	memcpy(exp_img + (src_height + 1) * (src_width + 2) + 1, src_img + 4 + (src_height - 1) * src_width, src_width * sizeof(double));

	double max_dx_value = -2 * *(src_img + 3);
	double max_dy_value = -2 * *(src_img + 3);
	double min_dx_value = 2 * *(src_img + 3);
	double min_dy_value = 2 * *(src_img + 3);

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

			*(dst_dx_img + 4 + y * src_width + x) = dx_value;


			const double dy_value = (*(exp_img + (y + 2)*(src_width + 2) + x + 1) - *(exp_img + y*(src_width + 2) + x + 1)) / 2.0;
			if (max_dy_value < dy_value)
			{
				max_dy_value = dy_value;
			}
			if (min_dy_value > dy_value)
			{
				min_dy_value = dy_value;
			}

			*(dst_dy_img + 4 + y * src_width + x) = dy_value;
		}
	}

	*(dst_dx_img + 2) = min_dx_value;
	*(dst_dx_img + 3) = max_dx_value;
	*(dst_dy_img + 2) = min_dy_value;
	*(dst_dy_img + 3) = max_dy_value;

	free(exp_img);
}
