#ifndef MULT_IMAGE_OPERATION_CLASS_H_INCLUDED
#define MULT_IMAGE_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_operation_class.h"

class MULT_IMAGE_OPERATION :
	public IMAGE_OPERATION
{
public:
	MULT_IMAGE_OPERATION();
	MULT_IMAGE_OPERATION(const MULT_IMAGE_OPERATION & src_mult_image_operation);
	MULT_IMAGE_OPERATION operator = (const MULT_IMAGE_OPERATION & src_mult_image_operation);
	~MULT_IMAGE_OPERATION();

	void assignParameterA(const double src_parameter_A);
	void assignParameterB(const double src_parameter_B);

protected:
	void performOperation();

private:
	NODE_SCALAR<double> parameter_A;
	NODE_SCALAR<double> parameter_B;
};

#endif // MULT_IMAGE_OPERATION_CLASS_H_INCLUDED
