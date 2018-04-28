#include "Weight_node_class.h"


Weight_node::Weight_node(const unsigned int src_outputs_count, const WEIGHT_INPUT_TYPE src_input_type, Input_node * src_input_node, const double src_weight_value, const unsigned int src_neuron_index, const unsigned int src_input_index, double **** src_weight_values_master_pointer, double **** src_weight_derivatives_values_master_pointer)
{
	outputs_count = src_outputs_count;
	input_type = src_input_type;
	
	neuron_index = src_neuron_index;
	input_index = src_input_index;

	switch (input_type)
	{
	case WIT_BIAS:
		set_weight_error_cotribution = &Weight_node::setBiasErrorContribution;
		get_input_response = &Weight_node::getBias;
		dump_weight_method = &Weight_node::dumpWeightDataBias;
		break;

	case WIT_PATTERN:
		set_weight_error_cotribution = &Weight_node::setWeightErrorContribution;
		get_input_response = &Weight_node::getWeightedInput;
		dump_weight_method = &Weight_node::dumpWeightDataPattern;
		break;

	case WIT_NEURON:
		set_weight_error_cotribution = &Weight_node::setWeightErrorContribution;
		get_input_response = &Weight_node::getWeightedInput;
		dump_weight_method = &Weight_node::dumpWeightDataNeuron;
		break;
	}
	
	/* Point the following variables to their respective pointers
	If the memory is self allocated, the weight value and derivatives values will be accessible from them:
	*/
	self_neuron_weight_values = &self_weight_value;
	self_neuron_weight_derivatives_values = &self_weight_derivatives_values;
	self_network_weight_values = &self_neuron_weight_values;
	self_network_weight_derivatives_values = &self_neuron_weight_derivatives_values;

	if (src_weight_values_master_pointer)
	{
		// Use an external array to contain the weight's values:
		weight_values_master_pointer = src_weight_values_master_pointer;
		self_weight_value = NULL;

		/* If the weights are defined externally, it is required the know neurons and input indices */
		ewv_neuron_index = neuron_index;
		ewv_input_index = input_index;
	}
	else
	{
		// Allocate the memory for the weight's value:
		self_weight_value = (double*)malloc(sizeof(double));
		weight_values_master_pointer = &self_network_weight_values;

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
		weight_derivatives_values_master_pointer = &self_network_weight_derivatives_values;

		/* If the weight's derivatives are defined locally,
		it is not required the know neurons and input indices
		*/
		ewdv_neuron_index = 0;
		ewdv_input_index = 0;
	}

	// Assign the weight value:
	setWeightValue(src_weight_value);
}


Weight_node::Weight_node(const Weight_node & src_weight_node)
{
	this->neuron_index = src_weight_node.neuron_index;
	this->input_index = src_weight_node.input_index;
	this->outputs_count = src_weight_node.outputs_count;
	this->input_type = src_weight_node.input_type;

	/* This might cause some problems due to the copied pointer is pointing to the node of another network.
	A reassignement of the input node must be done using the assignInputNodePointer method.
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
	this->self_weight_value = (double*) malloc(sizeof(double));
	this->self_weight_derivatives_values = (double*)malloc(outputs_count * sizeof(double));

	this->self_neuron_weight_values = &(this->self_weight_value);
	this->self_neuron_weight_derivatives_values = &(this->self_weight_derivatives_values);

	this->self_network_weight_values = &(this->self_neuron_weight_values);
	this->self_network_weight_derivatives_values = &(this->self_neuron_weight_derivatives_values);

	this->weight_values_master_pointer = &(this->self_network_weight_values);
	this->weight_derivatives_values_master_pointer = &(this->self_network_weight_derivatives_values);
	
	switch (this->input_type)
	{
	case WIT_BIAS:
		this->set_weight_error_cotribution = &Weight_node::setBiasErrorContribution;
		this->get_input_response = &Weight_node::getBias;
		this->dump_weight_method = &Weight_node::dumpWeightDataBias;
		break;

	case WIT_PATTERN:
		this->set_weight_error_cotribution = &Weight_node::setWeightErrorContribution;
		this->get_input_response = &Weight_node::getWeightedInput;
		this->dump_weight_method = &Weight_node::dumpWeightDataPattern;
		break;

	case WIT_NEURON:
		this->set_weight_error_cotribution = &Weight_node::setWeightErrorContribution;
		this->get_input_response = &Weight_node::getWeightedInput;
		this->dump_weight_method = &Weight_node::dumpWeightDataNeuron;
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
		A reassignement of the input node must be done using the assignInputNodePointer method.
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
		this->self_weight_value = (double*)malloc(sizeof(double));
		this->self_weight_derivatives_values = (double*)malloc(outputs_count * sizeof(double));

		this->self_neuron_weight_values = &(this->self_weight_value);
		this->self_neuron_weight_derivatives_values = &(this->self_weight_derivatives_values);

		this->self_network_weight_values = &(this->self_neuron_weight_values);
		this->self_network_weight_derivatives_values = &(this->self_neuron_weight_derivatives_values);

		this->weight_values_master_pointer = &(this->self_network_weight_values);
		this->weight_derivatives_values_master_pointer = &(this->self_network_weight_derivatives_values);

		switch (this->input_type)
		{
		case WIT_BIAS:
			this->set_weight_error_cotribution = &Weight_node::setBiasErrorContribution;
			this->get_input_response = &Weight_node::getBias;
			this->dump_weight_method = &Weight_node::dumpWeightDataBias;
			break;

		case WIT_PATTERN:
			this->set_weight_error_cotribution = &Weight_node::setWeightErrorContribution;
			this->get_input_response = &Weight_node::getWeightedInput;
			this->dump_weight_method = &Weight_node::dumpWeightDataPattern;
			break;

		case WIT_NEURON:
			this->set_weight_error_cotribution = &Weight_node::setWeightErrorContribution;
			this->get_input_response = &Weight_node::getWeightedInput;
			this->dump_weight_method = &Weight_node::dumpWeightDataNeuron;
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

	// If the weight's values were self defined, free the memory required.
	if (self_weight_value)
	{
		free(self_weight_value);
	}
}



void Weight_node::assignInputNodePointer(Input_node * src_input_node_pointer)
{
	input_node_pointer = src_input_node_pointer;
}



double Weight_node::getWeightedInput(const unsigned long long src_current_network_time)
{
	current_weight_time = src_current_network_time;
	return (this->*get_input_response)();
}



void Weight_node::setWeightValue(const double src_weight_value)
{
	*(*(**weight_values_master_pointer + ewv_neuron_index) + ewv_input_index) = src_weight_value;
}



double Weight_node::getWeightValue()
{
	return *(*(**weight_values_master_pointer + ewv_neuron_index) + ewv_input_index);
}



void Weight_node::setWeightsErrorContribution(const double src_node_error_contribution, const unsigned int src_output_index)
{
	(this->*set_weight_error_cotribution)(src_node_error_contribution, src_output_index);
}



void Weight_node::setWeightErrorContribution(const double src_node_error_contribution, const unsigned int src_output_index)
{
	// Assign the error contribution corresponding to this weight:
	*(*(**weight_derivatives_values_master_pointer + ewdv_neuron_index) + ewdv_input_index*outputs_count + src_output_index) = src_node_error_contribution * input_node_pointer->getInput(current_weight_time);
	
	// Back-propagate the error contribution to the corresponding input:
	input_node_pointer->addNodeErrorContribution(src_node_error_contribution * *(*(**weight_values_master_pointer + ewv_neuron_index) + ewv_input_index), src_output_index);
}



void Weight_node::setBiasErrorContribution(const double src_node_error_contribution, const unsigned int src_output_index)
{
	// Assign the error contribution corresponding to the bias:
	*(*(**weight_derivatives_values_master_pointer + ewdv_neuron_index) + ewdv_input_index*outputs_count + src_output_index) = src_node_error_contribution;
}



double Weight_node::getWeightedInput()
{
	return input_node_pointer->getInput(current_weight_time) * *(*(**weight_values_master_pointer + ewv_neuron_index) + ewv_input_index);
}



double Weight_node::getBias()
{
	return *(*(**weight_values_master_pointer + ewv_neuron_index) + ewv_input_index);
}



void Weight_node::makeExternalWeigthValue(double **** src_weight_values_master_pointer = NULL, double **** src_weight_derivatives_values_master_pointer = NULL)
{
	if (src_weight_values_master_pointer)
	{
		ewv_neuron_index = neuron_index;
		ewv_input_index = input_index;

		weight_values_master_pointer = src_weight_values_master_pointer;
		*(*(**weight_values_master_pointer + ewv_neuron_index) + ewv_input_index) = *self_weight_value;

		free(self_weight_value);
		self_weight_value = NULL;
	}

	if (src_weight_derivatives_values_master_pointer)
	{
		ewdv_neuron_index = neuron_index;
		ewdv_input_index = input_index;
		weight_derivatives_values_master_pointer = src_weight_derivatives_values_master_pointer;
		
		memcpy(*(**weight_derivatives_values_master_pointer + ewdv_neuron_index) + ewdv_input_index*outputs_count, self_weight_derivatives_values, outputs_count * sizeof(double));
		free(self_weight_derivatives_values);
		self_weight_derivatives_values = NULL;
	}
}



void Weight_node::makeInternalWeightValue(const bool make_weight_values_internal = true, const bool make_weight_derivatives_values_internal = true)
{
	if (make_weight_values_internal)
	{
		self_weight_value = (double*) malloc(sizeof(double));
		
		*self_weight_value = *(*(**weight_values_master_pointer + ewv_neuron_index) + ewv_input_index);

		weight_values_master_pointer = &self_network_weight_values;
		
		ewv_neuron_index = 0;
		ewv_input_index = 0;
	}

	if (make_weight_derivatives_values_internal)
	{
		self_weight_derivatives_values = (double*)malloc(outputs_count * sizeof(double));
		
		memcpy(self_weight_derivatives_values, *(**weight_derivatives_values_master_pointer + ewdv_neuron_index) + ewdv_input_index*outputs_count, outputs_count * sizeof(double));

		weight_derivatives_values_master_pointer = &self_network_weight_derivatives_values;

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
	fprintf(fp_network_data, "\t\t<Bias value = \"%.63f\"></Bias>\n", *(*(**weight_values_master_pointer + ewv_neuron_index) + ewv_input_index));
}



void Weight_node::dumpWeightDataNeuron(FILE * fp_network_data)
{
	/* Print weight value into the file: */
	fprintf(fp_network_data, "\t\t<Weight value = \"%.63f\" input_connection = \"Neuron\" input_position = \"%i\"></Weight>\n",
		*(*(**weight_values_master_pointer + ewv_neuron_index) + ewv_input_index),
		input_node_pointer->getGlobalInputIndex());
}



void Weight_node::dumpWeightDataPattern(FILE * fp_network_data)
{
	/* Print weight value into the file: */
	fprintf(fp_network_data, "\t\t<Weight value = \"%.63f\" input_connection = \"Pattern\" input_position = \"%i\"></Weight>\n",
		*(*(**weight_values_master_pointer + ewv_neuron_index) + ewv_input_index),
		input_node_pointer->getGlobalInputIndex());
}