#ifndef ROTATE_IMAGE_OPERATION_CLASS_H_INCLUDED
#define ROTATE_IMAGE_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_operation_class.h"


#define MY_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164


class ROTATE_IMAGE_OPERATION :
	public IMAGE_OPERATION
{
public:
	ROTATE_IMAGE_OPERATION();
	ROTATE_IMAGE_OPERATION(const ROTATE_IMAGE_OPERATION & src_rotate_image_operation);
	ROTATE_IMAGE_OPERATION operator = (const ROTATE_IMAGE_OPERATION & src_rotate_image_operation);
	~ROTATE_IMAGE_OPERATION();

	void setRotationAngle(const double src_theta);
	void setRotationAngle(const double src_theta, const double src_phi);
	void setRotationAngle(const double src_rotaiton_matrix_entry_11, const double src_rotaiton_matrix_entry_12, const double src_rotaiton_matrix_entry_21, const double src_rotaiton_matrix_entry_22);

	void setRotationAngle11(NODE_SCALAR<double> * src_rotaiton_matrix_entry_11_node);
	void setRotationAngle12(NODE_SCALAR<double> * src_rotaiton_matrix_entry_12_node);
	void setRotationAngle21(NODE_SCALAR<double> * src_rotaiton_matrix_entry_21_node);
	void setRotationAngle22(NODE_SCALAR<double> * src_rotaiton_matrix_entry_22_node);

protected:
	void performOperation();

private:
	inline double bicubicInterpolation(double *src_img, const double x, const double y);

	NODE_SCALAR<double> rotation_matrix_parameter_11;
	NODE_SCALAR<double> rotation_matrix_parameter_12;
	NODE_SCALAR<double> rotation_matrix_parameter_21;
	NODE_SCALAR<double> rotation_matrix_parameter_22;
};

#endif // ROTATE_IMAGE_OPERATION_CLASS_H_INCLUDED
