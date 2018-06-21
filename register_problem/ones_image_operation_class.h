#ifndef ONES_IMAGE_OPERATION_CLASS_H_INCLUDED
#define ONES_IMAGE_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_operation_class.h"

class ONES_IMAGE_OPERATION :
	public IMAGE_OPERATION
{
public:
	ONES_IMAGE_OPERATION();
	ONES_IMAGE_OPERATION(const ONES_IMAGE_OPERATION & src_ones_image_operation);
	ONES_IMAGE_OPERATION operator = (const ONES_IMAGE_OPERATION & src_ones_image_operation);
	~ONES_IMAGE_OPERATION();

protected:
	void performOperation();
};

#endif // ONES_IMAGE_OPERATION_CLASS_H_INCLUDED
