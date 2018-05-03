#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "activationHyperbolicTangent_class.h"

activationHyperbolicTangent::activationHyperbolicTangent()
{
	parameters_count = 1;
	parameters = (double*)malloc(sizeof(double));
	*parameters = 1.0;
}

activationHyperbolicTangent::activationHyperbolicTangent(double * src_function_parameters)
{
	parameters_count = 1;
	parameters = (double*)malloc(sizeof(double));
	*parameters = *(src_function_parameters);
}


activationHyperbolicTangent::activationHyperbolicTangent(const activationHyperbolicTangent &src_activation_function)
{
	this->parameters_count = 1;
	this->parameters = (double*)malloc(sizeof(double));
	*(this->parameters) = *(src_activation_function.parameters);
}



activationHyperbolicTangent & activationHyperbolicTangent::operator= (const activationHyperbolicTangent &src_activation_function)
{
	/* This copy assignement operator will copy the parameters used by the source
	The state of the function is not modified
	*/
	if (this != &src_activation_function)
	{
		parameters_count = 1;
		parameters = (double*)malloc(sizeof(double));
		*(this->parameters) = *(src_activation_function.parameters);
	}
	return *this;
}


activationHyperbolicTangent::~activationHyperbolicTangent()
{
	free(parameters);
}


double activationHyperbolicTangent::evaluateFunction(double input_value, const unsigned long long src_current_network_time)
{
	if (src_current_network_time > current_activation_function_time)
	{
		current_evaluation = tanh(*parameters * input_value);
		current_activation_function_time = src_current_network_time;
		function_was_reevaluated = true;
	}
	return current_evaluation;
}


double activationHyperbolicTangent::evaluateDerivative(double input_value)
{
	if (function_was_reevaluated)
	{
		current_evaluation_derivative = *parameters - current_evaluation*current_evaluation;
		function_was_reevaluated = false;
	}

	return current_evaluation_derivative;
}


void activationHyperbolicTangent::dumpActivationFunctionData(FILE * fp_network_data)
{
	fprintf(fp_network_data, "\t\t<ActivationFunction type = \"ACT_HYPERBOLIC_TANGENT\">");
	fprintf(fp_network_data, "\t\t\t<Parameter value = \"%.63f\"></Parameter>\n", *parameters);
	fprintf(fp_network_data, "\t\t</ActivationFunction>\n");
}