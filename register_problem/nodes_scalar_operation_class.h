#ifndef NODES_SCALAR_OPERATION_CLASS_H_INCLUDED
#define NODES_SCALAR_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node_scalar_class.h"

class NODES_SCALAR_OPERATION :
	public NODE_SCALAR<double>
{
public:
	NODES_SCALAR_OPERATION()
	{		
		parameters_have_hanged = false;

		node_parameter_A = NULL;
		node_parameter_B = NULL;

		result = 0.0;
	}

	virtual ~NODES_SCALAR_OPERATION()
	{
	}
	
	double getScalarValue() 
	{
		if (parameters_have_hanged)
		{
			result = performScalarOperation();
			parameters_have_hanged = false;
		}
		return result;
	}

protected:
	NODE_SCALAR<double> * node_parameter_A;
	NODE_SCALAR<double> * node_parameter_B;
	bool parameters_have_hanged;

	// Use this function when implementing the copy constructor, and assignation operator (operator =), in derived classes:
	void copyFromNodesScalarOperation(const NODES_SCALAR_OPERATION& src_nodes_scalar_operation)
	{
		copyFromNodeScalar(src_nodes_scalar_operation);

		this->parameters_have_hanged = src_nodes_scalar_operation.parameters_have_hanged;

		this->node_parameter_A = src_nodes_scalar_operation.node_parameter_A;
		this->node_parameter_B = src_nodes_scalar_operation.node_parameter_B;

		this->result = src_nodes_scalar_operation.result;
	}

	// Each derived class must define this function:
	virtual double performScalarOperation() = 0;
	
private:
	double result;
};

#endif // NODES_SCALAR_OPERATION_CLASS_H_INCLUDED
