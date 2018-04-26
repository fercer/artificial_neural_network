#ifndef INPUT_NODE_CLASS_H_INCLUDED
#define INPUT_NODE_CLASS_H_INCLUDED

#include <stdlib.h>

class Input_node
{
public:
	Input_node(const unsigned int src_global_input_index)
	{
		global_input_index = src_global_input_index;
	}

	~Input_node() 
	{
	}

	// Returns the value assigned to this input
	virtual double getInput(const unsigned long long current_time) = 0;
	virtual void addNodeErrorContribution(const double src_error_contribution, const unsigned int src_output_index) = 0;
	virtual void backpropagateNodeError() = 0;

	unsigned int getGlobalInputIndex()
	{
		return global_input_index;
	}

private:
	unsigned int global_input_index;
};

#endif // INPUT_NODE_CLASS_H_INCLUDED