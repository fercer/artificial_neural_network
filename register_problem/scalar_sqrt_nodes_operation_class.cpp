#include "scalar_sqrt_nodes_operation_class.h"

SCALAR_SQRT_NODES_OPERATION::SCALAR_SQRT_NODES_OPERATION()
{
	value.setScalarValue(1.0);
	node_parameter_A = &value;
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

void SCALAR_SQRT_NODES_OPERATION::setNodeValue(const double src_parameter_value)
{
	value.setScalarValue(src_parameter_value);
	parameters_have_hanged = true;
}

void SCALAR_SQRT_NODES_OPERATION::setNodeValue(NODE_SCALAR<double>* src_parameter_node)
{
	if (src_parameter_node)
	{
		node_parameter_A = src_parameter_node;
		parameters_have_hanged = true;
	}
	else
	{
		value.setScalarValue(node_parameter_A->getScalarValue());
		node_parameter_A = &value;
	}
}



double SCALAR_SQRT_NODES_OPERATION::performScalarOperation()
{
	return sqrt(node_parameter_A->getScalarValue());
}