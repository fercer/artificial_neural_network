#ifndef NODES_SCALAR_OPERATION_CLASS_H_INCLUDED
#define NODES_SCALAR_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generic_list_class.h"
#include "node_scalar_class.h"

class NODES_SCALAR_OPERATION :
	public NODE_SCALAR<double>
{
public:
	NODES_SCALAR_OPERATION()
	{		
		parameters_have_hanged = false;
		input_numeric_nodes_required = 0;
		input_string_nodes_required = 0;
	}


	virtual ~NODES_SCALAR_OPERATION()
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
		if (parameters_have_hanged)
		{
			*(*scalar_pointer_manager + array_position) = performScalarOperation();
			parameters_have_hanged = false;
		}
		return *(*scalar_pointer_manager + array_position);
	}


	unsigned int getInputNumericNodesRequired()
	{
		return input_numeric_nodes_required;
	}


	unsigned int getInputStringNodesRequired()
	{
		return input_string_nodes_required;
	}


	char * getInputName(const unsigned int src_variable_index)
	{
		return nodes_names_list.getNodeValue(src_variable_index).getScalarValue();
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
			parameters_have_hanged = true;
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
			parameters_have_hanged = true;
		}
		else
		{
			string_node_is_local_list.assignNodeValue(src_node_position, true);
			string_nodes_list.assignNodeValue(src_node_position, local_string_nodes_list.getNodeValue(src_node_position));
		}
	}

protected:	
	unsigned int input_numeric_nodes_required;
	unsigned int input_string_nodes_required;

	GENERIC_LIST<NODE_SCALAR<char*>> nodes_names_list;
	GENERIC_LIST<NODE_SCALAR<bool>> numeric_node_is_local_list;
	GENERIC_LIST<NODE_SCALAR<double>*> local_numeric_nodes_list;
	GENERIC_LIST<NODE_SCALAR<bool>> string_node_is_local_list;
	GENERIC_LIST<NODE_SCALAR<char*>*> local_string_nodes_list;

	GENERIC_LIST<NODE_SCALAR<double>*> numeric_nodes_list;
	GENERIC_LIST<NODE_SCALAR<char*>*> string_nodes_list;

	// Use this function when implementing the copy constructor, and assignation operator (operator =), in derived classes:
	void copyFromNodesScalarOperation(const NODES_SCALAR_OPERATION& src_nodes_scalar_operation)
	{
		copyFromNodeScalar(src_nodes_scalar_operation);

		this->parameters_have_hanged = src_nodes_scalar_operation.parameters_have_hanged;

		this->input_numeric_nodes_required = src_nodes_scalar_operation.input_numeric_nodes_required;
		this->input_string_nodes_required = src_nodes_scalar_operation.input_string_nodes_required;
		
		this->nodes_names_list = src_nodes_scalar_operation.nodes_names_list;
		this->numeric_node_is_local_list = src_nodes_scalar_operation.numeric_node_is_local_list;

		this->numeric_nodes_list = src_nodes_scalar_operation.numeric_nodes_list;

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

		this->local_string_nodes_list = src_nodes_scalar_operation.local_string_nodes_list;

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
	}

	// Each derived class must define this function:
	virtual double performScalarOperation() = 0;

private:
	bool parameters_have_hanged;

};

#endif // NODES_SCALAR_OPERATION_CLASS_H_INCLUDED
