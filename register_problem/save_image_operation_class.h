#ifndef SAVE_IMAGE_OPERATION_CLASS_H_INCLUDED
#define SAVE_IMAGE_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_operation_class.h"

class SAVE_IMAGE_OPERATION :
	public IMAGE_OPERATION
{
public:
	SAVE_IMAGE_OPERATION();
	SAVE_IMAGE_OPERATION(const SAVE_IMAGE_OPERATION & src_save_image_operation);
	SAVE_IMAGE_OPERATION operator = (const SAVE_IMAGE_OPERATION & src_save_image_operation);
	~SAVE_IMAGE_OPERATION();

protected:
	void performOperation();
};

#endif // SAVE_IMAGE_OPERATION_CLASS_H_INCLUDED
