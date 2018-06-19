#ifndef SCALAR_MULT_NODES_OPERATION_CLASS_H_INCLUDED
#define SCALAR_MULT_NODES_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nodes_scalar_operation_class.h"

class SCALAR_MULT_NODES_OPERATION :
	public NODES_SCALAR_OPERATION
{
public:
	SCALAR_MULT_NODES_OPERATION();
	SCALAR_MULT_NODES_OPERATION(const SCALAR_MULT_NODES_OPERATION & src_scalar_mult_nodes_operation);
	SCALAR_MULT_NODES_OPERATION operator = (const SCALAR_MULT_NODES_OPERATION & src_scalar_mult_nodes_operation);
	~SCALAR_MULT_NODES_OPERATION();

protected:
	double performScalarOperation();
};

#endif // SCALAR_MULT_NODES_OPERATION_CLASS_H_INCLUDED
