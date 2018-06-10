#ifndef MAX_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
#define MAX_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <limits>

#include "image_scalar_operation_class.h"

class MAX_IMAGE_SCALAR_OPERATION :
	public IMAGE_SCALAR_OPERATION
{
public:
	MAX_IMAGE_SCALAR_OPERATION();
	MAX_IMAGE_SCALAR_OPERATION(const MAX_IMAGE_SCALAR_OPERATION & src_max_image_scalar_operation);
	MAX_IMAGE_SCALAR_OPERATION operator = (const MAX_IMAGE_SCALAR_OPERATION & src_max_image_scalar_operation);
	~MAX_IMAGE_SCALAR_OPERATION();

protected:
	double performScalarOperation();
};

#endif // MAX_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
