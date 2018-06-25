#include "scalar_tan_nodes_operation_class.h"

SCALAR_TAN_NODES_OPERATION::SCALAR_TAN_NODES_OPERATION()
{
	input_numeric_nodes_required = 1;
	NODE_SCALAR<char*> * node_A_identifier = new NODE_SCALAR<char*>("node_A");
	numeric_nodes_names_list.assignNodeValue(0, node_A_identifier);

	NODE_SCALAR<double> * local_node_A = new NODE_SCALAR<double>(0.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);
}



SCALAR_TAN_NODES_OPERATION::SCALAR_TAN_NODES_OPERATION(const SCALAR_TAN_NODES_OPERATION & src_scalar_tan_nodes_operation)
{
	input_numeric_nodes_required = 1;
	NODE_SCALAR<char*> * node_A_identifier = new NODE_SCALAR<char*>("node_A");
	numeric_nodes_names_list.assignNodeValue(0, node_A_identifier);

	NODE_SCALAR<double> * local_node_A = new NODE_SCALAR<double>(0.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);

	copyFromNodesScalarOperation(src_scalar_tan_nodes_operation);
}


SCALAR_TAN_NODES_OPERATION SCALAR_TAN_NODES_OPERATION::operator=(const SCALAR_TAN_NODES_OPERATION & src_scalar_tan_nodes_operation)
{
	if (this != &src_scalar_tan_nodes_operation)
	{
		copyFromNodesScalarOperation(src_scalar_tan_nodes_operation);
	}

	return *this;
}


SCALAR_TAN_NODES_OPERATION::~SCALAR_TAN_NODES_OPERATION()
{

}


double SCALAR_TAN_NODES_OPERATION::performScalarOperation()
{
	return tan(numeric_nodes_list.getNodeValue(0)->getScalarValue());
}