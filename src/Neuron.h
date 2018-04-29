#ifndef NEURON_H_INCLUDED
#define NEURON_H_INCLUDED

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <string.h>

#include "Input_node_class.h"
#include "Weight_node_class.h"

#include "ActivationFunctionBase_class.h"
#include "activationSigmoid_class.h"
#include "activationHyperbolicTangent_class.h"
#include "identityActivationFunction_class.h"

class Neuron :
	public Input_node
{
public:
	typedef enum ACTIVATION_FUNCTION_TYPE {
		ACT_SIGMOID = 0,
		ACT_HYPERBOLIC_TANGENT = 1,
		ACT_IDENTITY = 2
	} ACTIVATION_FUNCTION_TYPE;

	Neuron(const unsigned int src_neuron_position, const unsigned int src_outputs_count = 1, const bool src_compute_derivatives = true, double **** src_weight_values_master_pointer = NULL, double **** src_weight_derivatives_values_master_pointer = NULL);

	Neuron(const Neuron &src_neuron);
	Neuron & operator= (const Neuron &src_neuron);

	~Neuron();
	
	void addWeightedInput(Weight_node::WEIGHT_INPUT_TYPE src_input_type, Input_node * src_input_node_pointer, const double src_weight_value);

	double getInput(const unsigned long long src_current_network_time);

	void addNodeErrorContribution(const double src_error_contribution, const unsigned int src_output_index);

	void backpropagateNodeError();
	void backpropagateNodeError(const unsigned int src_output_index);

	void setActivationFunction(ACTIVATION_FUNCTION_TYPE src_activation_function, double * src_function_parameters);
	
	// Changes the current pointer to the passed as argument:
	void setInputNodePointer(Input_node * src_input_node_pointer, const unsigned int src_input_index);
	Input_node *getInputNodePointer(const unsigned int src_input_index);
	
	void makeExternalWeightValues(double **** src_weight_values_master_pointer = NULL, double **** src_weight_derivatives_values_master_pointer = NULL);

	void makeInternalWeightValues(const bool make_weights_values_internal = true, const bool make_weights_derivatives_values_internal = true);

	unsigned int getInputsCount();
	void resetNodeCurrentTime();

	void dumpNodeData(FILE * fp_network_data);

private:
	typedef struct WEIGHTS_LIST_NODE {
		Weight_node * weighted_input;
		WEIGHTS_LIST_NODE * next_weighted_input;
	} WEIGHTS_LIST_NODE;
	
	Weight_node ** weights_array;
	bool dump_weights_list_into_array_required;

	unsigned long long node_current_time;
	unsigned int neuron_inputs_count;

	bool compute_derivatives;

	double response_to_input;
	double derivative_response_to_input;
	double * neuron_error_contribution;

	unsigned int neuron_outputs_count;

	double **** weight_values_master_pointer;
	double **** weight_derivatives_values_master_pointer;

	WEIGHTS_LIST_NODE * weights_list_tail;
	WEIGHTS_LIST_NODE weights_list_head;

	ACTIVATION_FUNCTION_TYPE neuron_activation_function_type;
	ActivationFunctionBase * neuron_activation_function;

	double (Neuron::*get_input_method)(const unsigned long long src_current_network_time);
	double getInputOnly(const unsigned long long src_current_network_time);
	double getInputWithDerivative(const unsigned long long src_current_network_time);
	
	void dumpWeightsListIntoArray();
};

#endif //NEURON_H_INCLUDED

