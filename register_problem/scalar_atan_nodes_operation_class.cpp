#include "scalar_atan_nodes_operation_class.h"

SCALAR_ATAN_NODES_OPERATION::SCALAR_ATAN_NODES_OPERATION()
{
	input_numeric_nodes_required = 2;
	NODE_SCALAR<char*> * node_A_identifier = new NODE_SCALAR<char*>("node_A");
	numeric_nodes_names_list.assignNodeValue(0, node_A_identifier);

	NODE_SCALAR<char*> * node_B_identifier = new NODE_SCALAR<char*>("node_B");
	numeric_nodes_names_list.assignNodeValue(1, node_B_identifier);

	NODE_SCALAR<double> * local_node_A = new NODE_SCALAR<double>(0.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<double> * local_node_B = new NODE_SCALAR<double>(1.0);
	local_numeric_nodes_list.assignNodeValue(1, local_node_B);
	numeric_nodes_list.assignNodeValue(1, local_numeric_nodes_list.getNodeValue(1));
	numeric_node_is_local_list.assignNodeValue(1, true);
}



SCALAR_ATAN_NODES_OPERATION::SCALAR_ATAN_NODES_OPERATION(const SCALAR_ATAN_NODES_OPERATION & src_scalar_atan_nodes_operation)
{
	input_numeric_nodes_required = 2;
	NODE_SCALAR<char*> * node_A_identifier = new NODE_SCALAR<char*>("node_A");
	numeric_nodes_names_list.assignNodeValue(0, node_A_identifier);

	NODE_SCALAR<char*> * node_B_identifier = new NODE_SCALAR<char*>("node_B");
	numeric_nodes_names_list.assignNodeValue(1, node_B_identifier);

	NODE_SCALAR<double> * local_node_A = new NODE_SCALAR<double>(0.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<double> * local_node_B = new NODE_SCALAR<double>(1.0);
	local_numeric_nodes_list.assignNodeValue(1, local_node_B);
	numeric_nodes_list.assignNodeValue(1, local_numeric_nodes_list.getNodeValue(1));
	numeric_node_is_local_list.assignNodeValue(1, true);

	copyFromNodesScalarOperation(src_scalar_atan_nodes_operation);
}


SCALAR_ATAN_NODES_OPERATION SCALAR_ATAN_NODES_OPERATION::operator=(const SCALAR_ATAN_NODES_OPERATION & src_scalar_atan_nodes_operation)
{
	if (this != &src_scalar_atan_nodes_operation)
	{
		copyFromNodesScalarOperation(src_scalar_atan_nodes_operation);
	}

	return *this;
}


SCALAR_ATAN_NODES_OPERATION::~SCALAR_ATAN_NODES_OPERATION()
{

}


double SCALAR_ATAN_NODES_OPERATION::performScalarOperation()
{
	return atan2(numeric_nodes_list.getNodeValue(1)->getScalarValue(), numeric_nodes_list.getNodeValue(0)->getScalarValue());
}