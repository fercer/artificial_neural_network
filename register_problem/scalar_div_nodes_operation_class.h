#ifndef SCALAR_DIV_NODES_OPERATION_CLASS_H_INCLUDED
#define SCALAR_DIV_NODES_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nodes_scalar_operation_class.h"

class SCALAR_DIV_NODES_OPERATION :
	public NODES_SCALAR_OPERATION
{
public:
	SCALAR_DIV_NODES_OPERATION();
	SCALAR_DIV_NODES_OPERATION(const SCALAR_DIV_NODES_OPERATION & src_scalar_div_nodes_operation);
	SCALAR_DIV_NODES_OPERATION operator = (const SCALAR_DIV_NODES_OPERATION & src_scalar_div_nodes_operation);
	~SCALAR_DIV_NODES_OPERATION();

	void setNodeAValue(const double src_parameter_A_value);
	void setNodeAValue(NODE_SCALAR<double>* src_parameter_A_node);
	void setNodeBValue(const double src_parameter_B_value);
	void setNodeBValue(NODE_SCALAR<double>* src_parameter_B_node);

protected:
	double performScalarOperation();

private:
	NODE_SCALAR<double> value_A;
	NODE_SCALAR<double> value_B;
};

#endif // SCALAR_DIV_NODES_OPERATION_CLASS_H_INCLUDED
