#ifndef MIN_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
#define MIN_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <limits>

#include "image_scalar_operation_class.h"

class MIN_IMAGE_SCALAR_OPERATION :
	public IMAGE_SCALAR_OPERATION
{
public:
	MIN_IMAGE_SCALAR_OPERATION();
	MIN_IMAGE_SCALAR_OPERATION(const MIN_IMAGE_SCALAR_OPERATION & src_min_image_scalar_operation);
	MIN_IMAGE_SCALAR_OPERATION operator = (const MIN_IMAGE_SCALAR_OPERATION & src_min_image_scalar_operation);
	~MIN_IMAGE_SCALAR_OPERATION();

protected:
	double performScalarOperation();
};

#endif // MIN_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
