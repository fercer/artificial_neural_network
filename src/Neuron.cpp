#include "Neuron.h"

Neuron::Neuron(const unsigned int src_neuron_position, const unsigned int src_outputs_count, const bool src_compute_derivatives, double **** src_weight_values_master_pointer = NULL, double ***** src_weight_derivatives_values_master_pointer = NULL)
{
	neuron_activation_function = NULL;
	node_current_time = 0;

	neuron_position = src_neuron_position;
	neuron_outputs_count = src_outputs_count;
	compute_derivatives = src_compute_derivatives;
	if (compute_derivatives)
	{
		neuron_error_contribution = (double*)malloc(neuron_outputs_count * sizeof(double));
		get_input_method = &Neuron::getInputWithDerivative;
	}
	else
	{
		neuron_error_contribution = NULL;
		get_input_method = &Neuron::getInputOnly;
	}

	weight_values_master_pointer = src_weight_values_master_pointer;
	weight_derivatives_values_master_pointer = src_weight_derivatives_values_master_pointer;

	/* Allocate the memory for the bias: */
	weights_list_head = new WEIGHTS_LIST_NODE;
	weights_list_head->weighted_input = new Weight_node(
		neuron_outputs_count, /*Pass the number of outputs of the network */
		Weight_node::WIT_BIAS, /* Define the first weight as the bias */
		NULL, /* The bias does not have an input node to be connected */
		0.0, /* The bias is intialized with 0 value */
		neuron_position, /* Pass this neuron position in the network */
		0, /* This is the first weight in the list */
		weight_values_master_pointer, /* Use either, the external weight value, or the self allocated */ 
		weight_derivatives_values_master_pointer/* Use either, the external weight derivatives values, or the self allocated */
	);

	weights_list_head->next_weighted_input = NULL;
	weights_list_tail = weights_list_head;
	inputs_count = 1;
}



Neuron::~Neuron()
{
	if (neuron_activation_function)
	{
		delete neuron_activation_function;
	}

	if (neuron_error_contribution)
	{
		free(neuron_error_contribution);
	}

	/* Free the memory allocated for the weighted inputs */
	WEIGHTS_LIST_NODE * current_weight_node_pointer;
	WEIGHTS_LIST_NODE * next_weight_node_pointer = weights_list_head;
	do
	{
		current_weight_node_pointer = next_weight_node_pointer;
		next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;
		delete current_weight_node_pointer;
	} while (next_weight_node_pointer);
}



void Neuron::addWeightedInput(Weight_node::WEIGHT_INPUT_TYPE src_input_type, Input_node * src_input_node_pointer, const double src_weight_value)
{
	/* Allocate the memory for the newly added weighted input: */
	weights_list_tail->next_weighted_input = new WEIGHTS_LIST_NODE;
	weights_list_tail = weights_list_tail->next_weighted_input;

	weights_list_tail->weighted_input = new Weight_node(
		neuron_outputs_count, /*Pass the number of outputs of the network */
		src_input_type, /* Define the first weight as the bias */
		src_input_node_pointer, /* The bias does not have an input node to be connected */
		src_weight_value, /* The bias is intialized with 0 value */
		neuron_position, /* Pass this neuron position in the network */
		inputs_count, /* This is the first weight in the list */
		weight_values_master_pointer, /* Use either, the external weight value, or the self allocated */
		weight_derivatives_values_master_pointer/* Use either, the external weight derivatives values, or the self allocated */
	);

	weights_list_tail->next_weighted_input = NULL;
	inputs_count++;
}



unsigned int Neuron::getInputsCount()
{
	return inputs_count;
}



double Neuron::getInput(const unsigned long long src_current_network_time)
{
	return (this->*get_input_method)(src_current_network_time);
}



void Neuron::addNodeErrorContribution(const double src_error_contribution, const unsigned int src_output_index)
{
	*(neuron_error_contribution + src_output_index) = *(neuron_error_contribution + src_output_index) + derivative_response_to_input * src_error_contribution;
}



void Neuron::backpropagateNodeError()
{
	WEIGHTS_LIST_NODE * current_weight_node_pointer;
	WEIGHTS_LIST_NODE * next_weight_node_pointer = weights_list_head;

	do
	{
		current_weight_node_pointer = next_weight_node_pointer;
		next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;

		for (unsigned int output_index = 0; output_index < neuron_outputs_count; output_index++)
		{
			current_weight_node_pointer->weighted_input->setWeightsErrorContribution(*(neuron_error_contribution + output_index), output_index);
		}
	} while (next_weight_node_pointer);
}



void Neuron::backpropagateNodeError(const unsigned int src_output_index)
{
	WEIGHTS_LIST_NODE * current_weight_node_pointer;
	WEIGHTS_LIST_NODE * next_weight_node_pointer = weights_list_head;

	do
	{
		current_weight_node_pointer = next_weight_node_pointer;
		next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;

		current_weight_node_pointer->weighted_input->setWeightsErrorContribution(*(neuron_error_contribution + src_output_index), src_output_index);
	} while(next_weight_node_pointer);
}



void Neuron::setActivationFunction(ACTIVATION_FUNCTION_TYPE src_activation_function, double * src_function_parameters)
{
	neuron_activation_function_type = src_activation_function;
	switch (src_activation_function)
	{
	case ACT_HYPERBOLIC_TANGENT:
		neuron_activation_function = new activationHyperbolicTangent(src_function_parameters);
		break;
	case ACT_SIGMOID:
		neuron_activation_function = new activationSigmoid(src_function_parameters);
		break;
	case ACT_IDENTITY:
		neuron_activation_function = new identityActivationFunction;
		break;
	}
}



Neuron::ACTIVATION_FUNCTION_TYPE Neuron::getActivationFunctionType()
{
	return neuron_activation_function_type;
}



ActivationFunctionBase* Neuron::getActivationFunctionPointer()
{
	return neuron_activation_function;
}



Weight_node * Neuron::getWeightedConnection(const unsigned int src_input_index)
{
	WEIGHTS_LIST_NODE * current_weight_node_pointer;
	WEIGHTS_LIST_NODE  * next_weight_node_pointer = weights_list_head;

	unsigned int current_input_index = 0;
	do 
	{
		current_weight_node_pointer = next_weight_node_pointer;
		next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;
		current_input_index++;
	} while ((current_input_index < src_input_index) && next_weight_node_pointer);

	return current_weight_node_pointer->weighted_input;
}



unsigned int Neuron::getNeuronPosition()
{
	return neuron_position;
}



void Neuron::resetNodeCurrentTime()
{
	node_current_time = 0;
	neuron_activation_function->resetActivationFunctionCurrentTime();

	WEIGHTS_LIST_NODE * current_weight_node_pointer;
	WEIGHTS_LIST_NODE * next_weight_node_pointer = weights_list_head;
	do
	{
		current_weight_node_pointer = next_weight_node_pointer;
		next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;

		current_weight_node_pointer->weighted_input->resetWeightCurrentTime();
	} while (next_weight_node_pointer);
}



double Neuron::getInputOnly(const unsigned long long src_current_network_time)
{
	if (src_current_network_time > node_current_time)
	{
		double neuron_weighted_sum = 0.0;
		WEIGHTS_LIST_NODE * current_weight_node_pointer;
		WEIGHTS_LIST_NODE * next_weight_node_pointer;
		do
		{
			current_weight_node_pointer = next_weight_node_pointer;
			next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;

			neuron_weighted_sum += current_weight_node_pointer->weighted_input->getWeightedInput(src_current_network_time);

		} while (next_weight_node_pointer);
		
		response_to_input = neuron_activation_function->evaluateFunction(neuron_weighted_sum);
		node_current_time = src_current_network_time;
	}

	return response_to_input;
}



double Neuron::getInputWithDerivative(const unsigned long long src_current_network_time)
{
	if (src_current_network_time > node_current_time)
	{
		double neuron_weighted_sum = 0.0;
		WEIGHTS_LIST_NODE * current_weight_node_pointer;
		WEIGHTS_LIST_NODE * next_weight_node_pointer;
		do
		{
			current_weight_node_pointer = next_weight_node_pointer;
			next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;

			neuron_weighted_sum += current_weight_node_pointer->weighted_input->getWeightedInput(src_current_network_time);

		} while (next_weight_node_pointer);
		
		response_to_input = neuron_activation_function->evaluateFunction(neuron_weighted_sum);
		derivative_response_to_input = neuron_activation_function->evaluateDerivative(neuron_weighted_sum);

		memset(neuron_error_contribution, 0, neuron_outputs_count * sizeof(double));
		node_current_time = src_current_network_time;
	}

	return response_to_input;
}