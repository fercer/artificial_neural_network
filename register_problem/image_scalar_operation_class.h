#ifndef IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
#define IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_functions.h"
#include "image_operation_class.h"

class IMAGE_SCALAR_OPERATION
{
public:
	IMAGE_SCALAR_OPERATION()
	{
		input_has_changed = false;

		operation = NULL;

		src_img = NULL;

		parameter = 0.0;

		result = 0.0;
	}

	virtual ~IMAGE_SCALAR_OPERATION() = 0;
	
	double getImageScalar() 
	{
		if (input_has_changed)
		{
			if (operation)
			{
				src_img = operation->getImageData();
			}

			result = performScalarOperation();
			input_has_changed = false;
		}
		return result;
	}

	void setInputOperation(IMAGE_OPERATION * src_operation)
	{
		src_operation = src_operation;
		input_has_changed = true;
	}

	void setParameter(const double src_parameter)
	{
		parameter = src_parameter;
	}


protected:
	unsigned int width;
	unsigned int height;

	double parameter;
	
	IMG_DATA * src_img;
	
	// Use this function when implementing the copy constructor, and assignation operator (operator =), in derived classes:
	void copyFromImageScalarOperation(const IMAGE_SCALAR_OPERATION& src_image_scalar_operation)
	{
		this->input_has_changed = src_image_scalar_operation.input_has_changed;

		this->operation = src_image_scalar_operation.operation;

		this->src_img = this->operation->getImageData();
		
		this->parameter = src_image_scalar_operation.parameter;

		this->width = src_image_scalar_operation.width;
		this->height = src_image_scalar_operation.height;
	}

	// Each derived class must define this function:
	virtual double performScalarOperation() = 0;
	
private:
	bool input_has_changed;
	double result;
	IMAGE_OPERATION * operation;
};

#endif // IMAGE_OPERATION_CLASS_H_INCLUDED
