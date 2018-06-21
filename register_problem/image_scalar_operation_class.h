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
		parameters_have_changed = true;

		operation = NULL;

		src_img = NULL;

		input_numeric_nodes_required = 0;
		input_string_nodes_required = 0;
	}

	virtual ~IMAGE_SCALAR_OPERATION() 
	{
		const unsigned int numeric_nodes_count = local_numeric_nodes_list.getNodesCount();
		for (unsigned int node_index = 0; node_index < numeric_nodes_count; node_index++)
		{
			free(local_numeric_nodes_list.getNodeValue(node_index));
		}

		const unsigned int string_nodes_count = local_string_nodes_list.getNodesCount();
		for (unsigned int node_index = 0; node_index < string_nodes_count; node_index++)
		{
			free(local_string_nodes_list.getNodeValue(node_index));
		}
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

			*(*scalar_pointer_manager + array_position) = performScalarOperation();
			input_has_changed = false;
		}
		return *(*scalar_pointer_manager + array_position);
	}

	void setInputOperation(IMAGE_OPERATION * src_operation)
	{
		operation = src_operation;
		input_has_changed = true;
	}

	void assignNodePointer(const unsigned int src_node_position, NODE_SCALAR<double> * src_node_pointer)
	{
		if (src_node_position >= input_numeric_nodes_required)
		{
			return;
		}

		if (src_node_pointer)
		{
			numeric_node_is_local_list.assignNodeValue(src_node_position, false);
			numeric_nodes_list.assignNodeValue(src_node_position, src_node_pointer);
			parameters_have_changed = true;
		}
		else
		{
			numeric_node_is_local_list.assignNodeValue(src_node_position, true);
			numeric_nodes_list.assignNodeValue(src_node_position, local_numeric_nodes_list.getNodeValue(src_node_position));
		}
	}


	void assignNodePointer(const unsigned int src_node_position, NODE_SCALAR<char*> * src_node_pointer)
	{
		if (src_node_position >= input_string_nodes_required)
		{
			return;
		}

		if (src_node_pointer)
		{
			string_node_is_local_list.assignNodeValue(src_node_position, false);
			string_nodes_list.assignNodeValue(src_node_position, src_node_pointer);
			parameters_have_changed = true;
		}
		else
		{
			string_node_is_local_list.assignNodeValue(src_node_position, true);
			string_nodes_list.assignNodeValue(src_node_position, local_string_nodes_list.getNodeValue(src_node_position));
		}
	}

protected:
	unsigned int width;
	unsigned int height;


	unsigned int input_numeric_nodes_required;
	unsigned int input_string_nodes_required;

	GENERIC_LIST<NODE_SCALAR<char*>> nodes_names_list;
	GENERIC_LIST<NODE_SCALAR<bool>> numeric_node_is_local_list;
	GENERIC_LIST<NODE_SCALAR<double>*> local_numeric_nodes_list;
	GENERIC_LIST<NODE_SCALAR<bool>> string_node_is_local_list;
	GENERIC_LIST<NODE_SCALAR<char*>*> local_string_nodes_list;

	GENERIC_LIST<NODE_SCALAR<double>*> numeric_nodes_list;
	GENERIC_LIST<NODE_SCALAR<char*>*> string_nodes_list;
	
	IMG_DATA * src_img;
	
	// Use this function when implementing the copy constructor, and assignation operator (operator =), in derived classes:
	void copyFromImageScalarOperation(const IMAGE_SCALAR_OPERATION& src_image_scalar_operation)
	{
		copyFromNodeScalar(src_image_scalar_operation);

		this->parameters_have_changed = src_image_scalar_operation.parameters_have_changed;

		this->input_numeric_nodes_required = src_image_scalar_operation.input_numeric_nodes_required;
		this->input_string_nodes_required = src_image_scalar_operation.input_string_nodes_required;

		this->nodes_names_list = src_image_scalar_operation.nodes_names_list;
		this->numeric_node_is_local_list = src_image_scalar_operation.numeric_node_is_local_list;

		this->numeric_nodes_list = src_image_scalar_operation.numeric_nodes_list;

		const unsigned int numeric_nodes_count = this->local_numeric_nodes_list.getNodesCount();
		for (unsigned int node_index = 0; node_index < numeric_nodes_count; node_index++)
		{
			// Copy the current numeric value to the local list:
			NODE_SCALAR<double> * new_numeric_node = (NODE_SCALAR<double> *)malloc(sizeof(NODE_SCALAR<double>));
			new_numeric_node->setScalarValue(this->numeric_nodes_list.getNodeValue(node_index)->getScalarValue());
			this->local_numeric_nodes_list.assignNodeValue(node_index, new_numeric_node);

			// Verify if the source list is linked to an external node, or to its local node:
			if (this->numeric_node_is_local_list.getNodeValue(node_index).getScalarValue())
			{
				this->numeric_nodes_list.assignNodeValue(node_index, new_numeric_node);
			}
		}

		this->local_string_nodes_list = src_image_scalar_operation.local_string_nodes_list;

		const unsigned int string_nodes_count = this->local_string_nodes_list.getNodesCount();
		for (unsigned int node_index = 0; node_index < string_nodes_count; node_index++)
		{
			// Copy the current numeric value to the local list:
			NODE_SCALAR<char*> * new_string_node = (NODE_SCALAR<char*> *)malloc(sizeof(NODE_SCALAR<char*>));
			new_string_node->setScalarValue(this->string_nodes_list.getNodeValue(node_index)->getScalarValue());
			this->local_string_nodes_list.assignNodeValue(node_index, new_string_node);

			// Verify if the source list is linked to an external node, or to its local node:
			if (this->string_node_is_local_list.getNodeValue(node_index).getScalarValue())
			{
				this->local_string_nodes_list.assignNodeValue(node_index, new_string_node);
			}
		}

		this->input_has_changed = src_image_scalar_operation.input_has_changed;

		this->operation = src_image_scalar_operation.operation;

		this->src_img = this->operation->getImageData();
		
		this->width = src_image_scalar_operation.width;
		this->height = src_image_scalar_operation.height;
	}

	// Each derived class must define this function:
	virtual double performScalarOperation() = 0;
	
private:
	bool input_has_changed;
	bool parameters_have_changed;
	IMAGE_OPERATION * operation;
};

#endif // IMAGE_OPERATION_CLASS_H_INCLUDED
