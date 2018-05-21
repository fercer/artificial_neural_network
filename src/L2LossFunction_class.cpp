#include "L2LossFunction_class.h"

L2LossFunction::L2LossFunction()
{
	my_loss_function_type = LF_L2_NORM;
}


L2LossFunction::L2LossFunction(const L2LossFunction & src_loss_function)
{
	this->my_loss_function_type = LF_L2_NORM;

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


L2LossFunction & L2LossFunction::operator=(const L2LossFunction & src_loss_function)
{
	if (this != &src_loss_function)
	{
		this->my_loss_function_type = LF_L2_NORM;

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


L2LossFunction::~L2LossFunction()
{
	/* Nothing to deallocate */
}


double L2LossFunction::computeLoss(const unsigned long long current_time)
{
	if (current_time > error_current_time)
	{
		const double network_output_value = network_output_pointer->getInput(current_time);
		difference = *(*groundtruth_pointer + global_output_index) - network_output_value;
		error = difference * difference / 2.0;
		error_current_time = current_time;
	}

	return error;
}


double L2LossFunction::computeLossWithDerivatives(const unsigned long long current_time)
{
	if (current_time > error_current_time)
	{
		const double network_output_value = network_output_pointer->getInputWithDerivatives(current_time);
		difference = *(*groundtruth_pointer + global_output_index) - network_output_value;
		error_derivative = -difference;
		error = difference * difference / 2.0;
		error_current_time = current_time;
	}

	return error;
}
