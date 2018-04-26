#include "Weight_node_class.h"


Weight_node::Weight_node()
{
	input_nodes = NULL;
	
	weight_values = (double*)malloc(sizeof(double));
	weight_error_contribution = (double**)malloc(sizeof(double*));
	input_types = (WEIGHT_INPUT_TYPE*)malloc(sizeof(WEIGHT_INPUT_TYPE));
	*input_types = WIT_BIAS;

	input_positions = (unsigned int *)malloc(sizeof(unsigned int));
	*input_positions = 0;

	external_weights_source = false;
	external_weights_error_contribution_source = false;

	current_weight_time = 0;
	weighted_sum = 0.0;
	inputs_count = 0;
	outputs_count = 0;

	input_node_method = &Weight_node::addInputNode_self_weight_self_error_contribution;
}


Weight_node::~Weight_node()
{
	if (inputs_count > 0)
	{
		free(input_nodes);
	}
	
	if (!external_weights_source)
	{
		free(weight_values);
	}

	if (!external_weights_error_contribution_source)
	{
		for (unsigned int input_index = 0; input_index <= inputs_count; input_index++)
		{
			free(*(weight_error_contribution + input_index));
		}
		free(weight_error_contribution);
	}

	free(input_types);
	free(input_positions);
}



void Weight_node::addInputNode(Input_node * src_input_node_pointer, const double src_weight_value, const WEIGHT_INPUT_TYPE src_input_type, const unsigned int src_input_position)
{
	(this->*input_node_method)(src_input_node_pointer, src_weight_value, src_input_type, src_input_position);
}



// Adds an input node to this weights manager
void Weight_node::addInputNode_self_weight_self_error_contribution(Input_node * src_input_node_pointer, const double src_weight_value, const WEIGHT_INPUT_TYPE src_input_type, const unsigned int src_input_position)
{
	if (inputs_count > 0)
	{
		Input_node ** swap_input_nodes = input_nodes;
		input_nodes = (Input_node**)malloc((inputs_count + 1) * sizeof(Input_node*));
		memmove(input_nodes, swap_input_nodes, inputs_count * sizeof(Input_node*));
		free(swap_input_nodes);

		double * swap_weight_values = weight_values;
		weight_values = (double*)malloc((inputs_count + 2) * sizeof(double));
		memmove(weight_values, swap_weight_values, (inputs_count + 1) * sizeof(double));
		free(swap_weight_values);

		double ** swap_weight_error_contribution = weight_error_contribution;
		weight_error_contribution = (double**)malloc((inputs_count + 2) * sizeof(double*));
		memmove(weight_error_contribution, swap_weight_error_contribution, (inputs_count + 1) * sizeof(double*));
		free(swap_weight_error_contribution);

		WEIGHT_INPUT_TYPE * swap_input_types = input_types;
		input_types = (WEIGHT_INPUT_TYPE*)malloc((inputs_count + 2) * sizeof(WEIGHT_INPUT_TYPE));
		memmove(input_types, swap_input_types, (inputs_count + 1) * sizeof(WEIGHT_INPUT_TYPE));
		free(swap_input_types);
		
		unsigned int * swap_input_positions = input_positions;
		input_positions = (unsigned int *)malloc((inputs_count + 2) * sizeof(unsigned int));
		memmove(input_positions, swap_input_positions, (inputs_count + 1) * sizeof(unsigned int));
		free(swap_input_positions);
	}
	else
	{
		input_nodes = (Input_node**)malloc(sizeof(Input_node*));

		double * swap_weight_values = weight_values;
		weight_values = (double*)malloc(2 * sizeof(double));
		memmove(weight_values, swap_weight_values, sizeof(double));
		free(swap_weight_values);

		double ** swap_weight_error_contribution = weight_error_contribution;
		weight_error_contribution = (double**)malloc(2 * sizeof(double*));
		memmove(weight_error_contribution, swap_weight_error_contribution, sizeof(double*));
		free(swap_weight_error_contribution);

		WEIGHT_INPUT_TYPE * swap_input_types = input_types;
		input_types = (WEIGHT_INPUT_TYPE*)malloc(2 * sizeof(WEIGHT_INPUT_TYPE));
		memmove(input_types, swap_input_types, sizeof(WEIGHT_INPUT_TYPE));
		free(swap_input_types);

		unsigned int * swap_input_positions = input_positions;
		input_positions = (unsigned int *)malloc(2 * sizeof(unsigned int));
		memmove(input_positions, swap_input_positions, sizeof(unsigned int));
		free(swap_input_positions);
	}

	*(input_nodes + inputs_count) = src_input_node_pointer;
	*(weight_values + inputs_count + 1) = src_weight_value;
	*(weight_error_contribution + inputs_count + 1) = (double*)malloc(outputs_count * sizeof(double));
	*(input_types + inputs_count + 1) = src_input_type;
	*(input_positions + inputs_count + 1) = src_input_position;

	inputs_count++;
}



void Weight_node::addInputNode_external_weight_self_error_contribution(Input_node * src_input_node_pointer, const double src_weight_value, const WEIGHT_INPUT_TYPE src_input_type, const unsigned int src_input_position)
{
	if (inputs_count > 0)
	{
		Input_node ** swap_input_nodes = input_nodes;
		input_nodes = (Input_node**)malloc((inputs_count + 1) * sizeof(Input_node*));
		memmove(input_nodes, swap_input_nodes, inputs_count * sizeof(Input_node*));
		free(swap_input_nodes);

		double ** swap_weight_error_contribution = weight_error_contribution;
		weight_error_contribution = (double**)malloc((inputs_count + 2) * sizeof(double*));
		memmove(weight_error_contribution, swap_weight_error_contribution, (inputs_count + 1) * sizeof(double*));
		free(swap_weight_error_contribution);

		WEIGHT_INPUT_TYPE * swap_input_types = input_types;
		input_types = (WEIGHT_INPUT_TYPE*)malloc((inputs_count + 2) * sizeof(WEIGHT_INPUT_TYPE));
		memmove(input_types, swap_input_types, (inputs_count + 1) * sizeof(WEIGHT_INPUT_TYPE));
		free(swap_input_types);

		unsigned int * swap_input_positions = input_positions;
		input_positions = (unsigned int *)malloc((inputs_count + 2) * sizeof(unsigned int));
		memmove(input_positions, swap_input_positions, (inputs_count + 1) * sizeof(unsigned int));
		free(swap_input_positions);
	}
	else
	{
		input_nodes = (Input_node**)malloc(sizeof(Input_node*));

		double ** swap_weight_error_contribution = weight_error_contribution;
		weight_error_contribution = (double**)malloc(2 * sizeof(double*));
		memmove(weight_error_contribution, swap_weight_error_contribution, sizeof(double*));
		free(swap_weight_error_contribution);

		WEIGHT_INPUT_TYPE * swap_input_types = input_types;
		input_types = (WEIGHT_INPUT_TYPE*)malloc(2 * sizeof(WEIGHT_INPUT_TYPE));
		memmove(input_types, swap_input_types, sizeof(WEIGHT_INPUT_TYPE));
		free(swap_input_types);

		unsigned int * swap_input_positions = input_positions;
		input_positions = (unsigned int *)malloc(2 * sizeof(unsigned int));
		memmove(input_positions, swap_input_positions, sizeof(unsigned int));
		free(swap_input_positions);
	}

	*(input_nodes + inputs_count) = src_input_node_pointer;

	// Weights is determined from the external array value:
	//*(weight_values + inputs_count + 1) = src_weight_value;
	*(weight_error_contribution + inputs_count + 1) = (double*)malloc(outputs_count * sizeof(double));
	*(input_types + inputs_count + 1) = src_input_type;
	*(input_positions + inputs_count + 1) = src_input_position;

	inputs_count++;
}



void Weight_node::addInputNode_self_weight_external_error_contribution(Input_node * src_input_node_pointer, const double src_weight_value, const WEIGHT_INPUT_TYPE src_input_type, const unsigned int src_input_position)
{
	if (inputs_count > 0)
	{
		Input_node ** swap_input_nodes = input_nodes;
		input_nodes = (Input_node**)malloc((inputs_count + 1) * sizeof(Input_node*));
		memmove(input_nodes, swap_input_nodes, inputs_count * sizeof(Input_node*));
		free(swap_input_nodes);

		double * swap_weight_values = weight_values;
		weight_values = (double*)malloc((inputs_count + 2) * sizeof(double));
		memmove(weight_values, swap_weight_values, (inputs_count + 1) * sizeof(double));
		free(swap_weight_values);

		WEIGHT_INPUT_TYPE * swap_input_types = input_types;
		input_types = (WEIGHT_INPUT_TYPE*)malloc((inputs_count + 2) * sizeof(WEIGHT_INPUT_TYPE));
		memmove(input_types, swap_input_types, (inputs_count + 1) * sizeof(WEIGHT_INPUT_TYPE));
		free(swap_input_types);

		unsigned int * swap_input_positions = input_positions;
		input_positions = (unsigned int *)malloc((inputs_count + 2) * sizeof(unsigned int));
		memmove(input_positions, swap_input_positions, (inputs_count + 1) * sizeof(unsigned int));
		free(swap_input_positions);
	}
	else
	{
		input_nodes = (Input_node**)malloc(sizeof(Input_node*));

		double * swap_weight_values = weight_values;
		weight_values = (double*)malloc(2 * sizeof(double));
		memmove(weight_values, swap_weight_values, sizeof(double));
		free(swap_weight_values);

		WEIGHT_INPUT_TYPE * swap_input_types = input_types;
		input_types = (WEIGHT_INPUT_TYPE*)malloc(2 * sizeof(WEIGHT_INPUT_TYPE));
		memmove(input_types, swap_input_types, sizeof(WEIGHT_INPUT_TYPE));
		free(swap_input_types);

		unsigned int * swap_input_positions = input_positions;
		input_positions = (unsigned int *)malloc(2 * sizeof(unsigned int));
		memmove(input_positions, swap_input_positions, sizeof(unsigned int));
		free(swap_input_positions);
	}

	*(input_nodes + inputs_count) = src_input_node_pointer;
	*(weight_values + inputs_count + 1) = src_weight_value;
	*(input_types + inputs_count + 1) = src_input_type;
	*(input_positions + inputs_count + 1) = src_input_position;

	inputs_count++;

}



void Weight_node::addInputNode_external_weight_external_error_contribution(Input_node * src_input_node_pointer, const double src_weight_value, const WEIGHT_INPUT_TYPE src_input_type, const unsigned int src_input_position)
{
	if (inputs_count > 0)
	{
		Input_node ** swap_input_nodes = input_nodes;
		input_nodes = (Input_node**)malloc((inputs_count + 1) * sizeof(Input_node*));
		memmove(input_nodes, swap_input_nodes, inputs_count * sizeof(Input_node*));
		free(swap_input_nodes);

		WEIGHT_INPUT_TYPE * swap_input_types = input_types;
		input_types = (WEIGHT_INPUT_TYPE*)malloc((inputs_count + 2) * sizeof(WEIGHT_INPUT_TYPE));
		memmove(input_types, swap_input_types, (inputs_count + 1) * sizeof(WEIGHT_INPUT_TYPE));
		free(swap_input_types);
		
		unsigned int * swap_input_positions = input_positions;
		input_positions = (unsigned int *)malloc((inputs_count + 2) * sizeof(unsigned int));
		memmove(input_positions, swap_input_positions, (inputs_count + 1) * sizeof(unsigned int));
		free(swap_input_positions);
	}
	else
	{
		input_nodes = (Input_node**)malloc(sizeof(Input_node*));

		WEIGHT_INPUT_TYPE * swap_input_types = input_types;
		input_types = (WEIGHT_INPUT_TYPE*)malloc(2 * sizeof(WEIGHT_INPUT_TYPE));
		memmove(input_types, swap_input_types, sizeof(WEIGHT_INPUT_TYPE));
		free(swap_input_types);

		unsigned int * swap_input_positions = input_positions;
		input_positions = (unsigned int *)malloc(2 * sizeof(unsigned int));
		memmove(input_positions, swap_input_positions, sizeof(unsigned int));
		free(swap_input_positions);
	}

	*(input_nodes + inputs_count) = src_input_node_pointer;

	// Weights is determined from the external array value:
	//*(weight_values + inputs_count + 1) = src_weight_value;
	*(input_types + inputs_count + 1) = src_input_type;
	*(input_positions + inputs_count + 1) = src_input_position;

	inputs_count++;
}



void Weight_node::assignExternalWeightPointer(double * src_weight_pointer)
{
	external_weights_source = true;

	if (weight_values)
	{
		free(weight_values);
	}

	if (external_weights_error_contribution_source)
	{		
		input_node_method = &Weight_node::addInputNode_external_weight_external_error_contribution;
	}
	else
	{
		input_node_method = &Weight_node::addInputNode_external_weight_self_error_contribution;
	}

	weight_values = src_weight_pointer;
}



void Weight_node::assignExternalWeightErrorContributionPointer(double ** src_weight_error_contribution_pointer)
{
	external_weights_error_contribution_source = true;

	if (weight_error_contribution)
	{
		for (unsigned int input_index = 0; input_index < inputs_count; input_index++)
		{
			free(*(weight_error_contribution + input_index));
		}
		free(weight_error_contribution);
	}

	if (external_weights_source)
	{
		input_node_method = &Weight_node::addInputNode_external_weight_external_error_contribution;
	}
	else
	{
		input_node_method = &Weight_node::addInputNode_self_weight_external_error_contribution;
	}

	weight_error_contribution = src_weight_error_contribution_pointer;
}



void Weight_node::assignOutputsCount(const unsigned int src_outputs_count)
{
	outputs_count = src_outputs_count;
	*weight_error_contribution = (double*)malloc(outputs_count * sizeof(double));
}



unsigned int Weight_node::getInputsCount()
{
	return inputs_count;
}



double Weight_node::getWeightValue(const unsigned int src_input_index)
{
	return *(weight_values + src_input_index);
}



// Computes the weighted sum of the inputs and their corresponding weights
double Weight_node::getWeightedSum(const unsigned long long current_time)
{
	weighted_sum = *weight_values;
	for (unsigned int i = 0; i < inputs_count; i++)
	{
		weighted_sum += *(weight_values + i + 1) * (*(input_nodes + i))->getInput(current_time);
	}

	current_weight_time = current_time;
	return weighted_sum;
}



double Weight_node::getWeightErrorContribution(const unsigned int src_input_index, const unsigned int src_output_index)
{
	return *(*(weight_error_contribution + src_input_index) + src_output_index);
}


// Updates the weight values using the new values passed
void Weight_node::updateWeightValue(const double src_updated_weight_value, const unsigned int src_input_index)
{
	*(weight_values + src_input_index) = src_updated_weight_value;
}


void Weight_node::setWeightsErrorContribution(const double src_node_error_contribution, const unsigned int src_output_index)
{
	*(*weight_error_contribution + src_output_index) = src_node_error_contribution;

	for (unsigned int input_index = 0; input_index < inputs_count; input_index++)
	{
		*(*(weight_error_contribution + input_index + 1) + src_output_index) = src_node_error_contribution * (*(input_nodes + input_index))->getInput(current_weight_time);

		(*(input_nodes + input_index))->addNodeErrorContribution(src_node_error_contribution * *(weight_values + input_index + 1), src_output_index);
	}
}



unsigned int Weight_node::getInputPosition(const unsigned int src_input_index)
{
	return *(input_positions + src_input_index);
}



Weight_node::WEIGHT_INPUT_TYPE Weight_node::getInputType(const unsigned int src_input_index)
{
	return *(input_types + src_input_index);
}
