#include "image_functions.h"

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

	const int UL_x = -floor((double)src_width / 2.0);
	const int UL_y = -floor((double)src_height / 2.0);

	new_img->width = src_width;
	new_img->height = src_height;

	new_img->max_value = 1.0;
	new_img->min_value = 0.0;

	new_img->head_roi.next_roi = NULL;
	new_img->tail_roi = &new_img->head_roi;
	
	new_img->head_roi.ROI_type = RBT_AREA;
	new_img->head_roi.UL_x = UL_x;
	new_img->head_roi.UL_y = UL_y;
	new_img->head_roi.UR_x = UL_x + src_width - 1;
	new_img->head_roi.UR_y = UL_y;
	new_img->head_roi.LR_x = UL_x + src_width - 1;
	new_img->head_roi.LR_y = UL_y + src_height - 1;
	new_img->head_roi.LL_x = UL_x;
	new_img->head_roi.LL_y = UL_y + src_height - 1;

	new_img->image_data = (double*)calloc(src_height * src_width, sizeof(double));
	
	return new_img;
}


void freeImageData(IMG_DATA * src_img_data_ptr)
{
	if (src_img_data_ptr->image_data)
	{
		free(src_img_data_ptr->image_data);
	}

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
	const int src_UL_x,
	const int src_UL_y,
	const int src_UR_x,
	const int src_UR_y,
	const int src_LR_x,
	const int src_LR_y,
	const int src_LL_x,
	const int src_LL_y)
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



IMG_DATA * createFromImageData(const IMG_DATA * src_img)
{
	const unsigned int width = src_img->width;
	const unsigned int height = src_img->height;

	IMG_DATA * dst_img = (IMG_DATA*)malloc(sizeof(IMG_DATA));

	dst_img->width = width;
	dst_img->height = height;
	dst_img->image_data = (double*)malloc(width * height * sizeof(double*));
	
	memcpy(src_img->image_data, dst_img->image_data, width * height * sizeof(double));
	
	memcpy(&dst_img->head_roi, &src_img->head_roi, sizeof(ROI_BBOX));
	dst_img->head_roi.next_roi = NULL;
	dst_img->tail_roi = &dst_img->head_roi;

	ROI_BBOX * next_roi_node = src_img->head_roi.next_roi;
	ROI_BBOX * current_roi_node;

	while (next_roi_node)
	{
		current_roi_node = next_roi_node;
		next_roi_node = current_roi_node->next_roi;

		addImageROI(dst_img, current_roi_node->ROI_type,
			current_roi_node->UL_x, current_roi_node->UL_y,
			current_roi_node->UR_x, current_roi_node->UR_y,
			current_roi_node->LR_x, current_roi_node->LR_y,
			current_roi_node->LL_x, current_roi_node->LL_y);
	}
	dst_img->max_value = src_img->max_value;
	dst_img->min_value = src_img->min_value;

	return dst_img;
}


void copyImageData(const IMG_DATA * src_img, IMG_DATA * dst_img)
{
	const unsigned int width = src_img->width;
	const unsigned int height = src_img->height;

	if (dst_img->image_data)
	{
		if ((dst_img->width != width) || (dst_img->height != height))
		{
			free(dst_img->image_data);
			dst_img->width = width;
			dst_img->height = height;
			dst_img->image_data = (double*)malloc(width * height * sizeof(double*));
		}

		memcpy(src_img->image_data, dst_img->image_data, width * height * sizeof(double));
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

		memcpy(&dst_img->head_roi, &src_img->head_roi, sizeof(ROI_BBOX));
		dst_img->head_roi.next_roi = NULL;
		dst_img->tail_roi = &dst_img->head_roi;

		next_roi_node = src_img->head_roi.next_roi;
		
		while (next_roi_node)
		{
			current_roi_node = next_roi_node;
			next_roi_node = current_roi_node->next_roi;

			addImageROI(dst_img, current_roi_node->ROI_type,
				current_roi_node->UL_x, current_roi_node->UL_y,
				current_roi_node->UR_x, current_roi_node->UR_y,
				current_roi_node->LR_x, current_roi_node->LR_y,
				current_roi_node->LL_x, current_roi_node->LL_y);
		}
	}

	dst_img->max_value = src_img->max_value;
	dst_img->min_value = src_img->min_value;
}
