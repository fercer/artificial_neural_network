#ifndef SQROOT_IMAGE_OPERATION_CLASS_H_INCLUDED
#define SQROOT_IMAGE_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_operation_class.h"

class SQROOT_IMAGE_OPERATION :
	public IMAGE_OPERATION
{
public:
	SQROOT_IMAGE_OPERATION();
	SQROOT_IMAGE_OPERATION(const SQROOT_IMAGE_OPERATION & src_sum_image_operation);
	SQROOT_IMAGE_OPERATION operator = (const SQROOT_IMAGE_OPERATION & src_sum_image_operation);
	~SQROOT_IMAGE_OPERATION();


	void assignParameter(const double src_parameter);

protected:
	void performOperation();


private:
	NODE_SCALAR<double> parameter;
};

#endif // SQROOT_IMAGE_OPERATION_CLASS_H_INCLUDED
