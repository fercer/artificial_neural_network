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

	Neuron(const unsigned int src_neuron_position, const unsigned int src_outputs_count = 1, double **** src_weight_values_master_pointer = NULL, double ***** src_weight_derivatives_values_master_pointer = NULL);
	~Neuron();

	double getInput(const unsigned long long current_time);

	void addNodeErrorContribution(const double src_error_contribution, const unsigned int src_output_index);
	void backpropagateNodeError();
	void backpropagateNodeError(const unsigned int src_output_index);

	void setActivationFunction(ACTIVATION_FUNCTION_TYPE src_activation_function, double * src_function_parameters);

	ACTIVATION_FUNCTION_TYPE getActivationFunctionType();
	unsigned int getActivationFunctionParametersCount();
	double getActivationFunctionParameterValue(const unsigned int src_parameter_index);

	unsigned int getNeuronPosition();

	void resetNodeCurrentTime();

private:
	Weight_node * neuron_weights;

	unsigned int neuron_position;
	unsigned int node_current_time;

	double response_to_input;
	double derivative_response_to_input;
	double * neuron_error_contribution;

	unsigned long long neuron_outputs_count;

	ACTIVATION_FUNCTION_TYPE neuron_activation_function_type;
	ActivationFunctionBase * neuron_activation_function;
};

#endif //NEURON_H_INCLUDED

