#ifndef REGISTER_PROBLEM_INCLUDED
#define REGISTER_PROBLEM_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MY_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164

typedef struct IMG_DATA {
	unsigned int width, height;
	double min_value, max_value;
	unsigned int UL_x, UL_y;
	unsigned int UR_x, UR_y;
	unsigned int LR_x, LR_y;
	unsigned int LL_x, LL_y;
	double * image_data;
} ;

IMG_DATA * loadImagePGM(const char * filename);

void saveImagePGM(const char * filename, IMG_DATA * src_img);

IMG_DATA * presetProblem(IMG_DATA * src_img, IMG_DATA * trg_img);

inline double bicubicInterpolation(IMG_DATA *src_img, const double x, const double y);

IMG_DATA * rotateBicubic(IMG_DATA * src_img, const double theta_11, const double theta_12, const double theta_21, const double theta_22);

double computeLoss(IMG_DATA * src_img, IMG_DATA * trg_img, const double delta_x, const double delta_y);

IMG_DATA * computeDerivativesX(IMG_DATA * src_img);
IMG_DATA * computeDerivativesY(IMG_DATA * src_img);

IMG_DATA * createVoidImage(const unsigned int src_width, const unsigned int src_height);

IMG_DATA * diffImage(IMG_DATA * src_img, IMG_DATA * trg_img, const double delta_x, const double delta_y);

#endif //REGISTER_PROBLEM_INCLUDED