#include "image_functions.h"

void saveImagePGM(const char * filename, IMG_DATA * src_img)
{
	const int width = src_img->width;
	const int height = src_img->height;
	const unsigned int n_channels = src_img->n_channels;

	FILE * fp_img = fopen(filename, "w");

	// Print magic number:
	fprintf(fp_img, "P2\n");

	// Print the commentary:
	fprintf(fp_img, "#Created by FerCer\n");

	// Print the width and height:
	fprintf(fp_img, "%i %i\n", width, height);
	
	fprintf(fp_img, "255\n");
	
	switch (src_img->image_data_type)
	{
	case IMG_DOUBLE:
		for (unsigned int pix_position = 0; pix_position < (unsigned int)(width*height*n_channels); pix_position++)
		{
			fprintf(fp_img, "%u\n", (unsigned int)floor(255.0 * *(src_img->image_data.double_image_data + pix_position)));
		}
		break;

	case IMG_FLOAT:
		for (unsigned int pix_position = 0; pix_position < (unsigned int)(width*height*n_channels); pix_position++)
		{
			fprintf(fp_img, "%u\n", (unsigned int)floorf(255.0f * *(src_img->image_data.float_image_data + pix_position)));
		}
		break;

	case IMG_INT:
		for (unsigned int pix_position = 0; pix_position < (unsigned int)(width*height*n_channels); pix_position++)
		{
			fprintf(fp_img, "%u\n", (unsigned int)*(src_img->image_data.integer_image_data + pix_position));
		}
		break;

	case IMG_UINT:
		for (unsigned int pix_position = 0; pix_position < (unsigned int)(width*height*n_channels); pix_position++)
		{
			fprintf(fp_img, "%u\n", *(src_img->image_data.unsigned_integer_image_data + pix_position));
		}
		break;

	case IMG_CHAR:
		for (unsigned int pix_position = 0; pix_position < (unsigned int)(width*height*n_channels); pix_position++)
		{
			fprintf(fp_img, "%u\n", (unsigned int)*(src_img->image_data.character_image_data + pix_position));
		}
		break;

	case IMG_UCHAR:
		for (unsigned int pix_position = 0; pix_position < (unsigned int)(width*height*n_channels); pix_position++)
		{
			fprintf(fp_img, "%u\n", (unsigned int)*(src_img->image_data.unsigned_character_image_data + pix_position));
		}
		break;

	case IMG_UNSET:
		break;

	default:
		fprintf(stderr, "<<Error: The source image data type is unknown>>\n");
		break;
	}

	fclose(fp_img);
}



IMG_DATA * createVoidImage(const int src_width, const int src_height, const IMG_TYPE src_img_type, const unsigned int src_n_channels)
{
	IMG_DATA * new_img = (IMG_DATA*)malloc(sizeof(IMG_DATA));

	const int UL_x = -src_width / 2;
	const int UL_y = -src_height / 2;

	new_img->width = src_width;
	new_img->height = src_height;
	new_img->n_channels = src_n_channels;

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

	new_img->image_data_type = src_img_type;

	switch (src_img_type)
	{
	case IMG_DOUBLE:
		new_img->image_data.double_image_data = (double*)malloc(src_width * src_height * src_n_channels * sizeof(double*));
		break;

	case IMG_FLOAT:
		new_img->image_data.float_image_data = (float*)malloc(src_width * src_height * src_n_channels * sizeof(float*));
		break;

	case IMG_INT:
		new_img->image_data.integer_image_data = (int*)malloc(src_width * src_height * src_n_channels * sizeof(int*));
		break;

	case IMG_UINT:
		new_img->image_data.unsigned_integer_image_data = (unsigned int*)malloc(src_width * src_height * src_n_channels * sizeof(unsigned int*));
		break;

	case IMG_CHAR:
		new_img->image_data.character_image_data = (char*)malloc(src_width * src_height * src_n_channels * sizeof(char*));
		break;

	case IMG_UCHAR:
		new_img->image_data.unsigned_character_image_data = (unsigned char*)malloc(src_width * src_height * src_n_channels * sizeof(unsigned char*));
		break;

	case IMG_UNSET:
		break;

	default:
		fprintf(stderr, "<<Error: The image data type is unknown>>\n");
		free(new_img);
		return NULL;
		break;
	}

	return new_img;
}


void freeImageData(IMG_DATA * src_img_data_ptr)
{
	ROI_BBOX * next_roi_node = src_img_data_ptr->head_roi.next_roi;
	ROI_BBOX * current_roi_node;

	while (next_roi_node)
	{
		current_roi_node = next_roi_node;
		next_roi_node = current_roi_node->next_roi;

		free(current_roi_node);
	}

	switch (src_img_data_ptr->image_data_type)
	{
	case IMG_DOUBLE:
		free(src_img_data_ptr->image_data.double_image_data);
		break;

	case IMG_FLOAT:
		free(src_img_data_ptr->image_data.float_image_data);
		break;

	case IMG_INT:
		free(src_img_data_ptr->image_data.integer_image_data);
		break;

	case IMG_UINT:
		free(src_img_data_ptr->image_data.unsigned_integer_image_data);
		break;

	case IMG_CHAR:
		free(src_img_data_ptr->image_data.character_image_data);
		break;

	case IMG_UCHAR:
		free(src_img_data_ptr->image_data.unsigned_character_image_data);
		break;

	case IMG_UNSET:
		break;

	default:
		fprintf(stderr, "<<Error: The destination image data type is unknown>>\n");
		return;
		break;
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
	if (!src_img)
	{
		return NULL;
	}

	const unsigned int width = src_img->width;
	const unsigned int height = src_img->height;
	const unsigned int n_channels = src_img->n_channels;

	IMG_DATA * dst_img = (IMG_DATA*)malloc(sizeof(IMG_DATA));

	dst_img->width = width;
	dst_img->height = height;
	dst_img->n_channels = n_channels;
	dst_img->image_data_type = src_img->image_data_type;
	switch (src_img->image_data_type)
	{
	case IMG_DOUBLE:
		dst_img->image_data.double_image_data = (double*)malloc(width * height * n_channels * sizeof(double*));
		memcpy(dst_img->image_data.double_image_data, src_img->image_data.double_image_data, width * height * n_channels * sizeof(double));
		break;
	
	case IMG_FLOAT:
		dst_img->image_data.float_image_data = (float*)malloc(width * height * n_channels * sizeof(float*));
		memcpy(dst_img->image_data.float_image_data, src_img->image_data.float_image_data, width * height * n_channels * sizeof(float));
		break;
	
	case IMG_INT:
		dst_img->image_data.integer_image_data = (int*)malloc(width * height * n_channels * sizeof(int*));
		memcpy(dst_img->image_data.integer_image_data, src_img->image_data.integer_image_data, width * height * n_channels * sizeof(int));
		break;
	
	case IMG_UINT:
		dst_img->image_data.unsigned_integer_image_data = (unsigned int*)malloc(width * height * n_channels * sizeof(unsigned int*));
		memcpy(dst_img->image_data.unsigned_integer_image_data, src_img->image_data.unsigned_integer_image_data, width * height * n_channels * sizeof(unsigned int));
		break;
	
	case IMG_CHAR:
		dst_img->image_data.character_image_data = (char*)malloc(width * height * n_channels * sizeof(char*));
		memcpy(dst_img->image_data.character_image_data, src_img->image_data.character_image_data, width * height * n_channels * sizeof(char));
		break;
	
	case IMG_UCHAR:
		dst_img->image_data.unsigned_character_image_data = (unsigned char*)malloc(width * height * n_channels * sizeof(unsigned char*));
		memcpy(dst_img->image_data.unsigned_character_image_data, src_img->image_data.unsigned_character_image_data, width * height * n_channels * sizeof(unsigned char));
		break;
	
	default:
		fprintf(stderr, "<<Error: The image data type is unknown>>\n");
		free(dst_img);
		return NULL;
		break;
	}

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

	return dst_img;
}


void copyImageData(const IMG_DATA * src_img, IMG_DATA * dst_img)
{
	if (!src_img || (src_img->width == 0 && src_img->height == 0))
	{
		freeImageData(dst_img);
		return;
	}

	if (dst_img->image_data_type != src_img->image_data_type)
	{
		switch (dst_img->image_data_type)
		{
		case IMG_DOUBLE:
			free(dst_img->image_data.double_image_data);
			break;

		case IMG_FLOAT:
			free(dst_img->image_data.float_image_data);
			break;

		case IMG_INT:
			free(dst_img->image_data.integer_image_data);
			break;

		case IMG_UINT:
			free(dst_img->image_data.unsigned_integer_image_data);
			break;

		case IMG_CHAR:
			free(dst_img->image_data.character_image_data);
			break;

		case IMG_UCHAR:
			free(dst_img->image_data.unsigned_character_image_data);
			break;

		default:
			fprintf(stderr, "<<Error: The destination image data type is unknown>>\n");
			return;
			break;
		}

		dst_img->image_data_type = IMG_UNSET;
	}

	switch (src_img->image_data_type)
	{
	case IMG_DOUBLE:
		if (dst_img->image_data_type == IMG_UNSET || !dst_img->image_data.double_image_data)
		{
			dst_img->image_data.double_image_data = (double*)malloc(src_img->width * src_img->height * src_img->n_channels * sizeof(double*));
		}
		else if ((dst_img->width != src_img->width) || (dst_img->height != src_img->height) || (dst_img->n_channels != src_img->n_channels))
		{
			free(dst_img->image_data.double_image_data);
			dst_img->image_data.double_image_data = (double*)malloc(src_img->width * src_img->height * src_img->n_channels * sizeof(double*));
		}

		dst_img->width = src_img->width;
		dst_img->height = src_img->height;
		memcpy(dst_img->image_data.double_image_data, src_img->image_data.double_image_data, src_img->width * src_img->height * src_img->n_channels * sizeof(double));

		break;

	case IMG_FLOAT:
		if (dst_img->image_data_type == IMG_UNSET || !dst_img->image_data.float_image_data)
		{
			dst_img->image_data.float_image_data = (float*)malloc(src_img->width * src_img->height * src_img->n_channels * sizeof(float*));
		}
		else if ((dst_img->width != src_img->width) || (dst_img->height != src_img->height) || (dst_img->n_channels != src_img->n_channels))
		{
			free(dst_img->image_data.float_image_data);
			dst_img->image_data.float_image_data = (float*)malloc(src_img->width * src_img->height * src_img->n_channels * sizeof(float*));
		}

		dst_img->width = src_img->width;
		dst_img->height = src_img->height;
		memcpy(dst_img->image_data.float_image_data, src_img->image_data.float_image_data, src_img->width * src_img->height * src_img->n_channels * sizeof(float));

		break;

	case IMG_INT:
		if (dst_img->image_data_type == IMG_UNSET || !dst_img->image_data.integer_image_data)
		{
			dst_img->image_data.integer_image_data = (int*)malloc(src_img->width * src_img->height  * src_img->n_channels * sizeof(int*));
		}
		else if ((dst_img->width != src_img->width) || (dst_img->height != src_img->height) || (dst_img->n_channels != src_img->n_channels))
		{
			free(dst_img->image_data.integer_image_data);
			dst_img->image_data.integer_image_data = (int*)malloc(src_img->width * src_img->height  * src_img->n_channels * sizeof(int*));
		}

		dst_img->width = src_img->width;
		dst_img->height = src_img->height;
		memcpy(dst_img->image_data.integer_image_data, src_img->image_data.integer_image_data, src_img->width * src_img->height * src_img->n_channels * sizeof(int));

		break;

	case IMG_UINT:
		if (dst_img->image_data_type == IMG_UNSET || !dst_img->image_data.unsigned_integer_image_data)
		{
			dst_img->image_data.unsigned_integer_image_data = (unsigned int*)malloc(src_img->width * src_img->height  * src_img->n_channels * sizeof(unsigned int*));
		}
		else if ((dst_img->width != src_img->width) || (dst_img->height != src_img->height) || (dst_img->n_channels != src_img->n_channels))
		{
			free(dst_img->image_data.unsigned_integer_image_data);
			dst_img->image_data.unsigned_integer_image_data = (unsigned int*)malloc(src_img->width * src_img->height  * src_img->n_channels * sizeof(unsigned int*));
		}

		dst_img->width = src_img->width;
		dst_img->height = src_img->height;
		memcpy(dst_img->image_data.unsigned_integer_image_data, src_img->image_data.unsigned_integer_image_data, src_img->width * src_img->height  * src_img->n_channels * sizeof(unsigned int));

		break;

	case IMG_CHAR:
		if (dst_img->image_data_type == IMG_UNSET || !dst_img->image_data.character_image_data)
		{
			dst_img->image_data.character_image_data = (char*)malloc(src_img->width * src_img->height * src_img->n_channels * sizeof(char*));
		}
		else if ((dst_img->width != src_img->width) || (dst_img->height != src_img->height) || (dst_img->n_channels != src_img->n_channels))
		{
			free(dst_img->image_data.character_image_data);
			dst_img->image_data.character_image_data = (char*)malloc(src_img->width * src_img->height  * src_img->n_channels * sizeof(char*));
		}

		dst_img->width = src_img->width;
		dst_img->height = src_img->height;
		memcpy(dst_img->image_data.character_image_data, src_img->image_data.character_image_data, src_img->width * src_img->height  * src_img->n_channels * sizeof(char));

		break;

	case IMG_UCHAR:
		if (dst_img->image_data_type == IMG_UNSET || !dst_img->image_data.unsigned_character_image_data)
		{
			dst_img->image_data.unsigned_character_image_data = (unsigned char*)malloc(src_img->width * src_img->height  * src_img->n_channels * sizeof(unsigned char*));
		}
		else if ((dst_img->width != src_img->width) || (dst_img->height != src_img->height) || (dst_img->n_channels != src_img->n_channels))
		{
			free(dst_img->image_data.unsigned_character_image_data);
			dst_img->image_data.unsigned_character_image_data = (unsigned char*)malloc(src_img->width * src_img->height * src_img->n_channels * sizeof(unsigned char*));
		}

		dst_img->width = src_img->width;
		dst_img->height = src_img->height;
		memcpy(dst_img->image_data.unsigned_character_image_data, src_img->image_data.unsigned_character_image_data, src_img->width * src_img->height * src_img->n_channels * sizeof(unsigned char));

		break;

	default:
		fprintf(stderr, "<<Error: The source image data type is unknown>>\n");
		return;
		break;
	}

	dst_img->n_channels = src_img->n_channels;


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
	}

	dst_img->head_roi.UL_x = src_img->head_roi.UL_x;
	dst_img->head_roi.UL_y = src_img->head_roi.UL_y;
	dst_img->head_roi.UR_x = src_img->head_roi.UR_x;
	dst_img->head_roi.UR_y = src_img->head_roi.UR_y;
	dst_img->head_roi.LR_x = src_img->head_roi.LR_x;
	dst_img->head_roi.LR_y = src_img->head_roi.LR_y;
	dst_img->head_roi.LL_x = src_img->head_roi.LL_x;
	dst_img->head_roi.LL_y = src_img->head_roi.LL_y;
	
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
}
