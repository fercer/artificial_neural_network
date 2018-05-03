#include "Input_pattern_class.h"


Input_pattern::Input_pattern()
{
	input_pointer = NULL;
	global_node_index = 0;
}



Input_pattern::Input_pattern(const unsigned int src_global_node_index)
{
	input_pointer = NULL;
	global_node_index = src_global_node_index;
}



Input_pattern::Input_pattern(const Input_pattern & src_input_pattern)
{
	this->input_pointer = src_input_pattern.input_pointer;
	this->global_node_index = src_input_pattern.global_node_index;
}



Input_pattern & Input_pattern::operator= (const Input_pattern & src_input_pattern)
{
	if (this != &src_input_pattern)
	{
		this->input_pointer = src_input_pattern.input_pointer;
		this->global_node_index = src_input_pattern.global_node_index;
	}

	return *this;
}



Input_pattern::~Input_pattern()
{
}



void Input_pattern::setInputPointer(double ** src_input_pointer)
{
	input_pointer = src_input_pointer;
}



void Input_pattern::setInputPointerPosition(const unsigned int src_global_node_index)
{
	global_node_index = src_global_node_index;
}


double Input_pattern::getInputWithDerivatives(const unsigned long long current_time)
{
	return *(*input_pointer + global_node_index);
}


double Input_pattern::getInput(const unsigned long long current_time)
{
	return *(*input_pointer + global_node_index);
}


void Input_pattern::addNodeErrorContribution(const double src_error_contribution, const unsigned int src_output_index)
{
	// This node has no error contribution ...
}


void Input_pattern::backpropagateNodeError() 
{
	// This node cannot backpropagate further ...
}


void Input_pattern::dumpNodeData(FILE * fp_network_data)
{
	fprintf(fp_network_data, "\t<Input position = \"%i\"></Input>\n", global_node_index);
}