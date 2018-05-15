#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "L1LossFunction_class.h"


L1LossFunction::L1LossFunction()
{
	my_loss_function_type = LF_L1_NORM;
}


L1LossFunction:: L1LossFunction(const L1LossFunction & src_loss_function)
{
	this->my_loss_function_type = LF_L1_NORM;

	/* This copy contructor copies the pointers form the source object,
	the output nodes pointers must be changed later using the "setOutputNode" method.
	The pointer to the groundtruth must be changed as well using the method "setGroundtruth".
	*/

	this->groundtruth_pointer = src_loss_function.groundtruth_pointer;
	this->network_output_pointer = src_loss_function.network_output_pointer;
	this->difference = src_loss_function.difference;
	this->error = src_loss_function.error;
	this->error_derivative = src_loss_function.error_derivative;
	this->global_output_index = src_loss_function.global_output_index;

	// The time is reseted:
	this->error_current_time = 0;
}



L1LossFunction & L1LossFunction::operator= (const L1LossFunction & src_loss_function)
{
	if (this != &src_loss_function)
	{
		this->my_loss_function_type = LF_L1_NORM;
		/* This copy operator copies the pointers form the source object,
		the output nodes pointers must be changed later using the "setOutputNode" method.
		The pointer to the groundtruth must be changed as well using the method "setGroundtruth".
		*/

		this->groundtruth_pointer = src_loss_function.groundtruth_pointer;
		this->network_output_pointer = src_loss_function.network_output_pointer;
		this->difference = src_loss_function.difference;
		this->error = src_loss_function.error;
		this->error_derivative = src_loss_function.error_derivative;
		this->global_output_index = src_loss_function.global_output_index;

		// The time is reseted:
		this->error_current_time = 0;
	}

	return *this;
}


L1LossFunction::~L1LossFunction()
{
	/* Nothing to deallocate */
}

// Computes the error using the L1 loss function
double L1LossFunction::computeLoss(const unsigned long long current_time)
{
	if (current_time > error_current_time)
	{
		const double network_output_value = network_output_pointer->getInput(current_time);
		difference = *(*groundtruth_pointer + global_output_index)-network_output_value;
		error = fabs(difference);
		error_current_time = current_time;
	}

	return error;
}

// Computes the error using the L1 loss function
double L1LossFunction::computeLossWithDerivatives(const unsigned long long current_time)
{
	if (current_time > error_current_time)
	{
		const double network_output_value = network_output_pointer->getInputWithDerivatives(current_time);
		difference = *(*groundtruth_pointer + global_output_index) - network_output_value;
		error = fabs(difference);
		error_derivative = -1.0;
		error_current_time = current_time;
	}

	return error;
}