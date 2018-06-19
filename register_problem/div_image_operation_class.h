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

	void setParameterA(const double src_parameter_A);
	void setParameterA(NODE_SCALAR<double> * src_node_A);
	void setParameterB(const double src_parameter_B);
	void setParameterB(NODE_SCALAR<double> * src_node_B);

protected:
	void performOperation();

private:
	NODE_SCALAR<double> parameter_A;
	NODE_SCALAR<double> parameter_B;
};

#endif // DIV_IMAGE_OPERATION_CLASS_H_INCLUDED
