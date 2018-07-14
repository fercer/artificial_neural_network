#ifndef POOL_IMAGE_OPERATION_CLASS_H_INCLUDED
#define POOL_IMAGE_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_operation_class.h"

class POOL_IMAGE_OPERATION :
	public IMAGE_OPERATION
{
public:
	POOL_IMAGE_OPERATION();
	POOL_IMAGE_OPERATION(const POOL_IMAGE_OPERATION & src_pool_image_operation);
	POOL_IMAGE_OPERATION operator = (const POOL_IMAGE_OPERATION & src_pool_image_operation);
	~POOL_IMAGE_OPERATION();

protected:
	void performOperation();
};

#endif // POOL_IMAGE_OPERATION_CLASS_H_INCLUDED
