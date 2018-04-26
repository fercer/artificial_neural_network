#include "identityActivationFunction_class.h"

identityActivationFunction::identityActivationFunction()
{
	parameters_count = 0;
	parameters = NULL;
}


identityActivationFunction::~identityActivationFunction()
{
}


double identityActivationFunction::evaluateFunction(double input_value)
{
	return input_value;
}


double identityActivationFunction::evaluateDerivative(double input_value)
{
	return 1.0;
}
