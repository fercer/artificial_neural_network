#include "L2LossFunction_class.h"

double L2LossFunction::computeLoss(const unsigned long long current_time)
{
	if (current_time > error_current_time)
	{
		const double network_output_value = network_output_pointer->getInput(current_time);
		difference = *(groundtruth_pointer)-network_output_value;
		error_derivative = -difference;
		error = difference * difference / 2.0;
		error_current_time = current_time;
	}

	return error;
}
