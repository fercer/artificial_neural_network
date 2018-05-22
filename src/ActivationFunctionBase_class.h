#ifndef ACTIVATIONFUNCTIONBASE_CLASS_H_INCLUDED
#define ACTIVATIONFUNCTIONBASE_CLASS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

class ActivationFunctionBase
{
public:
	typedef enum ACTIVATION_FUNCTION_TYPE {
		ACT_SIGMOID = 0,
		ACT_HYPERBOLIC_TANGENT = 1,
		ACT_IDENTITY = 2
	} ACTIVATION_FUNCTION_TYPE;

	ActivationFunctionBase()
	{
		function_was_reevaluated = true;
		current_input_value = 0.0;
		current_evaluation = 0.0;
		current_evaluation_derivative = 0.0;
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

	ACTIVATION_FUNCTION_TYPE getActivationFunctionType()
	{
		return my_activation_function_type;
	}

protected:
	unsigned int parameters_count;
	unsigned long long current_activation_function_time;
	bool function_was_reevaluated;
	double * parameters; 

	double current_input_value;
	double current_evaluation;
	double current_evaluation_derivative;

	ACTIVATION_FUNCTION_TYPE my_activation_function_type;
};

#endif //ACTIVATIONFUNCTIONBASE_CLASS_H_INCLUDED