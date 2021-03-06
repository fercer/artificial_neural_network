#include "Weight_node_class.h"


Weight_node::Weight_node(const unsigned int src_outputs_count, const WEIGHT_INPUT_TYPE src_input_type, Input_node * src_input_node, const double src_weight_value, const unsigned int src_neuron_index, const unsigned int src_input_index, double *** src_weight_values_master_pointer, double **** src_weight_derivatives_values_master_pointer)
{
	outputs_count = src_outputs_count;
	input_type = src_input_type;
	
	neuron_index = src_neuron_index;
	input_index = src_input_index;

	switch (input_type)
	{
	case WIT_BIAS:
		set_weight_error_cotribution = &Weight_node::setBiasErrorContribution;
		get_input_response_with_derivatives = &Weight_node::getBias;
		get_input_response = &Weight_node::getBias;
		dump_weight_method = &Weight_node::dumpWeightDataBias;
		get_input_node_global_index = &Weight_node::getBiasGlobalIndex;
		break;

	case WIT_PATTERN:
		set_weight_error_cotribution = &Weight_node::setWeightErrorContribution;
		get_input_response_with_derivatives = &Weight_node::getWeightedInputWithDerivatives;
		get_input_response = &Weight_node::getWeightedInput;
		dump_weight_method = &Weight_node::dumpWeightDataPattern;
		get_input_node_global_index = &Weight_node::getWeightedInputNodeGlobalIndex;
		break;

	case WIT_NEURON:
		set_weight_error_cotribution = &Weight_node::setWeightErrorContribution;
		get_input_response_with_derivatives = &Weight_node::getWeightedInputWithDerivatives;
		get_input_response = &Weight_node::getWeightedInput;
		dump_weight_method = &Weight_node::dumpWeightDataNeuron;
		get_input_node_global_index = &Weight_node::getWeightedInputNodeGlobalIndex;
		break;
	}
	
	/* Point the following variables to their respective pointers
	If the memory is self allocated, the weight value and derivatives values will be accessible from them:
	*/
	self_neuron_weight_values = &self_weight_value;
	self_neuron_weight_derivatives_values = &self_weight_derivatives_values;
	self_weights_pointers_manager = &self_neuron_weight_values;
	self_weights_derivatives_pointers_manager = &self_neuron_weight_derivatives_values;

	if (src_weight_values_master_pointer)
	{
		// Use an external array to contain the weight's values:
		weight_values_master_pointer = src_weight_values_master_pointer;

		/* If the weights are defined externally, it is required the know neurons and input indices */
		ewv_neuron_index = neuron_index;
		ewv_input_index = input_index;
	}
	else
	{
		// Allocate the memory for the weight's value:
		weight_values_master_pointer = &self_weights_pointers_manager;

		/* If the weights are defined locally, it is not required the know neurons and input indices */
		ewv_neuron_index = 0;
		ewv_input_index = 0;
	}

	if (src_weight_derivatives_values_master_pointer)
	{
		// Use an external array to contain the weight's derivatives values:
		weight_derivatives_values_master_pointer = src_weight_derivatives_values_master_pointer;
		self_weight_derivatives_values = NULL;

		/* If the weight's derivatives are defined externally,
		it is required the know neurons and input indices
		*/
		ewdv_neuron_index = neuron_index;
		ewdv_input_index = input_index;
	}
	else
	{
		// Allocate the memory for the weight's derivatives values:
		self_weight_derivatives_values = (double*)malloc(outputs_count * sizeof(double));
		weight_derivatives_values_master_pointer = &self_weights_derivatives_pointers_manager;

		/* If the weight's derivatives are defined locally,
		it is not required the know neurons and input indices
		*/
		ewdv_neuron_index = 0;
		ewdv_input_index = 0;
	}

	// Assign the weight value:
	setWeightValue(src_weight_value);

	// Assign the input value:
	input_node_pointer = src_input_node;

	current_weight_time = 0;
}


Weight_node::Weight_node(const Weight_node & src_weight_node)
{
	this->neuron_index = src_weight_node.neuron_index;
	this->input_index = src_weight_node.input_index;
	this->outputs_count = src_weight_node.outputs_count;
	this->input_type = src_weight_node.input_type;

	/* This might cause some problems due to the copied pointer is pointing to the node of another network.
	A reassignement of the input node must be done using the setInputNodePointer method.
	*/
	this->input_node_pointer = src_weight_node.input_node_pointer;

	/* The time is restarted */
	current_weight_time = 0;	

	/* This copy constructor will define the weight and derivatives values as self allocated.
	All the values will be extracted from the master pointers in the source weight object
	despite their actual origin.
	External definition of the weight and derivatives values will require the makeExternalWeigthValue
	method.
	*/
	this->self_weight_value = *(*(*(src_weight_node.weight_values_master_pointer) + src_weight_node.ewv_neuron_index) + src_weight_node.ewv_input_index);
	this->self_weight_derivatives_values = (double*)malloc(this->outputs_count * sizeof(double));
	memcpy(this->self_weight_derivatives_values, 
		*(*(*(src_weight_node.weight_derivatives_values_master_pointer) + src_weight_node.ewdv_neuron_index) + src_weight_node.ewdv_input_index),
		this->outputs_count * sizeof(double));

	this->self_neuron_weight_values = &(this->self_weight_value);
	this->self_neuron_weight_derivatives_values = &(this->self_weight_derivatives_values);

	this->self_weights_pointers_manager = &(this->self_neuron_weight_values);
	this->self_weights_derivatives_pointers_manager = &(this->self_neuron_weight_derivatives_values);

	this->weight_values_master_pointer = &(this->self_weights_pointers_manager);
	this->weight_derivatives_values_master_pointer = &(this->self_weights_derivatives_pointers_manager);
	
	this->ewv_input_index = 0;
	this->ewv_neuron_index = 0;

	this->ewdv_input_index = 0;
	this->ewdv_neuron_index = 0;
	
	switch (this->input_type)
	{
	case WIT_BIAS:
		this->set_weight_error_cotribution = &Weight_node::setBiasErrorContribution;
		this->get_input_response_with_derivatives = &Weight_node::getBias;
		this->get_input_response = &Weight_node::getBias;
		this->dump_weight_method = &Weight_node::dumpWeightDataBias;
		this->get_input_node_global_index = &Weight_node::getBiasGlobalIndex;
		break;

	case WIT_PATTERN:
		this->set_weight_error_cotribution = &Weight_node::setWeightErrorContribution;
		this->get_input_response_with_derivatives = &Weight_node::getWeightedInputWithDerivatives;
		this->get_input_response = &Weight_node::getWeightedInput;
		this->dump_weight_method = &Weight_node::dumpWeightDataPattern;
		this->get_input_node_global_index = &Weight_node::getWeightedInputNodeGlobalIndex;
		break;

	case WIT_NEURON:
		this->set_weight_error_cotribution = &Weight_node::setWeightErrorContribution;
		this->get_input_response_with_derivatives = &Weight_node::getWeightedInputWithDerivatives;
		this->get_input_response = &Weight_node::getWeightedInput;
		this->dump_weight_method = &Weight_node::dumpWeightDataNeuron;
		this->get_input_node_global_index = &Weight_node::getWeightedInputNodeGlobalIndex;
		break;
	}
}



Weight_node & Weight_node::operator= (const Weight_node & src_weight_node)
{
	if (this != &src_weight_node)
	{
		this->neuron_index = src_weight_node.neuron_index;
		this->input_index = src_weight_node.input_index;
		this->outputs_count = src_weight_node.outputs_count;
		this->input_type = src_weight_node.input_type;

		/* This might cause some problems due to the copied pointer is pointing to the node of another network.
		A reassignement of the input node must be done using the setInputNodePointer method.
		*/
		this->input_node_pointer = src_weight_node.input_node_pointer;

		/* The time is restarted */
		current_weight_time = 0;

		/* This copy constructor will define the weight and derivatives values as self allocated.
		All the values will be extracted from the master pointers in the source weight object
		despite their actual origin.
		External definition of the weight and derivatives values will require the makeExternalWeigthValue
		method.
		*/
		this->self_weight_value = *(*(*(src_weight_node.weight_values_master_pointer) + src_weight_node.ewv_neuron_index) + src_weight_node.ewv_input_index);
		this->self_weight_derivatives_values = (double*)malloc(this->outputs_count * sizeof(double));
		memcpy(this->self_weight_derivatives_values,
			*(*(*(src_weight_node.weight_derivatives_values_master_pointer) + src_weight_node.ewdv_neuron_index) + src_weight_node.ewdv_input_index),
			this->outputs_count * sizeof(double));

		this->self_neuron_weight_values = &(this->self_weight_value);
		this->self_neuron_weight_derivatives_values = &(this->self_weight_derivatives_values);

		this->self_weights_pointers_manager = &(this->self_neuron_weight_values);
		this->self_weights_derivatives_pointers_manager = &(this->self_neuron_weight_derivatives_values);

		this->weight_values_master_pointer = &(this->self_weights_pointers_manager);
		this->weight_derivatives_values_master_pointer = &(this->self_weights_derivatives_pointers_manager);

		this->ewv_input_index = 0;
		this->ewv_neuron_index = 0;

		this->ewdv_input_index = 0;
		this->ewdv_neuron_index = 0;

		switch (this->input_type)
		{
		case WIT_BIAS:
			this->set_weight_error_cotribution = &Weight_node::setBiasErrorContribution;
			this->get_input_response_with_derivatives = &Weight_node::getBias;
			this->get_input_response = &Weight_node::getBias;
			this->dump_weight_method = &Weight_node::dumpWeightDataBias;
			this->get_input_node_global_index = &Weight_node::getBiasGlobalIndex;
			break;

		case WIT_PATTERN:
			this->set_weight_error_cotribution = &Weight_node::setWeightErrorContribution;
			this->get_input_response_with_derivatives = &Weight_node::getWeightedInputWithDerivatives;
			this->get_input_response = &Weight_node::getWeightedInput;
			this->dump_weight_method = &Weight_node::dumpWeightDataPattern;
			this->get_input_node_global_index = &Weight_node::getWeightedInputNodeGlobalIndex;
			break;

		case WIT_NEURON:
			this->set_weight_error_cotribution = &Weight_node::setWeightErrorContribution;
			this->get_input_response_with_derivatives = &Weight_node::getWeightedInputWithDerivatives;
			this->get_input_response = &Weight_node::getWeightedInput;
			this->dump_weight_method = &Weight_node::dumpWeightDataNeuron;
			this->get_input_node_global_index = &Weight_node::getWeightedInputNodeGlobalIndex;
			break;
		}
	}
	return *this;
}



Weight_node::~Weight_node()
{
	// If the weight's derivatives values were self defined, free the memory required.
	if (self_weight_derivatives_values)
	{
		free(self_weight_derivatives_values);
	}
}



void Weight_node::setInputNodePointer(Input_node * src_input_node_pointer)
{
	input_node_pointer = src_input_node_pointer;
}



int  Weight_node::getInputNodeGlobalIndex()
{
	return (this->*get_input_node_global_index)();
}



double Weight_node::getWeightedInput(const unsigned long long src_current_network_time)
{
	current_weight_time = src_current_network_time;
	return (this->*get_input_response)();
}



double Weight_node::getWeightedInputWithDerivatives(const unsigned long long src_current_network_time)
{
	current_weight_time = src_current_network_time;
	return (this->*get_input_response_with_derivatives)();
}



double Weight_node::getWeightErrorContribution(const unsigned int src_output_index, const unsigned long long src_current_network_time)
{
	return *(*(*(*weight_derivatives_values_master_pointer + ewdv_neuron_index) + ewdv_input_index) + src_output_index);
}



void Weight_node::setWeightValue(const double src_weight_value)
{
	*(*(*weight_values_master_pointer + ewv_neuron_index) + ewv_input_index) = src_weight_value;
}



double Weight_node::getWeightValue()
{
	return *(*(*weight_values_master_pointer + ewv_neuron_index) + ewv_input_index);
}



void Weight_node::setWeightsErrorContribution(const double src_node_error_contribution, const unsigned int src_output_index)
{
	(this->*set_weight_error_cotribution)(src_node_error_contribution, src_output_index);
}

Weight_node::WEIGHT_INPUT_TYPE Weight_node::getWeigthedInputType()
{
	return input_type;
}



void Weight_node::setWeightErrorContribution(const double src_node_error_contribution, const unsigned int src_output_index)
{
	// Assign the error contribution corresponding to this weight:
	*(*(*(*weight_derivatives_values_master_pointer + ewdv_neuron_index) + ewdv_input_index) + src_output_index) = src_node_error_contribution * input_node_pointer->getInputWithDerivatives(current_weight_time);
	
	// Back-propagate the error contribution to the corresponding input:
	input_node_pointer->addNodeErrorContribution(src_node_error_contribution *
		*(*(*weight_values_master_pointer + ewv_neuron_index) + ewv_input_index), src_output_index);
}



void Weight_node::setBiasErrorContribution(const double src_node_error_contribution, const unsigned int src_output_index)
{
	// Assign the error contribution corresponding to the bias:
	*(*(*(*weight_derivatives_values_master_pointer + ewdv_neuron_index)) + src_output_index) = src_node_error_contribution;
}



double Weight_node::getWeightedInputWithDerivatives()
{
	return input_node_pointer->getInputWithDerivatives(current_weight_time) * *(*(*weight_values_master_pointer + ewv_neuron_index) + ewv_input_index);
}



double Weight_node::getWeightedInput()
{
	return input_node_pointer->getInput(current_weight_time) * *(*(*weight_values_master_pointer + ewv_neuron_index) + ewv_input_index);
}



double Weight_node::getBias()
{
	return  *(*(*weight_values_master_pointer + ewv_neuron_index) + ewv_input_index);
}



void Weight_node::makeExternalWeigthValue(double *** src_weight_values_master_pointer, double **** src_weight_derivatives_values_master_pointer, const bool src_copy_to_external)
{
	if (src_weight_values_master_pointer)
	{
		ewv_neuron_index = neuron_index;
		ewv_input_index = input_index;

		weight_values_master_pointer = src_weight_values_master_pointer;
		if (src_copy_to_external)
		{
			*(*(*weight_values_master_pointer + ewv_neuron_index) + ewv_input_index) = self_weight_value;
		}
	}

	if (src_weight_derivatives_values_master_pointer)
	{
		ewdv_neuron_index = neuron_index;
		ewdv_input_index = input_index;
		weight_derivatives_values_master_pointer = src_weight_derivatives_values_master_pointer;
		
		if (src_copy_to_external)
		{
			memcpy(*(*(*weight_derivatives_values_master_pointer + ewdv_neuron_index) + ewdv_input_index),
				self_weight_derivatives_values,
				outputs_count * sizeof(double));
		}

		free(self_weight_derivatives_values);
		self_weight_derivatives_values = NULL;
	}
}



void Weight_node::makeInternalWeightValue(const bool make_weight_values_internal, const bool make_weight_derivatives_values_internal)
{
	if (make_weight_values_internal)
	{
		self_weight_value = *(*(*weight_values_master_pointer + ewv_neuron_index) + ewv_input_index);
		weight_values_master_pointer = &self_weights_pointers_manager;
		
		ewv_neuron_index = 0;
		ewv_input_index = 0;
	}

	if (make_weight_derivatives_values_internal)
	{
		self_weight_derivatives_values = (double*)malloc(outputs_count * sizeof(double));
		
		memcpy(self_weight_derivatives_values, *(*(*weight_derivatives_values_master_pointer + ewdv_neuron_index) + ewdv_input_index), outputs_count * sizeof(double));

		weight_derivatives_values_master_pointer = &self_weights_derivatives_pointers_manager;

		ewdv_neuron_index = 0;
		ewdv_input_index = 0;
	}
}



void Weight_node::resetWeightCurrentTime()
{
	current_weight_time = 0;
}



void Weight_node::dumpWeightData(FILE * fp_network_data)
{
	(this->*dump_weight_method)(fp_network_data);
}



void Weight_node::dumpWeightDataBias(FILE * fp_network_data)
{
	/* As bias, only its value is printed: */
	fprintf(fp_network_data, "\t\t<Bias value = \"%.63f\">\n", *(*(*weight_values_master_pointer + ewv_neuron_index) + ewv_input_index));

	for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
	{
		fprintf(fp_network_data, "\t\t\t<ErrorContribution output_position=\"%i\" value=\"%.63f\"></ErrorContribution>\n", output_index, 
			*(*(*(*weight_derivatives_values_master_pointer + ewdv_neuron_index) + ewdv_input_index) + output_index));
	}

	fprintf(fp_network_data, "\t\t</Bias>\n");
}



void Weight_node::dumpWeightDataNeuron(FILE * fp_network_data)
{
	/* Print weight value into the file: */
	fprintf(fp_network_data, "\t\t<Weight value = \"%.63f\" input_connection = \"Neuron\" input_position = \"%i\">\n",
		*(*(*weight_values_master_pointer + ewv_neuron_index) + ewv_input_index),
		input_node_pointer->getGlobalNodeIndex());

	for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
	{
		fprintf(fp_network_data, "\t\t\t<ErrorContribution output_position=\"%i\" value=\"%.63f\"></ErrorContribution>\n", output_index,
			*(*(*(*weight_derivatives_values_master_pointer + ewdv_neuron_index) + ewdv_input_index) + output_index));
	}

	fprintf(fp_network_data, "\t\t</Weight>\n");
}



void Weight_node::dumpWeightDataPattern(FILE * fp_network_data)
{
	/* Print weight value into the file: */
	fprintf(fp_network_data, "\t\t<Weight value = \"%.63f\" input_connection = \"Pattern\" input_position = \"%i\">\n",
		*(*(*weight_values_master_pointer + ewv_neuron_index) + ewv_input_index),
		input_node_pointer->getGlobalNodeIndex());

	for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
	{
		fprintf(fp_network_data, "\t\t\t<ErrorContribution output_position=\"%i\" value=\"%.63f\"></ErrorContribution>\n", output_index,
			*(*(*(*weight_derivatives_values_master_pointer + ewdv_neuron_index) + ewdv_input_index) + output_index));
	}

	fprintf(fp_network_data, "\t\t</Weight>\n");
}



int Weight_node::getWeightedInputNodeGlobalIndex()
{
	return input_node_pointer->getGlobalNodeIndex();
}



int Weight_node::getBiasGlobalIndex()
{
	return -1;
}
