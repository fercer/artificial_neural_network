#ifndef ACTIVATIONFUNCTIONBASE_CLASS_H_INCLUDED
#define ACTIVATIONFUNCTIONBASE_CLASS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

class ActivationFunctionBase
{
public:
	ActivationFunctionBase()
	{
		function_was_reevaluated = true;
		current_activation_function_time = 0;
	}

	virtual ~ActivationFunctionBase() 
	{
	}
	
	// Evaluates the input value using the sigmoid function
	virtual double evaluateFunction(double input_value, const unsigned long long src_current_network_time) = 0;

	virtual double evaluateDerivative(double input_value) = 0;
	
	virtual void dumpActivationFunctionData(FILE * fp_network_data) = 0;

	void resetActivationFunctionCurrentTime()
	{
		current_activation_function_time = 0;
	}

protected:
	unsigned int parameters_count;
	unsigned long long current_activation_function_time;
	bool function_was_reevaluated;
	double * parameters;
	double current_evaluation;
	double current_evaluation_derivative;
};

#endif //ACTIVATIONFUNCTIONBASE_CLASS_H_INCLUDED