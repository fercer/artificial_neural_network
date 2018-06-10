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

protected:
	void performOperation();

private:
	inline double bicubicInterpolation(double *src_img, const double x, const double y);
};

#endif // ROTATE_IMAGE_OPERATION_CLASS_H_INCLUDED