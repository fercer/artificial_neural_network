#ifndef WIDTH_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
#define WIDTH_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <limits>

#include "image_scalar_operation_class.h"

class WIDTH_IMAGE_SCALAR_OPERATION :
	public IMAGE_SCALAR_OPERATION
{
public:
	WIDTH_IMAGE_SCALAR_OPERATION();
	WIDTH_IMAGE_SCALAR_OPERATION(const WIDTH_IMAGE_SCALAR_OPERATION & src_width_image_scalar_operation);
	WIDTH_IMAGE_SCALAR_OPERATION operator = (const WIDTH_IMAGE_SCALAR_OPERATION & src_width_image_scalar_operation);
	~WIDTH_IMAGE_SCALAR_OPERATION();

protected:
	double performScalarOperation();
};

#endif // WIDTH_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
