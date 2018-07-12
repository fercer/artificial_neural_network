#ifndef IMAGE_FUNCTIONS_INCLUDED
#define IMAGE_FUNCTIONS_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <acml.h>

typedef enum ROI_BBOX_TYPE {
	RBT_UNCOMPUTED = 0,
	RBT_AREA = 1,
	RBT_SOURCE = 2,
	RBT_TARGET = 4,
	RBT_INTERSECTION = 6
} ROI_BBOX_TYPE;

typedef struct ROI_BBOX /* [R]egion [O]f [I]nterest [B]ounding [BOX]*/
{
	ROI_BBOX_TYPE ROI_type;
	int UL_x, UL_y;
	int UR_x, UR_y;
	int LR_x, LR_y;
	int LL_x, LL_y;
	ROI_BBOX * next_roi;
} ROI_BBOX;


typedef enum IMG_TYPE
{
	IMG_DOUBLE = 0,
	IMG_FLOAT = 1,
	IMG_INT = 2,
	IMG_UINT = 3,
	IMG_CHAR = 4,
	IMG_UCHAR = 5,
	IMG_UNSET = 6
} IMG_TYPE;


typedef union IMG_DATA_TYPE
{
	double * double_image_data;
	float * float_image_data;
	int * integer_image_data;
	unsigned int * unsigned_integer_image_data;
	char * character_image_data;
	unsigned char * unsigned_character_image_data;
} IMG_DATA_TYPE;


typedef struct IMG_DATA {
	unsigned int width, height;
	ROI_BBOX head_roi;
	ROI_BBOX * tail_roi;
	IMG_TYPE image_data_type;
	IMG_DATA_TYPE image_data;
} IMG_DATA;

typedef struct POSITION_NODE
{
	int position;
	unsigned int tree_depth;
	unsigned int count_left_leaves;
	POSITION_NODE * left_leaf;
	POSITION_NODE * right_leaf;
} POSITION_NODE;

void saveImagePGM(const char * filename, IMG_DATA * src_img);
IMG_DATA * createVoidImage(const int src_width, const int src_height, const IMG_TYPE src_img_type = IMG_DOUBLE);

void freeImageData(IMG_DATA * src_img_data_ptr);

void addImageROI(IMG_DATA * src_image_data_ptr,
	const ROI_BBOX_TYPE new_roi_type,
	const int src_UL_x,
	const int src_UL_y,
	const int src_UR_x,
	const int src_UR_y,
	const int src_LR_x,
	const int src_LR_y,
	const int src_LL_x,
	const int src_LL_y);

int addPositionLeaf(POSITION_NODE * src_current_leaf, const int src_new_position);
POSITION_NODE * newPositionLeaf(const unsigned int src_new_position);
int * dumpPositionsTree(POSITION_NODE * tree_root);
void dumpPositionLeaf(POSITION_NODE * src_current_leaf, const unsigned int src_count_positions_left, int * dst_positions_array);
void freePositionsTree(POSITION_NODE * src_tree_root);

IMG_DATA * createFromImageData(const IMG_DATA * src_img);

void copyImageData(const IMG_DATA * src_img, IMG_DATA * dst_img);

#endif //IMAGE_FUNCTIONS_INCLUDED