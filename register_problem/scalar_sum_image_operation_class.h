#ifndef SCALAR_SUM_IMAGE_OPERATION_CLASS_H_INCLUDED
#define SCALAR_SUM_IMAGE_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_operation_class.h"

class SCALAR_SUM_IMAGE_OPERATION :
	public IMAGE_OPERATION
{
public:
	SCALAR_SUM_IMAGE_OPERATION();
	SCALAR_SUM_IMAGE_OPERATION(const SCALAR_SUM_IMAGE_OPERATION & src_scalar_sum_image_operation);
	SCALAR_SUM_IMAGE_OPERATION operator = (const SCALAR_SUM_IMAGE_OPERATION & src_sum_image_operation);
	~SCALAR_SUM_IMAGE_OPERATION();

protected:
	void performOperation();
};

#endif // SCALAR_SUM_IMAGE_OPERATION_CLASS_H_INCLUDED
