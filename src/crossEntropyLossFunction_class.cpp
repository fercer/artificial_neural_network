#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "crossEntropyLossFunction_class.h"

double crossEntropyLossFunction::computeLoss(const unsigned long long current_time)
{
	if (current_time > error_current_time)
	{
		const double network_output_value = network_output_pointer->getInput(current_time);
		difference = -*(groundtruth_pointer) * log(network_output_value) -
			(1 - *(groundtruth_pointer)) * log(1.0 - network_output_value);
		error = difference;

		error_derivative = (1 - *(groundtruth_pointer)) / (1.0 - network_output_value) - *(groundtruth_pointer) / network_output_value;
		error_current_time = current_time;
	}

	return error;
}
