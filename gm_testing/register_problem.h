#ifndef REGISTER_PROBLEM_INCLUDED
#define REGISTER_PROBLEM_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MY_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164

double * loadImagePGM(const char * filename);

void saveImagePGM(const char * filename, double * src_img);

double * presetProblem(double * src_img, double * trg_img);

inline double bicubicInterpolation(const double *src_img, const double x, const double y);

void rotateBicubic(double* src_img, double* dst_img, const double theta, const double delta_x, const double delta_y);

double computeLoss(double * src_img, double * trg_img);

double computeLossPerPixel(const unsigned int pix_position, double * src_img, double * trg_img, const double theta, double * src_dx_img, double * src_dy_img, double * differences, double ** error_contributions);

void computeDerivatives(double * src_img, double * dst_dx_img, double * dst_dy_img);

#endif //REGISTER_PROBLEM_INCLUDED