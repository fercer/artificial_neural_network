#ifndef SCALAR_SQRT_NODES_OPERATION_CLASS_H_INCLUDED
#define SCALAR_SQRT_NODES_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "nodes_scalar_operation_class.h"

class SCALAR_SQRT_NODES_OPERATION :
	public NODES_SCALAR_OPERATION
{
public:
	SCALAR_SQRT_NODES_OPERATION();
	SCALAR_SQRT_NODES_OPERATION(const SCALAR_SQRT_NODES_OPERATION & src_scalar_sqrt_nodes_operation);
	SCALAR_SQRT_NODES_OPERATION operator = (const SCALAR_SQRT_NODES_OPERATION & src_scalar_sqrt_nodes_operation);
	~SCALAR_SQRT_NODES_OPERATION();

	void setNodeValue(const double src_parameter_value);
	void setNodeValue(NODE_SCALAR<double>* src_parameter_node);

protected:
	double performScalarOperation();

private:
	NODE_SCALAR<double> value;
};

#endif // SCALAR_SQRT_NODES_OPERATION_CLASS_H_INCLUDED
