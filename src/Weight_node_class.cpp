#include "Weight_node_class.h"


Weight_node::Weight_node(const unsigned int src_outputs_count, const unsigned int src_neuron_index = 0, double **** src_weight_values_master_pointer = NULL, double ***** src_weight_derivatives_values_master_pointer = NULL)
{
	outputs_count = src_outputs_count;
	neuron_index = src_neuron_index;
	
	if (src_weight_values_master_pointer)
	{
		// Use an external array to contain the weight's values:
		weight_values_master_pointer = src_weight_values_master_pointer;
		self_weight_values = NULL;
		self_weight_derivatives_values_pointer = NULL;

		assign_weight_value_method = &Weight_node::assignWeightValueExternal;
	}
	else
	{
		// Allocate the memory for the weight's values of the current neuron:
		self_weight_values = (double**)malloc(sizeof(double*));

		// Start by allocating the memory required for the bias:
		*self_weight_values = (double*)malloc(sizeof(double));

		self_weight_values_pointer = &self_weight_values;
		weight_values_master_pointer = &self_weight_values_pointer;

		assign_weight_value_method = &Weight_node::assignWeightValueSelf;
	}

	if (src_weight_derivatives_values_master_pointer)
	{
		// Use an external array to contain the weight's derivatives values:
		weight_derivatives_values_master_pointer = src_weight_derivatives_values_master_pointer;
		self_weight_derivatives_values = NULL;
		self_weight_derivatives_values_pointer = NULL;
	}
	else
	{
		// Allocate the memory for the weight's derivatives values of the current neuron:
		self_weight_derivatives_values = (double***)malloc(sizeof(double**));

		// Start by allocating the memory required for the bias:
		*self_weight_derivatives_values = (double**)malloc(sizeof(double*));
		**self_weight_derivatives_values = (double*)malloc(outputs_count * sizeof(double));

		self_weight_derivatives_values_pointer = &self_weight_derivatives_values;
		weight_derivatives_values_master_pointer = &self_weight_derivatives_values_pointer;
	}

	// Start by allocating the memory for the bias node:
	weights_list_tail = (WEIGHT_LIST_NODE*)malloc(sizeof(WEIGHT_LIST_NODE));
	weights_list_tail->weight_node_type = WIT_BIAS;
	weights_list_tail->input_node = NULL;
	weights_list_tail->input_index = 0;
	weights_list_tail->next_weight_node = NULL;

	weights_list_head = weights_list_tail;

	inputs_count = 1;
}


Weight_node::~Weight_node()
{
	// If the weight's derivatives values were self defined, free the memory required.
	if (self_weight_derivatives_values_pointer)
	{
		for (unsigned int input_index = 0; input_index < inputs_count; input_index++)
		{
			free(*(*self_weight_derivatives_values + input_index));
		}
		free(*self_weight_derivatives_values);
		free(self_weight_derivatives_values);
	}

	// If the weight's values were self defined, free the memory required.
	if (self_weight_values_pointer)
	{
		free(*self_weight_values);
		free(self_weight_values);
	}

	// Free the allocated memory for each weight value:
	WEIGHT_LIST_NODE * current_weight_node;
	WEIGHT_LIST_NODE * next_weight_node = weights_list_head;
	do
	{
		current_weight_node = next_weight_node;
		next_weight_node = current_weight_node->next_weight_node;

		free(current_weight_node);
	} while (next_weight_node);
}



void Weight_node::addInputNode(Input_node * src_input_node_pointer, const double src_weight_value, const WEIGHT_INPUT_TYPE src_input_type)
{
	weights_list_tail->next_weight_node = (WEIGHT_LIST_NODE*)malloc(sizeof(WEIGHT_LIST_NODE));
	weights_list_tail->next_weight_node->weight_node_type = src_input_type;
	weights_list_tail->next_weight_node->input_node = src_input_node_pointer;
	weights_list_tail->next_weight_node->input_index = inputs_count;
	weights_list_tail->next_weight_node->next_weight_node = NULL;

	weights_list_tail = weights_list_tail->next_weight_node;

	(this->*assign_weight_value_method)(src_weight_value);

	inputs_count++;
}



void Weight_node::assignWeightValueSelf(const double src_weigth_value)
{
	// Require more memory for the newly added weighted input
	double * self_weight_values_swap = *self_weight_values;
	*self_weight_values = (double*)malloc((inputs_count + 1) * sizeof(double));
	memmove(*self_weight_values, self_weight_values_swap, inputs_count * sizeof(double));
	free(self_weight_values_swap);
		
	double ** self_weight_derivatives_values_swap = *self_weight_derivatives_values;
	*self_weight_derivatives_values = (double**)malloc((inputs_count + 1) * sizeof(double*));
	memmove(self_weight_derivatives_values_swap, *self_weight_derivatives_values, inputs_count * sizeof(double*));
	free(self_weight_derivatives_values_swap);
	
	*(*(**weight_values_master_pointer + neuron_index) + inputs_count) = src_weigth_value;
	*(*self_weight_derivatives_values + inputs_count) = (double*)malloc(outputs_count * sizeof(double));
}



void Weight_node::assignWeightValueExternal(const double src_weigth_value)
{
	*(*(**weight_values_master_pointer + neuron_index) + inputs_count) = src_weigth_value;
}



unsigned int Weight_node::getInputsCount()
{
	return inputs_count;
}



double Weight_node::getWeightedSum(const unsigned long long current_time)
{
	current_weight_time = current_time;
	return *(*(**weight_values_master_pointer + neuron_index)) + getWeightedSumNode(weights_list_head->next_weight_node, current_time);
}



double Weight_node::getWeightedSumNode(WEIGHT_LIST_NODE * src_current_weight_node, const unsigned long long current_time)
{
	if (!src_current_weight_node)
	{
		return 0.0;
	}

	return getWeightedSumNode(src_current_weight_node->next_weight_node, current_time) +
		*(*(**weight_values_master_pointer + neuron_index) + src_current_weight_node->input_index) *
		src_current_weight_node->input_node->getInput(current_time);
}



void Weight_node::setWeightValue(const unsigned int src_input_index, const double src_weight_value)
{
	*(*(**weight_values_master_pointer + neuron_index) + src_input_index) = src_weight_value;
}



double Weight_node::getWeightValue(const unsigned int src_input_index)
{
	return *(*(**weight_values_master_pointer + neuron_index) + src_input_index);
}



void Weight_node::setWeightsErrorContribution(const double src_node_error_contribution, const unsigned int src_output_index)
{
	// Assign the error contribution corresponding to the bias:
	*(*(*(**weight_derivatives_values_master_pointer + neuron_index)) + src_output_index) = src_node_error_contribution;

	setWeightsErrorContributionNode(weights_list_head->next_weight_node, src_node_error_contribution, src_output_index);
}



void Weight_node::setWeightsErrorContributionNode(WEIGHT_LIST_NODE * src_current_weight_node, const double src_node_error_contribution, const unsigned int src_output_index)
{
	if (!src_current_weight_node)
	{
		return;
	}

	// Assign the error contribution corresponding to this weight:
	*(*(*(**weight_derivatives_values_master_pointer + neuron_index) + src_current_weight_node->input_index) + src_output_index) = src_node_error_contribution * src_current_weight_node->input_node->getInput(current_weight_time);

	// Back-propagate the error contribution to the corresponding input:
	src_current_weight_node->input_node->addNodeErrorContribution(src_node_error_contribution * *(*(**weight_values_master_pointer + neuron_index) + src_current_weight_node->input_index), src_output_index);
}