#include "scalar_sin_nodes_operation_class.h"

SCALAR_SIN_NODES_OPERATION::SCALAR_SIN_NODES_OPERATION()
{
	input_numeric_nodes_required = 1;
	NODE_SCALAR<char*> * node_A_identifier = new NODE_SCALAR<char*>("node_A");
	numeric_nodes_names_list.assignNodeValue(0, node_A_identifier);

	NODE_SCALAR<double> * local_node_A = new NODE_SCALAR<double>(0.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<double> * local_previous_node_A = new NODE_SCALAR<double>(0.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(0, local_previous_node_A);
}



SCALAR_SIN_NODES_OPERATION::SCALAR_SIN_NODES_OPERATION(const SCALAR_SIN_NODES_OPERATION & src_scalar_sin_nodes_operation)
{
	input_numeric_nodes_required = 1;
	NODE_SCALAR<char*> * node_A_identifier = new NODE_SCALAR<char*>("node_A");
	numeric_nodes_names_list.assignNodeValue(0, node_A_identifier);

	NODE_SCALAR<double> * local_node_A = new NODE_SCALAR<double>(0.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<double> * local_previous_node_A = new NODE_SCALAR<double>(0.0);
	local_previous_numeric_nodes_values_list.assignNodeValue(0, local_previous_node_A);

	copyFromNodesScalarOperation(src_scalar_sin_nodes_operation);
}


SCALAR_SIN_NODES_OPERATION SCALAR_SIN_NODES_OPERATION::operator=(const SCALAR_SIN_NODES_OPERATION & src_scalar_sin_nodes_operation)
{
	if (this != &src_scalar_sin_nodes_operation)
	{
		copyFromNodesScalarOperation(src_scalar_sin_nodes_operation);
	}

	return *this;
}


SCALAR_SIN_NODES_OPERATION::~SCALAR_SIN_NODES_OPERATION()
{

}


double SCALAR_SIN_NODES_OPERATION::performScalarOperation()
{
	return sin(numeric_nodes_list.getNodeValue(0)->getScalarValue());
}