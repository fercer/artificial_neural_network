#include "Neuron.h"

Neuron::Neuron(const unsigned int src_neuron_position, const unsigned int src_outputs_count, const bool src_compute_derivatives, double **** src_weight_values_master_pointer, double **** src_weight_derivatives_values_master_pointer)
{
	weights_array = NULL;
	dump_weights_list_into_array_required = false;
	neuron_activation_function = NULL;
	node_current_time = 0;

	global_node_index = src_neuron_position;
	neuron_outputs_count = src_outputs_count;
	compute_derivatives = src_compute_derivatives;
	if (compute_derivatives)
	{
		neuron_error_contribution = (double*)malloc(neuron_outputs_count * sizeof(double));
		get_input_method = &Neuron::getInputWithDerivative;
	}
	else
	{
		neuron_error_contribution = NULL;
		get_input_method = &Neuron::getInputOnly;
	}

	weight_values_master_pointer = src_weight_values_master_pointer;
	weight_derivatives_values_master_pointer = src_weight_derivatives_values_master_pointer;

	/* Allocate the memory for the bias: */
	weights_list_head.weighted_input = NULL;
	weights_list_head.next_weighted_input = NULL;
	weights_list_tail = &weights_list_head;
	
	neuron_inputs_count = 0;
}



Neuron::Neuron(const Neuron &src_neuron)
{
	this->weights_array = NULL;
	this->dump_weights_list_into_array_required = false;
	this->global_node_index = src_neuron.global_node_index;
	this->node_current_time = 0;
	this->neuron_outputs_count = src_neuron.neuron_outputs_count;
	this->neuron_inputs_count = src_neuron.neuron_inputs_count;

	this->compute_derivatives = src_neuron.compute_derivatives;
	if (this->compute_derivatives)
	{
		this->neuron_error_contribution = (double*)malloc(this->neuron_outputs_count * sizeof(double));
		this->get_input_method = &Neuron::getInputWithDerivative;
	}
	else
	{
		this->neuron_error_contribution = NULL;
		this->get_input_method = &Neuron::getInputOnly;
	}

	this->weight_values_master_pointer = src_neuron.weight_values_master_pointer;
	this->weight_derivatives_values_master_pointer = src_neuron.weight_derivatives_values_master_pointer;
	
	/* Allocate the memory for the bias: */
	this->weights_list_head.next_weighted_input = NULL;
	this->weights_list_head.weighted_input = NULL;
	this->weights_list_tail = &(this->weights_list_head);
	
	WEIGHTS_LIST_NODE * current_src_weight_node_pointer;
	WEIGHTS_LIST_NODE * next_src_weight_node_pointer = src_neuron.weights_list_head.next_weighted_input;

	while(next_src_weight_node_pointer)
	{
		current_src_weight_node_pointer = next_src_weight_node_pointer;
		next_src_weight_node_pointer = current_src_weight_node_pointer->next_weighted_input;

		this->weights_list_tail->next_weighted_input = new WEIGHTS_LIST_NODE;
		this->weights_list_tail = this->weights_list_tail->next_weighted_input;

		this->weights_list_tail->weighted_input = new Weight_node(*(current_src_weight_node_pointer->weighted_input));
		this->weights_list_tail->next_weighted_input = NULL;
		dump_weights_list_into_array_required = true;
	}

	dumpWeightsListIntoArray();

	this->neuron_activation_function_type = src_neuron.neuron_activation_function_type;
	switch (this->neuron_activation_function_type)
	{
	case ACT_HYPERBOLIC_TANGENT:
		this->neuron_activation_function = new activationHyperbolicTangent(*(activationHyperbolicTangent*)(src_neuron.neuron_activation_function));
		break;
	case ACT_SIGMOID:
		this->neuron_activation_function = new activationSigmoid(*(activationSigmoid*)(src_neuron.neuron_activation_function));
		break;
	case ACT_IDENTITY:
		this->neuron_activation_function = new identityActivationFunction(*(identityActivationFunction*)(src_neuron.neuron_activation_function));
		break;
	}
}



Neuron & Neuron::operator= (const Neuron &src_neuron)
{
	if (this != &src_neuron)
	{
		this->weights_array = NULL;
		this->dump_weights_list_into_array_required = false;

		this->global_node_index = src_neuron.global_node_index;
		this->node_current_time = 0;
		this->neuron_outputs_count = src_neuron.neuron_outputs_count;
		this->neuron_inputs_count = src_neuron.neuron_inputs_count;

		this->compute_derivatives = src_neuron.compute_derivatives;
		if (this->compute_derivatives)
		{
			this->neuron_error_contribution = (double*)malloc(this->neuron_outputs_count * sizeof(double));
			this->get_input_method = &Neuron::getInputWithDerivative;
		}
		else
		{
			this->neuron_error_contribution = NULL;
			this->get_input_method = &Neuron::getInputOnly;
		}

		this->weight_values_master_pointer = src_neuron.weight_values_master_pointer;
		this->weight_derivatives_values_master_pointer = src_neuron.weight_derivatives_values_master_pointer;

		/* Allocate the memory for the bias: */
		this->weights_list_head.weighted_input = NULL;
		this->weights_list_head.next_weighted_input = NULL;
		this->weights_list_tail = &(this->weights_list_head);

		WEIGHTS_LIST_NODE * current_src_weight_node_pointer;
		WEIGHTS_LIST_NODE * next_src_weight_node_pointer = src_neuron.weights_list_head.next_weighted_input;

		while (next_src_weight_node_pointer)
		{
			current_src_weight_node_pointer = next_src_weight_node_pointer;
			next_src_weight_node_pointer = current_src_weight_node_pointer->next_weighted_input;

			this->weights_list_tail->next_weighted_input = new WEIGHTS_LIST_NODE;
			this->weights_list_tail = this->weights_list_tail->next_weighted_input;

			this->weights_list_tail->weighted_input = new Weight_node(*(current_src_weight_node_pointer->weighted_input));
			this->weights_list_tail->next_weighted_input = NULL;
			dump_weights_list_into_array_required = true;
		}

		dumpWeightsListIntoArray();

		this->neuron_activation_function_type = src_neuron.neuron_activation_function_type;
		switch (this->neuron_activation_function_type)
		{
		case ACT_HYPERBOLIC_TANGENT:
			this->neuron_activation_function = new activationHyperbolicTangent(*(activationHyperbolicTangent*)(src_neuron.neuron_activation_function));
			break;
		case ACT_SIGMOID:
			this->neuron_activation_function = new activationSigmoid(*(activationSigmoid*)(src_neuron.neuron_activation_function));
			break;
		case ACT_IDENTITY:
			this->neuron_activation_function = new identityActivationFunction(*(identityActivationFunction*)(src_neuron.neuron_activation_function));
			break;
		}
	}
	return *this;
}



Neuron::~Neuron()
{
	if (neuron_activation_function)
	{
		delete neuron_activation_function;
	}

	if (neuron_error_contribution)
	{
		free(neuron_error_contribution);
	}

	/* Free the memory allocated for the weighted inputs */
	WEIGHTS_LIST_NODE * current_weight_node_pointer;
	WEIGHTS_LIST_NODE * next_weight_node_pointer = weights_list_head.next_weighted_input;

	while (next_weight_node_pointer)
	{
		current_weight_node_pointer = next_weight_node_pointer;
		next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;
		delete current_weight_node_pointer;
	}

	if (weights_array)
	{
		free(weights_array);
	}
}



void Neuron::addWeightedInput(Weight_node::WEIGHT_INPUT_TYPE src_input_type, Input_node * src_input_node_pointer, const double src_weight_value)
{
	/* Allocate the memory for the newly added weighted input: */
	weights_list_tail->next_weighted_input = new WEIGHTS_LIST_NODE;
	weights_list_tail = weights_list_tail->next_weighted_input;

	weights_list_tail->weighted_input = new Weight_node(
		neuron_outputs_count, /*Pass the number of outputs of the network */
		src_input_type, /* Define the first weight as the bias */
		src_input_node_pointer, /* The bias does not have an input node to be connected */
		src_weight_value, /* The bias is intialized with 0 value */
		global_node_index, /* Pass this neuron position in the network */
		neuron_inputs_count, /* This is the first weight in the list */
		weight_values_master_pointer, /* Use either, the external weight value, or the self allocated */
		weight_derivatives_values_master_pointer/* Use either, the external weight derivatives values, or the self allocated */
	);

	weights_list_tail->next_weighted_input = NULL;
	neuron_inputs_count++;
	dump_weights_list_into_array_required = true;
}


void Neuron::dumpWeightsListIntoArray()
{
	if (!dump_weights_list_into_array_required)
	{
		return;
	}

	if (weights_array)
	{
		free(weights_array);
	}

	weights_array = (Weight_node**)malloc(neuron_inputs_count * sizeof(Weight_node));

	WEIGHTS_LIST_NODE * current_weight_node_pointer;
	WEIGHTS_LIST_NODE * next_weight_node_pointer = weights_list_head.next_weighted_input;

	unsigned int weighted_input_index = 0;
	while (next_weight_node_pointer)
	{
		current_weight_node_pointer = next_weight_node_pointer;
		next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;

		*(weights_array + weighted_input_index) = current_weight_node_pointer->weighted_input;
	}

	dump_weights_list_into_array_required = false;
}


unsigned int Neuron::getInputsCount()
{
	return neuron_inputs_count;
}



double Neuron::getInput(const unsigned long long src_current_network_time)
{
	return (this->*get_input_method)(src_current_network_time);
}



void Neuron::addNodeErrorContribution(const double src_error_contribution, const unsigned int src_output_index)
{
	*(neuron_error_contribution + src_output_index) = *(neuron_error_contribution + src_output_index) + derivative_response_to_input * src_error_contribution;
}



void Neuron::backpropagateNodeError()
{
	WEIGHTS_LIST_NODE * current_weight_node_pointer;
	WEIGHTS_LIST_NODE * next_weight_node_pointer = weights_list_head.next_weighted_input;

	while (next_weight_node_pointer)
	{
		current_weight_node_pointer = next_weight_node_pointer;
		next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;

		for (unsigned int output_index = 0; output_index < neuron_outputs_count; output_index++)
		{
			current_weight_node_pointer->weighted_input->setWeightsErrorContribution(*(neuron_error_contribution + output_index), output_index);
		}
	}
}



void Neuron::backpropagateNodeError(const unsigned int src_output_index)
{
	WEIGHTS_LIST_NODE * current_weight_node_pointer;
	WEIGHTS_LIST_NODE * next_weight_node_pointer = weights_list_head.next_weighted_input;

	while (next_weight_node_pointer)
	{
		current_weight_node_pointer = next_weight_node_pointer;
		next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;

		current_weight_node_pointer->weighted_input->setWeightsErrorContribution(*(neuron_error_contribution + src_output_index), src_output_index);
	}
}



void Neuron::setActivationFunction(ACTIVATION_FUNCTION_TYPE src_activation_function, double * src_function_parameters)
{
	neuron_activation_function_type = src_activation_function;
	switch (src_activation_function)
	{
	case ACT_HYPERBOLIC_TANGENT:
		neuron_activation_function = new activationHyperbolicTangent(src_function_parameters);
		break;
	case ACT_SIGMOID:
		neuron_activation_function = new activationSigmoid(src_function_parameters);
		break;
	case ACT_IDENTITY:
		neuron_activation_function = new identityActivationFunction;
		break;
	}
}



void Neuron::setInputNodePointer(Input_node * src_input_node_pointer, const unsigned int src_input_index)
{
	dumpWeightsListIntoArray();

	(*(weights_array + src_input_index))->setInputNodePointer(src_input_node_pointer);
}



Input_node *  Neuron::getInputNodePointer(const unsigned int src_input_index)
{
	dumpWeightsListIntoArray();

	return (*(weights_array + src_input_index))->getInputNodePointer();
}



void Neuron::resetNodeCurrentTime()
{
	node_current_time = 0;
	neuron_activation_function->resetActivationFunctionCurrentTime();

	WEIGHTS_LIST_NODE * current_weight_node_pointer;
	WEIGHTS_LIST_NODE * next_weight_node_pointer = weights_list_head.next_weighted_input;

	while (next_weight_node_pointer)
	{
		current_weight_node_pointer = next_weight_node_pointer;
		next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;

		current_weight_node_pointer->weighted_input->resetWeightCurrentTime();
	}
}



double Neuron::getInputOnly(const unsigned long long src_current_network_time)
{
	if (src_current_network_time > node_current_time)
	{
		double neuron_weighted_sum = 0.0;
		WEIGHTS_LIST_NODE * current_weight_node_pointer;
		WEIGHTS_LIST_NODE * next_weight_node_pointer = weights_list_head.next_weighted_input;

		while (next_weight_node_pointer)
		{
			current_weight_node_pointer = next_weight_node_pointer;
			next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;

			neuron_weighted_sum += current_weight_node_pointer->weighted_input->getWeightedInput(src_current_network_time);
		}
		
		response_to_input = neuron_activation_function->evaluateFunction(neuron_weighted_sum, src_current_network_time);
		node_current_time = src_current_network_time;
	}

	return response_to_input;
}



double Neuron::getInputWithDerivative(const unsigned long long src_current_network_time)
{
	if (src_current_network_time > node_current_time)
	{
		double neuron_weighted_sum = 0.0;
		WEIGHTS_LIST_NODE * current_weight_node_pointer;
		WEIGHTS_LIST_NODE * next_weight_node_pointer = weights_list_head.next_weighted_input;

		while (next_weight_node_pointer)
		{
			current_weight_node_pointer = next_weight_node_pointer;
			next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;

			neuron_weighted_sum += current_weight_node_pointer->weighted_input->getWeightedInput(src_current_network_time);
		}
		response_to_input = neuron_activation_function->evaluateFunction(neuron_weighted_sum, src_current_network_time);
		derivative_response_to_input = neuron_activation_function->evaluateDerivative(neuron_weighted_sum);

		memset(neuron_error_contribution, 0, neuron_outputs_count * sizeof(double));
		node_current_time = src_current_network_time;
	}

	return response_to_input;
}



void Neuron::makeExternalWeightValues(double **** src_weight_values_master_pointer, double **** src_weight_derivatives_values_master_pointer)
{
	WEIGHTS_LIST_NODE * current_weight_node_pointer;
	WEIGHTS_LIST_NODE * next_weight_node_pointer = weights_list_head.next_weighted_input;

	while (next_weight_node_pointer)
	{
		current_weight_node_pointer = next_weight_node_pointer;
		next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;

		current_weight_node_pointer->weighted_input->makeExternalWeigthValue(src_weight_values_master_pointer, src_weight_derivatives_values_master_pointer);
	}
}



void Neuron::makeInternalWeightValues(const bool make_weights_values_internal, const bool make_weights_derivatives_values_internal)
{
	WEIGHTS_LIST_NODE * current_weight_node_pointer;
	WEIGHTS_LIST_NODE * next_weight_node_pointer = weights_list_head.next_weighted_input;

	while (next_weight_node_pointer)
	{
		current_weight_node_pointer = next_weight_node_pointer;
		next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;

		current_weight_node_pointer->weighted_input->makeInternalWeightValue(make_weights_values_internal, make_weights_derivatives_values_internal);
	}
}



void Neuron::dumpNodeData(FILE * fp_network_data)
{
	fprintf(fp_network_data, "\t<Neuron position = \"%i\">\n", global_node_index);

	/* Dump the weighted connections data: */
	WEIGHTS_LIST_NODE * current_weight_node_pointer;
	WEIGHTS_LIST_NODE * next_weight_node_pointer = weights_list_head.next_weighted_input;

	while (next_weight_node_pointer)
	{
		current_weight_node_pointer = next_weight_node_pointer;
		next_weight_node_pointer = current_weight_node_pointer->next_weighted_input;

		current_weight_node_pointer->weighted_input->dumpWeightData(fp_network_data);

	}

	/* Dump the activation function data: */
	neuron_activation_function->dumpActivationFunctionData(fp_network_data);

	fprintf(fp_network_data, "\t</Neuron>\n");
}