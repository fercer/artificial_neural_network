#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "crossEntropyLossFunction_class.h"

crossEntropyLossFunction::crossEntropyLossFunction()
{
	my_loss_function_type = LF_CROSS_ENTROPY;
}

crossEntropyLossFunction::crossEntropyLossFunction(const crossEntropyLossFunction & src_loss_function)
{
	this->my_loss_function_type = LF_CROSS_ENTROPY;

	/* This copy operator copies the pointers form the source object,
	the output nodes pointers must be changed later using the "setOutputNode" method.
	The pointer to the groundtruth must be changed as well using the method "setGroundtruth".
	*/

	this->groundtruth_pointer = src_loss_function.groundtruth_pointer;
	this->network_output_pointer = src_loss_function.network_output_pointer;
	this->difference = src_loss_function.difference;
	this->error = src_loss_function.error;
	*(this->derivative_pointer_manager + this->ede_index) = *(src_loss_function.derivative_pointer_manager + src_loss_function.ede_index);
	this->global_output_index = src_loss_function.global_output_index;

	// The time is reseted:
	this->error_current_time = 0;
}



crossEntropyLossFunction & crossEntropyLossFunction::operator=(const crossEntropyLossFunction & src_loss_function)
{
	if (this != &src_loss_function)
	{
		this->my_loss_function_type = LF_CROSS_ENTROPY;
		/* This copy operator copies the pointers form the source object,
		the output nodes pointers must be changed later using the "setOutputNode" method.
		The pointer to the groundtruth must be changed as well using the method "setGroundtruth".
		*/

		this->groundtruth_pointer = src_loss_function.groundtruth_pointer;
		this->network_output_pointer = src_loss_function.network_output_pointer;
		this->difference = src_loss_function.difference;
		this->error = src_loss_function.error;
		*(this->derivative_pointer_manager + this->ede_index) = *(src_loss_function.derivative_pointer_manager + src_loss_function.ede_index);
		this->global_output_index = src_loss_function.global_output_index;

		// The time is reseted:
		this->error_current_time = 0;
	}

	return *this;

}

crossEntropyLossFunction::~crossEntropyLossFunction()
{
	/* Nothing to deallocate */
}


double crossEntropyLossFunction::computeLoss(const unsigned long long current_time)
{
	if (current_time > error_current_time)
	{
		const double network_output_value = network_output_pointer->getInput(current_time);
		difference = -*(*groundtruth_pointer + global_output_index) * log(network_output_value) -
			(1 - *(*groundtruth_pointer + global_output_index)) * log(1.0 - network_output_value);
		error = difference;
		error_current_time = current_time;
	}

	return error;
}


double crossEntropyLossFunction::computeLossWithDerivatives(const unsigned long long current_time)
{
	if (current_time > error_current_time)
	{
		const double network_output_value = network_output_pointer->getInputWithDerivatives(current_time);
		difference = -*(*groundtruth_pointer + global_output_index) * log(network_output_value) -
			(1 - *(*groundtruth_pointer + global_output_index)) * log(1.0 - network_output_value);
		error = difference;

		*(derivative_pointer_manager + ede_index) = (1 - *(*groundtruth_pointer + global_output_index)) / (1.0 - network_output_value) - *(*groundtruth_pointer + global_output_index) / network_output_value;
		error_current_time = current_time;
	}

	return error;
}




void crossEntropyLossFunction::dumpLossFunctionData(FILE * fp_network_data)
{
	fprintf(fp_network_data, "\t<LossFunction type=\"LF_CROSS_ENTROPY\" output_position=\"%i\">\n", global_output_index);
	fprintf(fp_network_data, "\t\t<Output value=\"%.63f\"></Output>\n", error);
	fprintf(fp_network_data, "\t\t<Difference value=\"%.63f\"></Difference>\n", difference);
	fprintf(fp_network_data, "\t\t<Derivative value=\"%.63f\"></Derivative>\n", *(derivative_pointer_manager + ede_index));
	fprintf(fp_network_data, "\t</LossFunction>\n");
}