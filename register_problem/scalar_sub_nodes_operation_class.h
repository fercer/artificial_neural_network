#ifndef SCALAR_SUB_NODES_OPERATION_CLASS_H_INCLUDED
#define SCALAR_SUB_NODES_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nodes_scalar_operation_class.h"

class SCALAR_SUB_NODES_OPERATION :
	public NODES_SCALAR_OPERATION
{
public:
	SCALAR_SUB_NODES_OPERATION();
	SCALAR_SUB_NODES_OPERATION(const SCALAR_SUB_NODES_OPERATION & src_scalar_sub_nodes_operation);
	SCALAR_SUB_NODES_OPERATION operator = (const SCALAR_SUB_NODES_OPERATION & src_scalar_sub_nodes_operation);
	~SCALAR_SUB_NODES_OPERATION();

protected:
	double performScalarOperation();
};

#endif // SCALAR_SUB_NODES_OPERATION_CLASS_H_INCLUDED
