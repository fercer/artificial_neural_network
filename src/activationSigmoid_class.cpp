#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "activationSigmoid_class.h"

activationSigmoid::activationSigmoid()
{
	parameters_count = 1;
	parameters = (double*)malloc(sizeof(double));
	*parameters = 1.0;
}

activationSigmoid::activationSigmoid(double * src_function_parameters)
{
	parameters_count = 1;
	parameters = (double*)malloc(sizeof(double));
	*parameters = *(src_function_parameters);
}


activationSigmoid::activationSigmoid(const activationSigmoid &src_activation_function)
{
	this->parameters_count = 1;
	this->parameters = (double*)malloc(sizeof(double));
	*(this->parameters) = *(src_activation_function.parameters);
}



activationSigmoid & activationSigmoid::operator= (const activationSigmoid &src_activation_function)
{
	/* This copy assignement operator will copy the parameters used by the source
	The state of the function is not modified
	*/
	if (this != &src_activation_function)
	{
		this->parameters_count = 1;
		this->parameters = (double*)malloc(sizeof(double));
		*(this->parameters) = *(src_activation_function.parameters);
	}
	return *this;
}



activationSigmoid::~activationSigmoid()
{
	free(parameters);
}



// Evaluates the input value using the sigmoid function
double activationSigmoid::evaluateFunction(double input_value, const unsigned long long src_current_network_time)
{
	if (src_current_network_time > current_activation_function_time)
	{
		current_evaluation = 1.0 / (1.0 + exp(-*parameters * input_value));
		current_activation_function_time = src_current_network_time;
		function_was_reevaluated = true;
	}

	return current_evaluation;
}



double activationSigmoid::evaluateDerivative(double input_value)
{
	if (function_was_reevaluated)
	{
		current_evaluation_derivative = *parameters * current_evaluation * (1.0 - current_evaluation);;
		function_was_reevaluated = false;
	}
	return current_evaluation_derivative;
}
