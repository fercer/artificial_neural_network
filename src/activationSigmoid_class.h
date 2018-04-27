#ifndef ACTIVATIONSIGMOID_CLASS_H_INCLUDED
#define ACTIVATIONSIGMOID_CLASS_H_INCLUDED

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <math.h>

#include "ActivationFunctionBase_class.h"

class activationSigmoid:
	public ActivationFunctionBase
{
public:
	activationSigmoid();
	activationSigmoid(double * src_function_parameters);
	activationSigmoid(const activationSigmoid &src_activation_function);
	activationSigmoid & operator= (const activationSigmoid &src_activation_function);
	~activationSigmoid();

	// Evaluates the input value using the sigmoid function
	double evaluateFunction(double input_value, const unsigned long long src_current_network_time);
	double evaluateDerivative(double input_value);
};

#endif //ACTIVATIONSIGMOID_CLASS_H_INCLUDED