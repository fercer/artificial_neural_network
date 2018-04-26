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


activationSigmoid::~activationSigmoid()
{
	free(parameters);
}


// Evaluates the input value using the sigmoid function
double activationSigmoid::evaluateFunction(double input_value)
{
	return  1.0 / (1.0 + exp(-*parameters * input_value));
}


double activationSigmoid::evaluateDerivative(double input_value)
{
	/* To increase efficiency of this function, evaluate first the function, and then its derivative */
	const double evaluation = 1.0 / (1.0 + exp(-*parameters * input_value));
	return  *parameters * evaluation * (1.0 - evaluation);
}
