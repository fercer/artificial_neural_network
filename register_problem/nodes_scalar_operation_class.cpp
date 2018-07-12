#include "nodes_scalar_operation_class.h"

NODES_SCALAR_OPERATION::NODES_SCALAR_OPERATION()
{		
	input_numeric_nodes_required = 0;
	input_string_nodes_required = 0;
}


NODES_SCALAR_OPERATION::~NODES_SCALAR_OPERATION()
{
	emptyNodeScalarOperation();
}
	

double NODES_SCALAR_OPERATION::getScalarValue()
{
	performNodeScalarOperation();
	return *(*scalar_pointer_manager + array_position);
}


unsigned int NODES_SCALAR_OPERATION::getInputNumericNodesRequired()
{
	return input_numeric_nodes_required;
}


unsigned int NODES_SCALAR_OPERATION::getInputStringNodesRequired()
{
	return input_string_nodes_required;
}


char * NODES_SCALAR_OPERATION::getNumericInputName(const unsigned int src_variable_index)
{
	return numeric_nodes_names_list.getNodeValue(src_variable_index)->getScalarValue();
}

char * NODES_SCALAR_OPERATION::getStringInputName(const unsigned int src_variable_index)
{
	return string_nodes_names_list.getNodeValue(src_variable_index)->getScalarValue();
}


void NODES_SCALAR_OPERATION::assignNodeValue(const unsigned int src_node_position, const double src_node_value)
{	
	if (src_node_position >= input_numeric_nodes_required)
	{
		return;
	}

	local_numeric_nodes_list.getNodeValue(src_node_position)->setScalarValue(src_node_value);

	numeric_node_is_local_list.assignNodeValue(src_node_position, true);
	numeric_nodes_list.assignNodeValue(src_node_position, local_numeric_nodes_list.getNodeValue(src_node_position));
}


void NODES_SCALAR_OPERATION::assignNodeValue(const unsigned int src_node_position, const char* src_node_value)
{
	if (src_node_position >= input_string_nodes_required)
	{
		return;
	}

	local_string_nodes_list.getNodeValue(src_node_position)->setScalarValue(src_node_value);
	string_node_is_local_list.assignNodeValue(src_node_position, true);
	string_nodes_list.assignNodeValue(src_node_position, local_string_nodes_list.getNodeValue(src_node_position));
}



void NODES_SCALAR_OPERATION::assignNodeValue(const unsigned int src_node_position, NODE_SCALAR<double> * src_node_pointer)
{
	if (src_node_position >= input_numeric_nodes_required)
	{
		return;
	}

	if (src_node_pointer)
	{
		numeric_node_is_local_list.assignNodeValue(src_node_position, false);
		numeric_nodes_list.assignNodeValue(src_node_position, src_node_pointer);
	}
	else
	{
		numeric_node_is_local_list.assignNodeValue(src_node_position, true);
		local_numeric_nodes_list.getNodeValue(src_node_position)->setScalarValue(numeric_nodes_list.getNodeValue(src_node_position)->getScalarValue());
		numeric_nodes_list.assignNodeValue(src_node_position, local_numeric_nodes_list.getNodeValue(src_node_position));
	}
}


void NODES_SCALAR_OPERATION::assignNodeValue(const unsigned int src_node_position, NODE_SCALAR<char*> * src_node_pointer)
{
	if (src_node_position >= input_string_nodes_required)
	{
		return;
	}

	if (src_node_pointer)
	{
		string_node_is_local_list.assignNodeValue(src_node_position, false);
		string_nodes_list.assignNodeValue(src_node_position, src_node_pointer);
	}
	else
	{
		string_node_is_local_list.assignNodeValue(src_node_position, true);
		local_string_nodes_list.getNodeValue(src_node_position)->setScalarValue(string_nodes_list.getNodeValue(src_node_position)->getScalarValue());
		string_nodes_list.assignNodeValue(src_node_position, local_string_nodes_list.getNodeValue(src_node_position));
	}
}



void NODES_SCALAR_OPERATION::emptyNodeScalarOperation()
{
	for (unsigned int node_index = 0; node_index < input_numeric_nodes_required; node_index++)
	{
		delete numeric_nodes_names_list.getNodeValue(node_index);
		delete local_numeric_nodes_list.getNodeValue(node_index);
		delete local_previous_numeric_nodes_values_list.getNodeValue(node_index);
	}

	for (unsigned int node_index = 0; node_index < input_string_nodes_required; node_index++)
	{
		delete string_nodes_names_list.getNodeValue(node_index);
		delete local_string_nodes_list.getNodeValue(node_index);
		delete local_previous_string_nodes_values_list.getNodeValue(node_index);
	}
}



void NODES_SCALAR_OPERATION::copyFromNodesScalarOperation(const NODES_SCALAR_OPERATION& src_nodes_scalar_operation)
{
	copyFromNodeScalar(src_nodes_scalar_operation);
		
	this->input_numeric_nodes_required = src_nodes_scalar_operation.input_numeric_nodes_required;
	this->input_string_nodes_required = src_nodes_scalar_operation.input_string_nodes_required;

	this->numeric_node_is_local_list = src_nodes_scalar_operation.numeric_node_is_local_list;
	this->numeric_nodes_list = src_nodes_scalar_operation.numeric_nodes_list;

	const unsigned int numeric_nodes_count = this->local_numeric_nodes_list.getNodesCount();
	for (unsigned int node_index = 0; node_index < numeric_nodes_count; node_index++)
	{
		// Copy the current numeric value to the local list:
		this->local_numeric_nodes_list.getNodeValue(node_index)->setScalarValue(this->numeric_nodes_list.getNodeValue(node_index)->getScalarValue());

		// Verify if the source list is linked to an external node, or to its local node:
		if (this->numeric_node_is_local_list.getNodeValue(node_index))
		{
			this->numeric_nodes_list.assignNodeValue(node_index, this->local_numeric_nodes_list.getNodeValue(node_index));
		}
	}

	this->string_node_is_local_list = src_nodes_scalar_operation.string_node_is_local_list;
	this->string_nodes_list = src_nodes_scalar_operation.string_nodes_list;

	const unsigned int string_nodes_count = this->string_nodes_list.getNodesCount();
	for (unsigned int node_index = 0; node_index < string_nodes_count; node_index++)
	{
		// Copy the current numeric value to the local list:
		this->local_string_nodes_list.getNodeValue(node_index)->setScalarValue(this->string_nodes_list.getNodeValue(node_index)->getScalarValue());

		// Verify if the source list is linked to an external node, or to its local node:
		if (this->string_node_is_local_list.getNodeValue(node_index))
		{
			this->string_nodes_list.assignNodeValue(node_index, this->local_string_nodes_list.getNodeValue(node_index));
		}
	}
}



void NODES_SCALAR_OPERATION::performNodeScalarOperation()
{

	bool parameters_have_changed = false;
	for (unsigned int node_index = 0; node_index < input_numeric_nodes_required; node_index++)
	{
		if (numeric_nodes_list.getNodeValue(node_index)->getScalarValue() !=
			local_previous_numeric_nodes_values_list.getNodeValue(node_index)->getScalarValue())
		{
			local_previous_numeric_nodes_values_list.getNodeValue(node_index)->setScalarValue(numeric_nodes_list.getNodeValue(node_index)->getScalarValue());
			parameters_have_changed = true;
		}
	}

	for (unsigned int node_index = 0; node_index < input_string_nodes_required; node_index++)
	{
		if (strcmp(string_nodes_list.getNodeValue(node_index)->getScalarValue(),
			local_previous_string_nodes_values_list.getNodeValue(node_index)->getScalarValue()) != 0)
		{
			local_previous_string_nodes_values_list.getNodeValue(node_index)->setScalarValue(string_nodes_list.getNodeValue(node_index)->getScalarValue());
			parameters_have_changed = true;
		}
	}

	if (parameters_have_changed)
	{
		*(*scalar_pointer_manager + array_position) = performScalarOperation();
	}
}
