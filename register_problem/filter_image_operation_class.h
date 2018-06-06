#ifndef FILTER_IMAGE_OPERATION_CLASS_H_INCLUDED
#define FILTER_IMAGE_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <acml.h>

#include "image_operation_class.h"

class FILTER_IMAGE_OPERATION :
	public IMAGE_OPERATION
{
public:
	FILTER_IMAGE_OPERATION();
	FILTER_IMAGE_OPERATION(const FILTER_IMAGE_OPERATION & src_rotate_image_operation);
	FILTER_IMAGE_OPERATION operator = (const FILTER_IMAGE_OPERATION & src_rotate_image_operation);
	~FILTER_IMAGE_OPERATION();

protected:
	void performOperation();
};

#endif // FILTER_IMAGE_OPERATION_CLASS_H_INCLUDED
