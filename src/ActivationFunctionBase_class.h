#ifndef ACTIVATIONFUNCTIONBASE_CLASS_H_INCLUDED
#define ACTIVATIONFUNCTIONBASE_CLASS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

class ActivationFunctionBase
{
public:
	virtual ~ActivationFunctionBase() 
	{
	}

	// Evaluates the input value using the sigmoid function
	virtual double evaluateFunction(double input_value) = 0;
	virtual double evaluateDerivative(double input_value) = 0;
	
	unsigned int getParametersCount()
	{
		return parameters_count;
	}


	virtual double getParameterValue(unsigned int src_parameter_index)
	{
		return *(parameters + src_parameter_index);
	}

protected:
	unsigned int parameters_count;
	double * parameters;
};

#endif //ACTIVATIONFUNCTIONBASE_CLASS_H_INCLUDED