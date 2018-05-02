#ifndef WEIGHT_NODE_CLASS_INLCUDED
#define WEIGHT_NODE_CLASS_INLCUDED

#include <stdio.h>
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

	Weight_node(const unsigned int src_outputs_count, const WEIGHT_INPUT_TYPE src_input_type, Input_node * src_input_node, const double src_weight_value, const unsigned int src_neuron_index = 0, const unsigned int src_input_index = 0, double **** src_weight_values_master_pointer = NULL, double **** src_weight_derivatives_values_master_pointer = NULL);

	Weight_node(const Weight_node & src_weight_node);
	Weight_node & operator= (const Weight_node & src_weight_node);

	~Weight_node();
	
	// Sets/Gets the current input node pointer:
	void setInputNodePointer(Input_node * src_input_node_pointer);
	Input_node * getInputNodePointer();

	// Computes the weighted sum of the inputs and their corresponding weights
	double getWeightedInput(const unsigned long long src_current_network_time);
	double getWeightErrorContribution(const unsigned int src_output_index, const unsigned long long src_current_network_time);

	// Set/Get the weight value
	void setWeightValue(const double src_weight_value);
	double getWeightValue();

	// Assign the error reflected into the specified output corresponding to all the weights in the list.
	void setWeightsErrorContribution(const double src_node_error_contribution, const unsigned int src_output_index);

	void makeExternalWeigthValue(double **** src_weight_values_master_pointer = NULL, double **** src_weight_derivatives_values_master_pointer = NULL);

	void makeInternalWeightValue(const bool make_weight_values_internal = true, const bool make_weight_derivatives_values_internal = true);

	void resetWeightCurrentTime();

	void dumpWeightData(FILE * fp_network_data);

private:
	
	/* These arrays contain the actual weight, and derivatives values */
	double * self_weight_value;
	double * self_weight_derivatives_values;

	/* These pointers represent the neuron's weights, and derivative values 
	If the memory is self allocated, these pointers point directly to the arrays 'self_weight_value' and 'self_weight_derivatives_values' respectively,
	and should be accessed exclusively in their position [0]:
	*/
	double ** self_neuron_weight_values;
	double ** self_neuron_weight_derivatives_values;

	/* These pointers represent the network's neurons
	If the memory is self allocated, these pointers point directly to the pointers 'self_neuron_weight_values' and 'self_neuron_weight_derivatives_values' respectively,
	and should be accessed exclusively in their position [0]:
	*/
	double *** self_network_weight_values;
	double *** self_network_weight_derivatives_values;

	/* These pointers contain the weights and derivatives values of the entire network,
	If the memory is self allocated, these pointers point directly to the pointers 'self_network_weight_values' and 'self_network_weight_derivatives_values' respectively,
	and should be accessed exclusively in their position [0]:
	*/
	double **** weight_values_master_pointer;
	double **** weight_derivatives_values_master_pointer;

	unsigned int neuron_index;
	unsigned int input_index;

	/* [e]xternal [w]eight [v]alue neuron and input indices: */
	unsigned int ewv_neuron_index;
	unsigned int ewv_input_index;

	/* [e]xternal [w]eight [d]erivatives [v]alues neuron and input indices: */
	unsigned int ewdv_neuron_index;
	unsigned int ewdv_input_index;

	unsigned int outputs_count;
	WEIGHT_INPUT_TYPE input_type;
	Input_node * input_node_pointer;
	unsigned long long current_weight_time;


	double(Weight_node::*get_input_response) ();
	double getWeightedInput();
	double getBias();

	void(Weight_node::*set_weight_error_cotribution) (const double src_node_error_contribution, const unsigned int src_output_index);
	void setWeightErrorContribution(const double src_node_error_contribution, const unsigned int src_output_index);
	void setBiasErrorContribution(const double src_node_error_contribution, const unsigned int src_output_index);

	void(Weight_node::*dump_weight_method) (FILE * fp_network_data);
	void dumpWeightDataBias(FILE * fp_network_data);
	void dumpWeightDataNeuron(FILE * fp_network_data);
	void dumpWeightDataPattern(FILE * fp_network_data);
};

#endif // WEIGHT_NODE_CLASS_INLCUDED