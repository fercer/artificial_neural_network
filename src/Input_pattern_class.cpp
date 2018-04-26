#include "Input_pattern_class.h"


Input_pattern::Input_pattern()
{
	input_pointer = NULL;
	input_pointer_position = 0;
}


Input_pattern::~Input_pattern()
{
}


void Input_pattern::setInputPointer(double * src_input_pointer)
{
	input_pointer = src_input_pointer;
}

void Input_pattern::setInputPointerPosition(const unsigned int src_input_pointer_position)
{
	input_pointer_position = src_input_pointer_position;
}


double Input_pattern::getInput(const unsigned long long current_time)
{
	return *(input_pointer + input_pointer_position);
}

unsigned int Input_pattern::getInputPointerPosition()
{
	return input_pointer_position;
}

void Input_pattern::addNodeErrorContribution(const double src_error_contribution, const unsigned int src_output_index)
{
	// This node has no error contribution ...
}


void Input_pattern::backpropagateNodeError() 
{
	// This node cannot backpropagate further ...
}