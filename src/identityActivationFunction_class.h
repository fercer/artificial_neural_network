#ifndef IDENTITYACTIVATIONFUNCTION_CLASS_H_INCLUDED
#define IDENTITYACTIVATIONFUNCTION_CLASS_H_INCLUDED

#include "ActivationFunctionBase_class.h"

class identityActivationFunction :
	public ActivationFunctionBase
{
public:
	identityActivationFunction();
	~identityActivationFunction();

	// Evaluates the activation function in the input value
	double evaluateFunction(double input_value);

	// Evaluates the derivative of the activation function in the input value
	double evaluateDerivative(double input_value);
};

#endif //IDENTITYACTIVATIONFUNCTION_CLASS_H_INCLUDED