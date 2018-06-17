#ifndef IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
#define IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_functions.h"
#include "image_operation_class.h"
#include "node_scalar_class.h"
#include "generic_list_class.h"

class IMAGE_SCALAR_OPERATION :
	public NODE_SCALAR<double>
{
public:
	IMAGE_SCALAR_OPERATION()
	{
		input_has_changed = false;
		parameters_have_hanged = false;

		operation = NULL;

		src_img = NULL;

		numeric_parameters_count = 0;
		string_parameters_count = 0;

		result = 0.0;
	}

	virtual ~IMAGE_SCALAR_OPERATION() 
	{
	}
	
	double getScalarValue() 
	{
		if (input_has_changed)
		{
			if (operation)
			{
				src_img = operation->getImageData();
				width = src_img->width;
				height = src_img->height;
			}

			result = performScalarOperation();
			input_has_changed = false;
		}
		return result;
	}

	void setInputOperation(IMAGE_OPERATION * src_operation)
	{
		operation = src_operation;
		input_has_changed = true;
	}

	void setParameter(NODE_SCALAR<double> * src_parameter_node_pointer, const unsigned int src_parameter_index = 0)
	{
		numeric_parameters_count = numeric_parameters.assignNodeValue(src_parameter_index, src_parameter_node_pointer);
		parameters_have_hanged = true;
	}

	void setParameter(NODE_SCALAR<char*> * src_parameter_node_pointer, const unsigned int src_parameter_index = 0)
	{
		string_parameters_count = string_parameters.assignNodeValue(src_parameter_index, src_parameter_node_pointer);
		parameters_have_hanged = true;
	}

protected:
	unsigned int width;
	unsigned int height;

	GENERIC_LIST<NODE_SCALAR<double>*> numeric_parameters;
	unsigned int numeric_parameters_count;
	GENERIC_LIST<NODE_SCALAR<char*>*> string_parameters;
	unsigned int string_parameters_count;
	
	IMG_DATA * src_img;
	
	// Use this function when implementing the copy constructor, and assignation operator (operator =), in derived classes:
	void copyFromImageScalarOperation(const IMAGE_SCALAR_OPERATION& src_image_scalar_operation)
	{
		this->input_has_changed = src_image_scalar_operation.input_has_changed;

		this->operation = src_image_scalar_operation.operation;

		this->src_img = this->operation->getImageData();
		
		this->numeric_parameters = src_image_scalar_operation.numeric_parameters;
		this->numeric_parameters_count = src_image_scalar_operation.numeric_parameters_count;
		this->string_parameters = src_image_scalar_operation.string_parameters;
		this->string_parameters_count = src_image_scalar_operation.string_parameters_count;

		this->width = src_image_scalar_operation.width;
		this->height = src_image_scalar_operation.height;
	}

	// Each derived class must define this function:
	virtual double performScalarOperation() = 0;
	
private:
	bool input_has_changed;
	bool parameters_have_hanged;
	double result;
	IMAGE_OPERATION * operation;
};

#endif // IMAGE_OPERATION_CLASS_H_INCLUDED
