#ifndef DIV_IMAGE_OPERATION_CLASS_H_INCLUDED
#define DIV_IMAGE_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_operation_class.h"

class DIV_IMAGE_OPERATION :
	public IMAGE_OPERATION
{
public:
	DIV_IMAGE_OPERATION();
	DIV_IMAGE_OPERATION(const DIV_IMAGE_OPERATION & src_div_image_operation);
	DIV_IMAGE_OPERATION operator = (const DIV_IMAGE_OPERATION & src_div_image_operation);
	~DIV_IMAGE_OPERATION();

protected:
	void performOperation();

};

#endif // DIV_IMAGE_OPERATION_CLASS_H_INCLUDED
