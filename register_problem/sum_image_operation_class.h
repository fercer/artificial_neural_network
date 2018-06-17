#ifndef SUM_IMAGE_OPERATION_CLASS_H_INCLUDED
#define SUM_IMAGE_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_operation_class.h"

class SUM_IMAGE_OPERATION :
	public IMAGE_OPERATION
{
public:
	SUM_IMAGE_OPERATION();
	SUM_IMAGE_OPERATION(const SUM_IMAGE_OPERATION & src_sum_image_operation);
	SUM_IMAGE_OPERATION operator = (const SUM_IMAGE_OPERATION & src_sum_image_operation);
	~SUM_IMAGE_OPERATION();

	void assignParameterA(const double src_parameter_A);
	void assignParameterB(const double src_parameter_B);

protected:
	void performOperation();

private:
	NODE_SCALAR<double> parameter_A;
	NODE_SCALAR<double> parameter_B;
};

#endif // SUM_IMAGE_OPERATION_CLASS_H_INCLUDED
