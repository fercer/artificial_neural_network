#include "scalar_div_nodes_operation_class.h"

SCALAR_DIV_NODES_OPERATION::SCALAR_DIV_NODES_OPERATION()
{
	value_A.setScalarValue(1.0);
	value_B.setScalarValue(1.0);

	node_parameter_A = &value_A;
	node_parameter_B = &value_B;
}



SCALAR_DIV_NODES_OPERATION::SCALAR_DIV_NODES_OPERATION(const SCALAR_DIV_NODES_OPERATION & src_scalar_div_nodes_operation)
{
	copyFromNodesScalarOperation(src_scalar_div_nodes_operation);
}



SCALAR_DIV_NODES_OPERATION SCALAR_DIV_NODES_OPERATION::operator=(const SCALAR_DIV_NODES_OPERATION & src_scalar_div_nodes_operation)
{
	if (this != &src_scalar_div_nodes_operation)
	{
		copyFromNodesScalarOperation(src_scalar_div_nodes_operation);
	}

	return *this;
}

SCALAR_DIV_NODES_OPERATION::~SCALAR_DIV_NODES_OPERATION()
{
	// Nothing to deallocate
}


void SCALAR_DIV_NODES_OPERATION::setNodeAValue(const double src_parameter_A_value)
{
	value_A.setScalarValue(src_parameter_A_value);
}


void SCALAR_DIV_NODES_OPERATION::setNodeAValue(NODE_SCALAR<double> * src_parameter_A_node)
{
	if (src_parameter_A_node)
	{
		node_parameter_A = src_parameter_A_node;
	}
	else
	{
		node_parameter_A = &value_A;
	}
}


void SCALAR_DIV_NODES_OPERATION::setNodeBValue(const double src_parameter_B_value)
{
	value_B.setScalarValue(src_parameter_B_value);
}


void SCALAR_DIV_NODES_OPERATION::setNodeBValue(NODE_SCALAR<double> * src_parameter_B_node)
{
	if (src_parameter_B_node)
	{
		node_parameter_B = src_parameter_B_node;
	}
	else
	{
		node_parameter_B = &value_B;
	}
}



double SCALAR_DIV_NODES_OPERATION::performScalarOperation()
{
	return node_parameter_A->getScalarValue() / node_parameter_B->getScalarValue();
}