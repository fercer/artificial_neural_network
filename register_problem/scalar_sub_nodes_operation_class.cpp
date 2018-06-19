#include "scalar_sub_nodes_operation_class.h"

SCALAR_SUB_NODES_OPERATION::SCALAR_SUB_NODES_OPERATION()
{
}



SCALAR_SUB_NODES_OPERATION::SCALAR_SUB_NODES_OPERATION(const SCALAR_SUB_NODES_OPERATION & src_scalar_sub_nodes_operation)
{
	copyFromNodesScalarOperation(src_scalar_sub_nodes_operation);
}



SCALAR_SUB_NODES_OPERATION SCALAR_SUB_NODES_OPERATION::operator=(const SCALAR_SUB_NODES_OPERATION & src_scalar_sub_nodes_operation)
{
	if (this != &src_scalar_sub_nodes_operation)
	{
		copyFromNodesScalarOperation(src_scalar_sub_nodes_operation);
	}

	return *this;
}

SCALAR_SUB_NODES_OPERATION::~SCALAR_SUB_NODES_OPERATION()
{
	// Nothing to deallocate
}



double SCALAR_SUB_NODES_OPERATION::performScalarOperation()
{
	return node_A_value - node_B_value;
}