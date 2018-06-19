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

	void setWidth(const unsigned int src_width);
	void setWidth(NODE_SCALAR<double> * src_width_node);
	void setHeight(const unsigned int src_height);
	void setHeight(NODE_SCALAR<double> * src_height_node);
	void setFillValue(const double src_fill_value);
	void setFillValue(NODE_SCALAR<double> * src_fill_value_node);

protected:
	void performOperation();

private:
	NODE_SCALAR<double> height_parameter;
	NODE_SCALAR<double> width_parameter;
	NODE_SCALAR<double> fill_parameter;
};

#endif // ONES_IMAGE_OPERATION_CLASS_H_INCLUDED
