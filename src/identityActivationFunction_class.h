#ifndef IDENTITYACTIVATIONFUNCTION_CLASS_H_INCLUDED
#define IDENTITYACTIVATIONFUNCTION_CLASS_H_INCLUDED

#include "ActivationFunctionBase_class.h"

class identityActivationFunction :
	public ActivationFunctionBase
{
public:
	identityActivationFunction();
	identityActivationFunction(const identityActivationFunction & src_activation_function);
	identityActivationFunction & operator= (const identityActivationFunction & src_activation_function);
	identityActivationFunction();
	~identityActivationFunction();

	// Evaluates the activation function in the input value
	double evaluateFunction(double input_value, const unsigned long long src_current_network_time);

	// Evaluates the derivative of the activation function in the input value
	double evaluateDerivative(double input_value);
};

#endif //IDENTITYACTIVATIONFUNCTION_CLASS_H_INCLUDED