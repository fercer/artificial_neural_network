#ifndef INPUT_NODE_CLASS_H_INCLUDED
#define INPUT_NODE_CLASS_H_INCLUDED

#include <stdlib.h>

class Input_node
{
public:
	~Input_node() 
	{
	}

	// Returns the value assigned to this input
	virtual double getInput(const unsigned long long current_time) = 0;
	virtual void addNodeErrorContribution(const double src_error_contribution, const unsigned int src_output_index) = 0;
	virtual void backpropagateNodeError() = 0;
};

#endif // INPUT_NODE_CLASS_H_INCLUDED