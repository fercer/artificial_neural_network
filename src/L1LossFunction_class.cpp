#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "L1LossFunction_class.h"

// Computes the error using the L1 loss function
double L1LossFunction::computeLoss(const unsigned long long current_time)
{
	if (current_time > error_current_time)
	{
		const double network_output_value = network_output_pointer->getInput(current_time);
		difference = *(groundtruth_pointer)-network_output_value;
		error = fabs(difference);
		error_derivative = -1.0;
		error_current_time = current_time;
	}

	return error;
}