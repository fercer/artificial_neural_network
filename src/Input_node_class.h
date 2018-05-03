#ifndef INPUT_NODE_CLASS_H_INCLUDED
#define INPUT_NODE_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

class Input_node
{
public:
	virtual ~Input_node() 
	{
	}

	// Returns the value assigned to this input
	virtual double getInputWithDerivatives(const unsigned long long current_time) = 0;
	virtual double getInput(const unsigned long long current_time) = 0;

	virtual void addNodeErrorContribution(const double src_error_contribution, const unsigned int src_output_index) = 0;
	virtual void backpropagateNodeError() = 0;

	unsigned int getGlobalNodeIndex()
	{
		return global_node_index;
	}
	
	virtual void dumpNodeData(FILE * fp_network_data) = 0;

protected:
	unsigned int global_node_index;
};

#endif // INPUT_NODE_CLASS_H_INCLUDED