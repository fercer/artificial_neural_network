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
	~activationSigmoid();

	// Evaluates the input value using the sigmoid function
	double evaluateFunction(double input_value);
	double evaluateDerivative(double input_value);
};

#endif //ACTIVATIONSIGMOID_CLASS_H_INCLUDED