#include "Neuron.h"

Neuron::Neuron(const unsigned int src_neuron_position, const unsigned int src_outputs_count, double **** src_weight_values_master_pointer = NULL, double ***** src_weight_derivatives_values_master_pointer = NULL)
{
	neuron_activation_function = NULL;
	node_current_time = 0;
	
	neuron_position = src_neuron_position;
	neuron_outputs_count = src_outputs_count;
	neuron_error_contribution = (double*) malloc(neuron_outputs_count * sizeof(double));
	
	neuron_weights = new Weight_node(neuron_outputs_count, neuron_position, src_weight_values_master_pointer, src_weight_derivatives_values_master_pointer);
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

	delete neuron_weights;
}



double Neuron::getInput(const unsigned long long current_time)
{
	if (current_time > node_current_time)
	{
		const double neuron_weighted_sum = neuron_weights->getWeightedSum(current_time);
		response_to_input = neuron_activation_function->evaluateFunction(neuron_weighted_sum);
		derivative_response_to_input = neuron_activation_function->evaluateDerivative(neuron_weighted_sum);

		memset(neuron_error_contribution, 0, neuron_outputs_count * sizeof(double));
		node_current_time = current_time;
	}

	return response_to_input;
}



void Neuron::addNodeErrorContribution(const double src_error_contribution, const unsigned int src_output_index)
{
	*(neuron_error_contribution + src_output_index) = *(neuron_error_contribution + src_output_index) + derivative_response_to_input * src_error_contribution;
}



void Neuron::backpropagateNodeError()
{
	for (unsigned int output_index = 0; output_index < neuron_outputs_count; output_index++)
	{
		neuron_weights->setWeightsErrorContribution(*(neuron_error_contribution + output_index), output_index);
	}
}



void Neuron::backpropagateNodeError(const unsigned int src_output_index)
{
	neuron_weights->setWeightsErrorContribution(*(neuron_error_contribution + src_output_index), src_output_index);
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



unsigned int Neuron::getActivationFunctionParametersCount()
{
	return neuron_activation_function->getParametersCount();
}



double Neuron::getActivationFunctionParameterValue(const unsigned int src_parameter_index)
{
	return neuron_activation_function->getParameterValue(src_parameter_index);
}



unsigned int Neuron::getNeuronPosition()
{
	return neuron_position;
}



void Neuron::resetNodeCurrentTime()
{
	node_current_time = 0;
}

