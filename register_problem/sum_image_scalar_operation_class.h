#ifndef SUM_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
#define SUM_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_scalar_operation_class.h"

class SUM_IMAGE_SCALAR_OPERATION :
	public IMAGE_SCALAR_OPERATION
{
public:
	SUM_IMAGE_SCALAR_OPERATION();
	SUM_IMAGE_SCALAR_OPERATION(const SUM_IMAGE_SCALAR_OPERATION & src_sum_image_scalar_operation);
	SUM_IMAGE_SCALAR_OPERATION operator = (const SUM_IMAGE_SCALAR_OPERATION & src_sum_image_scalar_operation);
	~SUM_IMAGE_SCALAR_OPERATION();

protected:
	double performScalarOperation();
};

#endif // SUM_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
