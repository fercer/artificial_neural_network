#include "scalar_sqrt_nodes_operation_class.h"

SCALAR_SQRT_NODES_OPERATION::SCALAR_SQRT_NODES_OPERATION()
{
	input_numeric_nodes_required = 1;
	NODE_SCALAR<char*> node_identifier("node_A");
	numeric_nodes_names_list.assignNodeValue(0, node_identifier);

	NODE_SCALAR<double> local_node_A(0.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, &local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);
}



SCALAR_SQRT_NODES_OPERATION::SCALAR_SQRT_NODES_OPERATION(const SCALAR_SQRT_NODES_OPERATION & src_scalar_sqrt_nodes_operation)
{
	copyFromNodesScalarOperation(src_scalar_sqrt_nodes_operation);
}


SCALAR_SQRT_NODES_OPERATION SCALAR_SQRT_NODES_OPERATION::operator=(const SCALAR_SQRT_NODES_OPERATION & src_scalar_sqrt_nodes_operation)
{
	if (this != &src_scalar_sqrt_nodes_operation)
	{
		copyFromNodesScalarOperation(src_scalar_sqrt_nodes_operation);
	}

	return *this;
}


SCALAR_SQRT_NODES_OPERATION::~SCALAR_SQRT_NODES_OPERATION()
{
	// Nothing to deallocate
}


double SCALAR_SQRT_NODES_OPERATION::performScalarOperation()
{
	return sqrt(numeric_nodes_list.getNodeValue(0)->getScalarValue());
}