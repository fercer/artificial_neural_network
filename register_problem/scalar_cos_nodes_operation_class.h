#ifndef SCALAR_COS_NODES_OPERATION_CLASS_H_INCLUDED
#define SCALAR_COS_NODES_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "nodes_scalar_operation_class.h"

class SCALAR_COS_NODES_OPERATION :
	public NODES_SCALAR_OPERATION
{
public:
	SCALAR_COS_NODES_OPERATION();
	SCALAR_COS_NODES_OPERATION(const SCALAR_COS_NODES_OPERATION & src_scalar_cos_nodes_operation);
	SCALAR_COS_NODES_OPERATION operator = (const SCALAR_COS_NODES_OPERATION & src_scalar_cos_nodes_operation);
	~SCALAR_COS_NODES_OPERATION();

protected:
	double performScalarOperation();
};

#endif // SCALAR_COS_NODES_OPERATION_CLASS_H_INCLUDED
