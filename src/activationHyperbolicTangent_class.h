#ifndef ACTIVATIONHYPERBOLICTANGENT_CLASS_H_INCLUDED
#define ACTIVATIONHYPERBOLICTANGENT_CLASS_H_INCLUDED

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <math.h>
#include "ActivationFunctionBase_class.h"

class activationHyperbolicTangent:
	public ActivationFunctionBase
{
public:
	activationHyperbolicTangent();
	activationHyperbolicTangent(double * src_function_parameters);
	activationHyperbolicTangent(const activationHyperbolicTangent &src_activation_function);
	activationHyperbolicTangent & operator= (const activationHyperbolicTangent &src_activation_function);

	~activationHyperbolicTangent();

	// Evaluates the activation function in the input value
	double evaluateFunction(double input_value, const unsigned long long src_current_network_time);

	// Evaluates the derivative of the activation function in the input value
	double evaluateDerivative(double input_value);

	void dumpActivationFunctionData(FILE * fp_network_data);
};

#endif //ACTIVATIONHYPERBOLICTANGENT_CLASS_H_INCLUDED
