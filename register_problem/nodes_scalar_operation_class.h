#ifndef NODES_SCALAR_OPERATION_CLASS_H_INCLUDED
#define NODES_SCALAR_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "generic_list_class.cpp"
#include "node_scalar_class.h"

class NODES_SCALAR_OPERATION :
	public NODE_SCALAR<double>
{
public:
	NODES_SCALAR_OPERATION();

	virtual ~NODES_SCALAR_OPERATION();	

	double getScalarValue();

	unsigned int getInputNumericNodesRequired();

	unsigned int getInputStringNodesRequired();

	char * getNumericInputName(const unsigned int src_variable_index);

	char * getStringInputName(const unsigned int src_variable_index);

	void assignNodeValue(const unsigned int src_node_position, const double src_node_value);

	void assignNodeValue(const unsigned int src_node_position, const char* src_node_value);

	void assignNodeValue(const unsigned int src_node_position, NODE_SCALAR<double> * src_node_pointer);

	void assignNodeValue(const unsigned int src_node_position, NODE_SCALAR<char*> * src_node_pointer);

protected:	
	unsigned int input_numeric_nodes_required;
	unsigned int input_string_nodes_required;

	GENERIC_LIST<NODE_SCALAR<char*>*> numeric_nodes_names_list;
	GENERIC_LIST<bool> numeric_node_is_local_list;
	GENERIC_LIST<NODE_SCALAR<double>*> local_numeric_nodes_list;
	GENERIC_LIST<NODE_SCALAR<double>*> local_previous_numeric_nodes_values_list;

	GENERIC_LIST<NODE_SCALAR<char*>*> string_nodes_names_list;
	GENERIC_LIST<bool> string_node_is_local_list;
	GENERIC_LIST<NODE_SCALAR<char*>*> local_string_nodes_list;
	GENERIC_LIST<NODE_SCALAR<char*>*> local_previous_string_nodes_values_list;

	GENERIC_LIST<NODE_SCALAR<double>*> numeric_nodes_list;
	GENERIC_LIST<NODE_SCALAR<char*>*> string_nodes_list;

	// Use this function when implementing the derived class destructor:
	void emptyNodeScalarOperation();

	// Use this function when implementing the copy constructor, and assignation operator (operator =), in derived classes:
	void copyFromNodesScalarOperation(const NODES_SCALAR_OPERATION& src_nodes_scalar_operation);

	// Each derived class must define this function:
	virtual double performScalarOperation() = 0;
	
	void defineGraphicNodeScalarOperation();

private:
	bool graphic_nodes_defined;
	LINK_NODE ** link_numeric_nodes_inputs;
	LINK_NODE ** link_string_nodes_inputs;

	void performNodeScalarOperation();
};

#endif // NODES_SCALAR_OPERATION_CLASS_H_INCLUDED
