#ifndef INPUT_PATTERN_CLASS_INCLUDED
#define INPUT_PATTERN_CLASS_INCLUDED

#include "Input_node_class.h"

class Input_pattern :
	public Input_node
{
public:
	Input_pattern();
	Input_pattern(const unsigned int src_global_node_index);
	Input_pattern(const Input_pattern & src_input_pattern);
	Input_pattern & operator=(const Input_pattern & src_input_pattern);
	~Input_pattern();


	void setInputPointer(double ** src_input_pointer);
	void setInputPointerPosition(const unsigned int src_global_node_index);

	double getInputWithDerivatives(const unsigned long long current_time);
	double getInput(const unsigned long long current_time);

	void addNodeErrorContribution(const double src_error_contribution, const unsigned int src_output_index);
	void backpropagateNodeError();

	void dumpNodeData(FILE * fp_network_data);

private:
	double ** input_pointer;
};

#endif // INPUT_PATTERN_CLASS_INCLUDED