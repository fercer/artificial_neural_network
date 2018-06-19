#ifndef TRANSLATE_IMAGE_OPERATION_CLASS_H_INCLUDED
#define TRANSLATE_IMAGE_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_operation_class.h"

class TRANSLATE_IMAGE_OPERATION :
	public IMAGE_OPERATION
{
public:
	TRANSLATE_IMAGE_OPERATION();
	TRANSLATE_IMAGE_OPERATION(const TRANSLATE_IMAGE_OPERATION & );
	TRANSLATE_IMAGE_OPERATION operator = (const TRANSLATE_IMAGE_OPERATION & src_translate_image_operation);
	~TRANSLATE_IMAGE_OPERATION();

	void setDeltaX(const double src_delta_x);
	void setDeltaX(NODE_SCALAR<double> * src_delta_x_node);
	void setDeltaY(const double src_delta_y);
	void setDeltaY(NODE_SCALAR<double> * src_delta_y_node);
protected:
	void performOperation();

private:
	NODE_SCALAR<double> parameter_dx;
	NODE_SCALAR<double> parameter_dy;
};

#endif // TRANSLATE_IMAGE_OPERATION_CLASS_H_INCLUDED
