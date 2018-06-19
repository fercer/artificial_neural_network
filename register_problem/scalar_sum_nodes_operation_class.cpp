#include "scalar_sum_nodes_operation_class.h"

SCALAR_SUM_NODES_OPERATION::SCALAR_SUM_NODES_OPERATION()
{
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
	return node_A_value + node_B_value;
}