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
};

typedef struct ROI_BBOX /* [R]egion [O]f [I]nterest [B]ounding [BOX]*/
{
	ROI_BBOX_TYPE ROI_type;
	int UL_x, UL_y;
	int UR_x, UR_y;
	int LR_x, LR_y;
	int LL_x, LL_y;
	ROI_BBOX * next_roi;
};

typedef struct IMG_DATA {
	unsigned int width, height;
	ROI_BBOX head_roi;
	ROI_BBOX * tail_roi;
	double * image_data;
};

typedef struct POSITION_NODE
{
	int position;
	unsigned int tree_depth;
	unsigned int count_left_leaves;
	POSITION_NODE * left_leaf;
	POSITION_NODE * right_leaf;
};

IMG_DATA * loadImagePGM(const char * filename);
void saveImagePGM(const char * filename, IMG_DATA * src_img);
double computeLoss(IMG_DATA * src_diff_img);

IMG_DATA * computeDerivativesX(IMG_DATA * src_img);
IMG_DATA * computeDerivativesY(IMG_DATA * src_img);
IMG_DATA * createVoidImage(const unsigned int src_width, const unsigned int src_height);
IMG_DATA * diffImage(IMG_DATA * src_img, IMG_DATA * trg_img, const double delta_x, const double delta_y);

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