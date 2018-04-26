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

	Weight_node(const unsigned int src_outputs_count, const unsigned int src_neuron_index = 0, double **** src_weight_values_master_pointer = NULL, double ***** src_weight_derivatives_values_master_pointer = NULL);
	~Weight_node();

	// Adds an input node to this weights manager
	void addInputNode(Input_node * src_input_node_pointer, const double src_weight_value, const WEIGHT_INPUT_TYPE src_input_type);

	unsigned int getInputsCount();

	// Computes the weighted sum of the inputs and their corresponding weights
	double getWeightedSum(const unsigned long long current_time);

	// Set the bias/weight value
	void setWeightValue(const unsigned int src_input_index, const double src_weight_value);

	// Get the bias/weight value
	double getWeightValue(const unsigned int src_input_index);

	void setWeightsErrorContribution(const double src_node_error_contribution, const unsigned int src_output_index);

private:
	typedef struct WEIGHT_LIST_NODE {
		WEIGHT_INPUT_TYPE weight_node_type;
		unsigned int input_index;
		Input_node * input_node;
		WEIGHT_LIST_NODE * next_weight_node;
	} WEIGHT_LIST_NODE;

	WEIGHT_LIST_NODE * weights_list_head;
	WEIGHT_LIST_NODE * weights_list_tail;

	void(Weight_node::*assign_weight_value_method) (const double src_weigth_value);

	unsigned int neuron_index;

	double *** self_weight_values_pointer;
	double **** self_weight_derivatives_values_pointer;

	double ** self_weight_values;
	double *** self_weight_derivatives_values;

	double **** weight_values_master_pointer;
	double ***** weight_derivatives_values_master_pointer;

	unsigned int inputs_count;
	unsigned int outputs_count;

	unsigned long long current_weight_time;

	double getWeightedSumNode(WEIGHT_LIST_NODE * src_current_weight_node, const unsigned long long current_time);

	void setWeightsErrorContributionNode(WEIGHT_LIST_NODE * src_current_weight_node, const double src_node_error_contribution, const unsigned int src_output_index);

	void assignWeightValueSelf(const double src_weigth_value);
	void assignWeightValueExternal(const double src_weigth_value);
};

#endif // WEIGHT_NODE_CLASS_INLCUDED