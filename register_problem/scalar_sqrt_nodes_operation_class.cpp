#include "scalar_sqrt_nodes_operation_class.h"

SCALAR_SQRT_NODES_OPERATION::SCALAR_SQRT_NODES_OPERATION()
{
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
	return sqrt(node_A_value);
}