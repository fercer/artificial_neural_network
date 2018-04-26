#ifndef WEIGHT_NODE_CLASS_INLCUDED
#define WEIGHT_NODE_CLASS_INLCUDED

#include <stdlib.h>
#include <string.h>
#include "Input_node_class.h"

class Weight_node
{
public:
	typedef enum WEIGHT_INPUT_TYPE{
		WIT_BIAS = 0,
		WIT_PATTERN = 1,
		WIT_NEURON = 2
	}WEIGHT_INPUT_TYPE;

	Weight_node();
	~Weight_node();

	// Adds an input node to this weights manager
	void addInputNode(Input_node * src_input_node_pointer, const double src_weight_value, const WEIGHT_INPUT_TYPE src_input_type, const unsigned int src_input_position);
	void assignExternalWeightPointer(double * src_weight_pointer);
	void assignExternalWeightErrorContributionPointer(double ** src_weight_error_contribution_pointer);

	unsigned int getInputsCount();

	double getWeightValue(const unsigned int src_input_index);

	// Computes the weighted sum of the inputs and their corresponding weights
	double getWeightedSum(const unsigned long long current_time);
	
	// Returns the error contribution of the selected weight:
	double getWeightErrorContribution(const unsigned int src_input_index, const unsigned int src_output_index);

	// Updates the weight values using the new values passed
	void updateWeightValue(const double src_updated_weight_value, const unsigned int src_input_index);

	void setWeightsErrorContribution(const double src_node_error_contribution, const unsigned int src_output_index);

	unsigned int getInputPosition(const unsigned int src_input_index);
	WEIGHT_INPUT_TYPE getInputType(const unsigned int src_input_index);

protected:
	double weighted_sum;
	void assignOutputsCount(const unsigned int src_outputs_count);

private:
	void (Weight_node::*input_node_method) (Input_node * src_input_node_pointer, const double src_weight_value, const WEIGHT_INPUT_TYPE src_input_type, const unsigned int src_input_position);

	unsigned long long current_weight_time;
	WEIGHT_INPUT_TYPE * input_types;
	unsigned int * input_positions;

	double ** weight_error_contribution;
	Input_node ** input_nodes;

	unsigned int inputs_count;
	unsigned int outputs_count;

	double * weight_values;

	bool external_weights_source;
	bool external_weights_error_contribution_source;

	// Adds an input node to this weights manager
	void addInputNode_self_weight_self_error_contribution(Input_node * src_input_node_pointer, const double src_weight_value, const WEIGHT_INPUT_TYPE src_input_type, const unsigned int src_input_position);
	void addInputNode_external_weight_self_error_contribution(Input_node * src_input_node_pointer, const double src_weight_value, const WEIGHT_INPUT_TYPE src_input_type, const unsigned int src_input_position);
	void addInputNode_self_weight_external_error_contribution(Input_node * src_input_node_pointer, const double src_weight_value, const WEIGHT_INPUT_TYPE src_input_type, const unsigned int src_input_position);
	void addInputNode_external_weight_external_error_contribution(Input_node * src_input_node_pointer, const double src_weight_value, const WEIGHT_INPUT_TYPE src_input_type, const unsigned int src_input_position);
};

#endif // WEIGHT_NODE_CLASS_INLCUDED