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


activationHyperbolicTangent::~activationHyperbolicTangent()
{
	free(parameters);
}

double activationHyperbolicTangent::evaluateFunction(double input_value)
{
	return tanh(*parameters * input_value);
}

double activationHyperbolicTangent::evaluateDerivative(double input_value)
{
	/* To increase efficiency of this function, evaluate first the function, and then its derivative */
	const double current_evaluation = tanh(*parameters * input_value);
	return *parameters - current_evaluation*current_evaluation;
}


