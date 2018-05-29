#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "activationHyperbolicTangent_class.h"

activationHyperbolicTangent::activationHyperbolicTangent()
{
	my_activation_function_type = ACT_HYPERBOLIC_TANGENT;

	parameters_count = 1;
	parameters = (double*)malloc(sizeof(double));
	*parameters = 1.0;
}

activationHyperbolicTangent::activationHyperbolicTangent(double * src_function_parameters)
{
	my_activation_function_type = ACT_HYPERBOLIC_TANGENT;

	parameters_count = 1;
	parameters = (double*)malloc(sizeof(double));
	if (src_function_parameters)
	{
		*parameters = *(src_function_parameters);
	}
	else
	{
		*parameters = 1.0;
	}
}


activationHyperbolicTangent::activationHyperbolicTangent(const activationHyperbolicTangent &src_activation_function)
{
	this->my_activation_function_type = ACT_HYPERBOLIC_TANGENT;

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
		this->my_activation_function_type = ACT_HYPERBOLIC_TANGENT;

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
		current_input_value = input_value;
		current_evaluation = tanh(*parameters * current_input_value);
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
	fprintf(fp_network_data, "\t\t<ActivationFunction type = \"ACT_HYPERBOLIC_TANGENT\">\n");
	fprintf(fp_network_data, "\t\t\t<Parameter value = \"%.63f\"></Parameter>\n", *parameters);
	fprintf(fp_network_data, "\t\t\t<Input value=\"%.63f\"></Input>", current_input_value);
	fprintf(fp_network_data, "\t\t\t<Output value=\"%.63f\"></Output>", current_evaluation);
	fprintf(fp_network_data, "\t\t\t<Derivative value=\"%.63f\"></Derivative>", current_evaluation_derivative);
	fprintf(fp_network_data, "\t\t</ActivationFunction>\n");
}