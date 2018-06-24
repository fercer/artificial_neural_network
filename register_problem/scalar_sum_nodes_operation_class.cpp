#include "scalar_sum_nodes_operation_class.h"

SCALAR_SUM_NODES_OPERATION::SCALAR_SUM_NODES_OPERATION()
{
	input_numeric_nodes_required = 2;
	NODE_SCALAR<char*> node_identifier("node_A");
	numeric_nodes_names_list.assignNodeValue(0, node_identifier);

	node_identifier.setScalarValue("node_B");
	numeric_nodes_names_list.assignNodeValue(1, node_identifier);


	NODE_SCALAR<double> local_node_A(0.0);
	local_numeric_nodes_list.assignNodeValue(0, local_node_A);
	numeric_nodes_list.assignNodeValue(0, &local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);


	NODE_SCALAR<double> local_node_B(0.0);
	local_numeric_nodes_list.assignNodeValue(1, local_node_B);
	numeric_nodes_list.assignNodeValue(1, &local_numeric_nodes_list.getNodeValue(1));
	numeric_node_is_local_list.assignNodeValue(1, true);
}



SCALAR_SUM_NODES_OPERATION::SCALAR_SUM_NODES_OPERATION(const SCALAR_SUM_NODES_OPERATION & src_scalar_sum_nodes_operation)
{
	copyFromNodesScalarOperation(src_scalar_sum_nodes_operation);
}



SCALAR_SUM_NODES_OPERATION SCALAR_SUM_NODES_OPERATION::operator=(const SCALAR_SUM_NODES_OPERATION & src_scalar_sum_nodes_operation)
{
	if (this != &src_scalar_sum_nodes_operation)
	{
		copyFromNodesScalarOperation(src_scalar_sum_nodes_operation);
	}

	return *this;
}

SCALAR_SUM_NODES_OPERATION::~SCALAR_SUM_NODES_OPERATION()
{
	// Nothing to deallocate
}


double SCALAR_SUM_NODES_OPERATION::performScalarOperation()
{
	return numeric_nodes_list.getNodeValue(0)->getScalarValue() + numeric_nodes_list.getNodeValue(1)->getScalarValue();
}