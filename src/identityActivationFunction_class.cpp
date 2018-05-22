#include "identityActivationFunction_class.h"

identityActivationFunction::identityActivationFunction()
{
	my_activation_function_type = ACT_IDENTITY;

	parameters_count = 0;
	parameters = NULL;
}

identityActivationFunction::identityActivationFunction(const identityActivationFunction & src_activation_function)
{
	this->my_activation_function_type = ACT_IDENTITY;

	this->parameters_count = 0;
	this->parameters = NULL;
}


identityActivationFunction & identityActivationFunction::operator= (const identityActivationFunction & src_activation_function)
{

	/* This copy assignement operator will copy the parameters used by the source
	The state of the function is not modified
	*/
	if (this != &src_activation_function)
	{
		this->my_activation_function_type = ACT_IDENTITY;

		this->parameters_count = 0;
		this->parameters = NULL;
	}

	return *this;
}


identityActivationFunction::~identityActivationFunction()
{
	/* Nothing to free */
}


double identityActivationFunction::evaluateFunction(double input_value, const unsigned long long src_current_network_time)
{
	current_input_value = input_value;
	return current_input_value;
}


double identityActivationFunction::evaluateDerivative(double input_value)
{
	return 1.0;
}



void identityActivationFunction::dumpActivationFunctionData(FILE * fp_network_data)
{
	fprintf(fp_network_data, "\t\t<ActivationFunction type = \"ACT_IDENTITY\">\n");
	fprintf(fp_network_data, "\t\t\t<Input value=\"%.63f\"></Input>", current_input_value);
	fprintf(fp_network_data, "\t\t\t<Output value=\"%.63f\"></Output>", current_evaluation);
	fprintf(fp_network_data, "\t\t\t<Derivative value=\"%.63f\"></Derivative>", current_evaluation_derivative);
	fprintf(fp_network_data, "\t\t</ActivationFunction>\n");
}