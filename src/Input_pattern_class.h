#ifndef INPUT_PATTERN_CLASS_INCLUDED
#define INPUT_PATTERN_CLASS_INCLUDED

#include "Input_node_class.h"

class Input_pattern :
	public Input_node
{
public:
	~Input_pattern();

	void setInputPointer(double * src_input_pointer);
	void setInputPointerPosition(const unsigned int src_input_pointer_position);
	double getInput(const unsigned long long current_time);
	unsigned int getInputPointerPosition();

	void addNodeErrorContribution(const double src_error_contribution, const unsigned int src_output_index);
	void backpropagateNodeError();

private:
	double * input_pointer;
	unsigned int input_pointer_position;
};

#endif // INPUT_PATTERN_CLASS_INCLUDED