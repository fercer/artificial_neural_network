#ifndef HEIGHT_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
#define HEIGHT_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <limits>

#include "image_scalar_operation_class.h"

class HEIGHT_IMAGE_SCALAR_OPERATION :
	public IMAGE_SCALAR_OPERATION
{
public:
	HEIGHT_IMAGE_SCALAR_OPERATION();
	HEIGHT_IMAGE_SCALAR_OPERATION(const HEIGHT_IMAGE_SCALAR_OPERATION & src_height_image_scalar_operation);
	HEIGHT_IMAGE_SCALAR_OPERATION operator = (const HEIGHT_IMAGE_SCALAR_OPERATION & src_height_image_scalar_operation);
	~HEIGHT_IMAGE_SCALAR_OPERATION();

protected:
	double performScalarOperation();
};

#endif // HEIGHT_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
