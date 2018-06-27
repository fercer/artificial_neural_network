#ifndef SAVE_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
#define SAVE_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_scalar_operation_class.h"

class SAVE_IMAGE_SCALAR_OPERATION :
	public IMAGE_SCALAR_OPERATION
{
public:
	SAVE_IMAGE_SCALAR_OPERATION();
	SAVE_IMAGE_SCALAR_OPERATION(const SAVE_IMAGE_SCALAR_OPERATION & src_save_image_scalar_operation);
	SAVE_IMAGE_SCALAR_OPERATION operator = (const SAVE_IMAGE_SCALAR_OPERATION & src_save_image_scalar_operation);
	~SAVE_IMAGE_SCALAR_OPERATION();

protected:
	double performScalarOperation();
};

#endif // SAVE_IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
