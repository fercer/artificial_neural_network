#include "scalar_mult_nodes_operation_class.h"

SCALAR_MULT_NODES_OPERATION::SCALAR_MULT_NODES_OPERATION()
{
	
}



SCALAR_MULT_NODES_OPERATION::SCALAR_MULT_NODES_OPERATION(const SCALAR_MULT_NODES_OPERATION & src_scalar_mult_nodes_operation)
{
	copyFromNodesScalarOperation(src_scalar_mult_nodes_operation);
}



SCALAR_MULT_NODES_OPERATION SCALAR_MULT_NODES_OPERATION::operator=(const SCALAR_MULT_NODES_OPERATION & src_scalar_mult_nodes_operation)
{
	if (this != &src_scalar_mult_nodes_operation)
	{
		copyFromNodesScalarOperation(src_scalar_mult_nodes_operation);
	}

	return *this;
}

SCALAR_MULT_NODES_OPERATION::~SCALAR_MULT_NODES_OPERATION()
{
	// Nothing to deallocate
}



double SCALAR_MULT_NODES_OPERATION::performScalarOperation()
{
	return node_parameter_A->getScalarValue() * node_parameter_B->getScalarValue();
}