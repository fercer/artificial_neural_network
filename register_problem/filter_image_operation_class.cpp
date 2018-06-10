#include "filter_image_operation_class.h"

FILTER_IMAGE_OPERATION::FILTER_IMAGE_OPERATION()
{
	// Nothing to define
}



FILTER_IMAGE_OPERATION::FILTER_IMAGE_OPERATION(const FILTER_IMAGE_OPERATION & src_filter_image_operation)
{
	copyFromImageOperation(src_filter_image_operation);
}



FILTER_IMAGE_OPERATION FILTER_IMAGE_OPERATION::operator=(const FILTER_IMAGE_OPERATION & src_filter_image_operation)
{
	if (this != &src_filter_image_operation)
	{
		copyFromImageOperation(src_filter_image_operation);
	}

	return *this;
}



FILTER_IMAGE_OPERATION::~FILTER_IMAGE_OPERATION()
{
	// Nothing to deallocate
}



void FILTER_IMAGE_OPERATION::performOperation()
{
	double * communication_work_array;
	int information_integer;

	/* Offset for the zero padded image */
	const unsigned int offset_y = (unsigned int)floor((double)height_B / 2.0);
	const unsigned int offset_x = (unsigned int)floor((double)width_B / 2.0);

	const unsigned int nearest_2p_dim = (unsigned int)pow(2, ceil(log2(((height_A + offset_y) > (width_A + offset_x)) ? (height_A + offset_y) : (width_A + offset_x))));

	/* Communication variables for Fourier transform (The size of the communication array was taken from the example of the library usage) */
	communication_work_array = (double*)malloc((4 * nearest_2p_dim + 6 * nearest_2p_dim + 300) * sizeof(double));

	/* Zero pad the input images: */
	doublecomplex * fft_zp_img = (doublecomplex*)malloc((nearest_2p_dim / 2 + 1)*nearest_2p_dim * sizeof(doublecomplex));

	IMG_DATA * zp_temp = createVoidImage(nearest_2p_dim, nearest_2p_dim);

	for (unsigned int i = 0; i < height_A; i++)
	{
		memcpy(zp_temp->image_data + i * nearest_2p_dim,
			src_img_A->image_data + i * width_A, width_A * sizeof(double));
	}

	/* Initialize the function: */
	dzfft2d(0, nearest_2p_dim, nearest_2p_dim, zp_temp->image_data, fft_zp_img, communication_work_array, &information_integer);

	/* Perform the Fourier Transform: */
	dzfft2d(1, nearest_2p_dim, nearest_2p_dim, zp_temp->image_data, fft_zp_img, communication_work_array, &information_integer);

	/* Zero pad the kernel: */
	doublecomplex * fft_zp_kernel = (doublecomplex*)malloc((nearest_2p_dim / 2 + 1)*nearest_2p_dim * sizeof(doublecomplex));

	memset(zp_temp->image_data, 0, nearest_2p_dim * nearest_2p_dim * sizeof(double));

	for (unsigned int i = 0; i < height_B; i++)
	{
		memcpy(zp_temp->image_data + i * nearest_2p_dim,
			src_img_B->image_data + i * width_B, width_B * sizeof(double));
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
	
	if (!dst_img)
	{
		dst_img = createVoidImage(width_A, height_A);
	}


	if ((dst_img->width != width_A) || (dst_img->height != height_A))
	{
		if (dst_img->image_data)
		{
			free(dst_img->image_data);
		}
		dst_img->width = width_A;
		dst_img->height = height_A;
		dst_img->image_data = (double*)calloc(width_A * height_A, sizeof(double));
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


	const int half_width = floor((double)width_A / 2.0);
	const int half_height = floor((double)height_A / 2.0);

	dst_img->head_roi.ROI_type = RBT_AREA;
	dst_img->head_roi.UL_x = -half_width;
	dst_img->head_roi.UL_y = -half_height;
	dst_img->head_roi.UR_x = -half_width + width_A;
	dst_img->head_roi.UR_y = -half_height;
	dst_img->head_roi.LR_x = -half_width + width_A;
	dst_img->head_roi.LR_y = -half_height + height_A;
	dst_img->head_roi.LL_x = -half_width;
	dst_img->head_roi.LL_y = -half_height + height_A;

	for (unsigned int i = 0; i < height_A; i++)
	{
		memcpy(dst_img->image_data, zp_temp->image_data + (i + offset_y) * nearest_2p_dim + offset_x, width_A * sizeof(double));
	}

	freeImageData(zp_temp);
}

