#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "ArtificialNeuralNetwork.h"


ArtificialNeuralNetwork::ArtificialNeuralNetwork()
{
	inputs_count = 0;
	outputs_count = 0;
	neurons_count = 0;
	weights_count = 0;

	network_current_time = 1;

	network_input_nodes = NULL;
	network_neurons = NULL;
	network_output_nodes = NULL;
	
	input_pattern_master_pointer = NULL;

	sprintf(ann_log_filename, "NULL");
}



ArtificialNeuralNetwork::ArtificialNeuralNetwork(const ArtificialNeuralNetwork & src_ann)
{
	this->inputs_count = src_ann.inputs_count;
	this->outputs_count = src_ann.outputs_count;
	this->neurons_count = src_ann.neurons_count;
	this->weights_count = src_ann.weights_count;
	
	this->network_input_nodes = (Input_pattern**) malloc(this->inputs_count * sizeof(Input_pattern*));
	this->network_neurons = (Neuron**) malloc(this->neurons_count * sizeof(Neuron*));
	this->network_output_nodes = (Neuron**) malloc(this->outputs_count * sizeof(Neuron*));

	for (unsigned int input_index = 0; input_index < this->inputs_count; input_index++)
	{
		*(this->network_input_nodes + input_index) = new Input_pattern(input_index);
		(*(this->network_input_nodes + input_index))->setInputPointer(&input_pattern_master_pointer);
	}

	for (unsigned int neuron_index = 0; neuron_index < this->inputs_count; neuron_index++)
	{
		*(this->network_neurons + neuron_index) = new Neuron(*(*(src_ann.network_neurons + neuron_index)));
	}

	/* Redirect the pointers to the current network's neurons */
	for (unsigned int neuron_index = 0; neuron_index < this->neurons_count; neuron_index++)
	{
		const unsigned int current_inputs_count = (*(this->network_neurons + neuron_index))->getInputsCount();

		for (unsigned int weighted_input_index = 0; weighted_input_index < current_inputs_count; weighted_input_index++)
		{
			/* Get the current's global input index in the source's network */
			const int current_input_global_index = (*(this->network_neurons + neuron_index))->getInputNodeGlobalIndex(weighted_input_index);

			/* The bias will have a NULL pointer, for the other weighted inputs, it is changed */
			if (current_input_global_index >= 0)
			{
				/* Change the pointer to this network's neuron */
				(*(this->network_neurons + neuron_index))->setInputNodePointer(*(this->network_neurons + current_input_global_index), weighted_input_index);
			}
		}
	}

	for (unsigned int output_index = 0; output_index < this->outputs_count; output_index++)
	{
		const unsigned int current_neuron_position = (*(src_ann.network_output_nodes + output_index))->getGlobalNodeIndex();
		*(this->network_output_nodes + output_index) = *(this->network_neurons + current_neuron_position);
	}


	this->input_pattern_master_pointer = src_ann.input_pattern_master_pointer;

	this->network_current_time = 1;
}



ArtificialNeuralNetwork & ArtificialNeuralNetwork::operator=(const ArtificialNeuralNetwork & src_ann)
{
	if (this != &src_ann)
	{
		this->inputs_count = src_ann.inputs_count;
		this->outputs_count = src_ann.outputs_count;
		this->neurons_count = src_ann.neurons_count;
		this->weights_count = src_ann.weights_count;
		
		this->network_input_nodes = (Input_pattern**)malloc(this->inputs_count * sizeof(Input_pattern*));
		this->network_neurons = (Neuron**)malloc(this->neurons_count * sizeof(Neuron*));
		this->network_output_nodes = (Neuron**)malloc(this->outputs_count * sizeof(Neuron*));

		for (unsigned int input_index = 0; input_index < this->inputs_count; input_index++)
		{
			*(this->network_input_nodes + input_index) = new Input_pattern(input_index);
			(*(this->network_input_nodes + input_index))->setInputPointer(&input_pattern_master_pointer);
		}

		for (unsigned int neuron_index = 0; neuron_index < this->neurons_count; neuron_index++)
		{
			*(this->network_neurons + neuron_index) = new Neuron(*(*(src_ann.network_neurons + neuron_index)));
		}

		/* Redirect the pointers to the current network's neurons */
		for (unsigned int neuron_index = 0; neuron_index < this->neurons_count; neuron_index++)
		{
			const unsigned int current_inputs_count = (*(this->network_neurons + neuron_index))->getInputsCount();

			for (unsigned int weighted_input_index = 0; weighted_input_index < current_inputs_count; weighted_input_index++)
			{
				/* Get the current's global input index in the source's network */
				const int current_input_global_index = (*(this->network_neurons + neuron_index))->getInputNodeGlobalIndex(weighted_input_index);

				/* The bias will have a negative global index, for the other weighted inputs, it is changed */
				if (current_input_global_index >= 0)
				{
					/* Change the pointer to this network's neuron */
					(*(this->network_neurons + neuron_index))->setInputNodePointer(*(this->network_neurons + current_input_global_index), weighted_input_index);
				}
			}
		}

		for (unsigned int output_index = 0; output_index < this->outputs_count; output_index++)
		{
			const unsigned int current_neuron_position = (*(src_ann.network_output_nodes + output_index))->getGlobalNodeIndex();
			*(this->network_output_nodes + output_index) = *(this->network_neurons + current_neuron_position);
		}

		this->input_pattern_master_pointer = src_ann.input_pattern_master_pointer;

		this->network_current_time = 1;
	}

	return *this;
}



ArtificialNeuralNetwork::~ArtificialNeuralNetwork()
{
	if (inputs_count > 0)
	{
		for (unsigned int i = 0; i < inputs_count; i++)
		{
			delete (*(network_input_nodes + i));
		}
		free(network_input_nodes);
	}

	if (neurons_count > 0)
	{
		unsigned int weight_and_bias_position = 0;
		for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
		{
			delete (*(network_neurons + neuron_index));
		}
		free(network_neurons);
	}

	if (outputs_count > 0)
	{
		free(network_output_nodes);
	}
}



// Prints the current solution (a 2D image with the hyperplanes that separates the data is intended)
void ArtificialNeuralNetwork::printSolution()
{
}



void ArtificialNeuralNetwork::addInputNode(const unsigned int src_input_position)
{
	if (inputs_count > 0)
	{
		Input_pattern ** swap_input_nodes = network_input_nodes;
		network_input_nodes = (Input_pattern **)malloc((inputs_count + 1) * sizeof(Input_pattern*));
		memmove(network_input_nodes, swap_input_nodes, inputs_count * sizeof(Input_pattern*));
		free(swap_input_nodes);
	}
	else
	{
		network_input_nodes = (Input_pattern **)malloc(sizeof(Input_pattern*));
	}
	*(network_input_nodes + inputs_count) = new Input_pattern(src_input_position);
	(*(network_input_nodes + inputs_count))->setInputPointer(&input_pattern_master_pointer);

	inputs_count++;
}



void ArtificialNeuralNetwork::addNeuron()
{
	if (neurons_count > 0)
	{
		Neuron ** swap_network_neurons = network_neurons;
		network_neurons = (Neuron **)malloc((neurons_count + 1) * sizeof(Neuron*));
		memmove(network_neurons, swap_network_neurons, neurons_count * sizeof(Neuron*));
		free(swap_network_neurons);
	}
	else
	{
		network_neurons = (Neuron **)malloc(sizeof(Neuron*));
	}

	*(network_neurons + neurons_count) = new Neuron(neurons_count, outputs_count);

	neurons_count++;
}



void ArtificialNeuralNetwork::addOutputNode(const unsigned int src_neuron_position)
{
	if (outputs_count > 0)
	{
		Neuron ** swap_output_nodes = network_output_nodes;
		network_output_nodes = (Neuron **)malloc((outputs_count + 1) * sizeof(Neuron*));
		memmove(network_output_nodes, swap_output_nodes, outputs_count * sizeof(Neuron*));
		free(swap_output_nodes);
	}
	else
	{
		network_output_nodes = (Neuron**)malloc(sizeof(Neuron*));
	}

	*(network_output_nodes + outputs_count) = *(network_neurons + src_neuron_position);
	outputs_count++;
}



void ArtificialNeuralNetwork::setNetworkWeights(double *** src_weights_and_bias)
{
	for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
	{
		(*(network_neurons + neuron_index))->makeExternalWeightValues(src_weights_and_bias, NULL);
	}
}



void ArtificialNeuralNetwork::setNetworkWeightsDerivatives(double **** src_weights_and_bias_derivatives)
{
	for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
	{
		(*(network_neurons + neuron_index))->makeExternalWeightValues(NULL, src_weights_and_bias_derivatives);
	}
}



void ArtificialNeuralNetwork::setNetworkLogFilename(const char * src_filename)
{
	sprintf(ann_log_filename, "%s", src_filename);
}



void ArtificialNeuralNetwork::loadNetworkData(const char * src_filename)
{
	using namespace rapidxml;
	using namespace std;
	xml_document<> doc;
	xml_node<> * root_node;

	// Read the xml file into a vector
	ifstream theFile(src_filename);
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');

	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);

	// Find our root node
	root_node = doc.first_node("NeuralNetwork");

	// Iterate over the Outputs to know how many output nodes exists in the network
	for (xml_node<> * output_node = root_node->first_node("Output"); output_node; output_node = output_node->next_sibling())
	{
		if (strcmp(output_node->name(), "Output") != 0)
		{
			continue;
		}
		outputs_count++;
	}

	// Iterate over the Inputs
	for (xml_node<> * input_node = root_node->first_node("Input"); input_node; input_node = input_node->next_sibling())
	{
		if (strcmp(input_node->name(), "Input") != 0)
		{
			continue;
		}
		const int pointer_position_temp = atoi(input_node->first_attribute("position")->value());
		this->addInputNode(pointer_position_temp);
	}

	// Iterate over the Neurons
	for (xml_node<> * neuron_node = root_node->first_node("Neuron"); neuron_node; neuron_node = neuron_node->next_sibling())
	{
		if (strcmp(neuron_node->name(), "Neuron") != 0)
		{
			continue;
		}

		this->addNeuron();

		const int neuron_position = atoi(neuron_node->first_attribute("position")->value());

		xml_node<> * activation_function_node = neuron_node->first_node("ActivationFunction");
		char * activation_function_name = activation_function_node->first_attribute("type")->value();

		ActivationFunctionBase::ACTIVATION_FUNCTION_TYPE activations_function_type;
		if (strcmp(activation_function_name, "ACT_IDENTITY") == 0)
		{
			activations_function_type = ActivationFunctionBase::ACT_IDENTITY;
		}
		else if (strcmp(activation_function_name, "ACT_SIGMOID") == 0)
		{
			activations_function_type = ActivationFunctionBase::ACT_SIGMOID;
		}
		else // ACT_HYPERBOLIC_TANGENT
		{
			activations_function_type = ActivationFunctionBase::ACT_HYPERBOLIC_TANGENT;
		}

		unsigned int parameters_count = 0;
		double * function_parameters = NULL;
		for (xml_node<> * parameter_node = activation_function_node->first_node("Parameter"); parameter_node; parameter_node = parameter_node->next_sibling(), parameters_count++)
		{
			if (parameters_count > 0)
			{
				double * swap_function_parameters = function_parameters;
				function_parameters = (double*)malloc((parameters_count + 1) * sizeof(double));
				memmove(function_parameters, swap_function_parameters, parameters_count * sizeof(double));
				free(swap_function_parameters);
			}
			else
			{
				function_parameters = (double*)malloc(sizeof(double));
			}
			double parameter_value_temp = atof(parameter_node->first_attribute("value")->value());
			*(function_parameters + parameters_count) = parameter_value_temp;
		}

		(*(network_neurons + neuron_position))->setActivationFunction(activations_function_type, function_parameters);
		free(function_parameters);
		
		xml_node<> * bias_node = neuron_node->first_node("Bias");
		(*(network_neurons + neuron_position))->addWeightedInput(Weight_node::WIT_BIAS, NULL, atof(bias_node->first_attribute("value")->value()));
		weights_count++;

		for (xml_node<> * weight_node = neuron_node->first_node("Weight"); weight_node; weight_node = weight_node->next_sibling())
		{
			if (strcmp(weight_node->name(), "Weight") != 0)
			{
				continue;
			}

			const double weight_value_temp = atof(weight_node->first_attribute("value")->value());

			const int input_position_temp = atoi(weight_node->first_attribute("input_position")->value());

			char * input_connection_temp = weight_node->first_attribute("input_connection")->value();
			
			if (strcmp(input_connection_temp, "Pattern") == 0)
			{
				(*(network_neurons + neuron_position))->addWeightedInput(
					Weight_node::WIT_PATTERN,
					*(network_input_nodes + input_position_temp),
					weight_value_temp);
			}
			else // Neuron
			{
				(*(network_neurons + neuron_position))->addWeightedInput(
					Weight_node::WIT_NEURON,
					*(network_neurons + input_position_temp),
					weight_value_temp);
			}
			weights_count++;
		}
	}

	outputs_count = 0;
	// Iterate over the Outputs to assign the output neurons
	for (xml_node<> * output_node = root_node->first_node("Output"); output_node; output_node = output_node->next_sibling())
	{
		if (strcmp(output_node->name(), "Output") != 0)
		{
			continue;
		}

		const int neuron_position_temp = atoi(output_node->first_attribute("input_position")->value());
		this->addOutputNode(neuron_position_temp);
	}
}



// Saves the current state of the network to a file
void ArtificialNeuralNetwork::saveNetworkState()
{
	if (strcmp(ann_log_filename, "NULL") == 0)
	{
		printf("<Error: Network log file has not been set yet>\n");
		return;
	}
	
	FILE *fp_log = fopen(ann_log_filename, "w");
	fprintf(fp_log, "<?xml version = \"1.0\" encoding = \"utf-8\"?>\n<NeuralNetwork>\n");

	// Print the inputs of the network:
	for (unsigned int input_index = 0; input_index < inputs_count; input_index++)
	{
		(*(network_input_nodes + input_index))->dumpNodeData(fp_log);
	}

	// Print the neurons with their respective weights and activation functions:
	for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
	{
		(*(network_neurons + neuron_index))->dumpNodeData(fp_log);
	}

	// Print the inputs of the network:
	for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
	{
		fprintf(fp_log, "\t<Output input_position=\"%i\"></Output>\n", (*(network_output_nodes + output_index))->getGlobalNodeIndex());
	}

	fprintf(fp_log, "</NeuralNetwork>");
	fclose(fp_log);
}



void ArtificialNeuralNetwork::setInputPatternPointer(double * src_input_pattern_pointer)
{
	input_pattern_master_pointer = src_input_pattern_pointer;
}



void ArtificialNeuralNetwork::predict(double * src_input_pattern_pointer, double * dst_prediction)//, const double threshold)
{
	// Assign the input pattern master pointer to the testing array pointer
	input_pattern_master_pointer = src_input_pattern_pointer;

	// Save the output to the destination prediction array
	for (unsigned int i = 0; i < outputs_count; i++)
	{
		*(dst_prediction + i) = (*(network_output_nodes + i))->getInput(network_current_time);
	}

	network_current_time++;
}



void ArtificialNeuralNetwork::predictWithDerivatives(double * src_input_pattern_pointer, double * dst_prediction)
{
	// Assign the input pattern master pointer to the testing array pointer
	input_pattern_master_pointer = src_input_pattern_pointer;

	// Save the output to the destination prediction array
	for (unsigned int i = 0; i < outputs_count; i++)
	{
		*(dst_prediction + i) = (*(network_output_nodes + i))->getInputWithDerivatives(network_current_time);
	}

	network_current_time++;
}


Input_node * ArtificialNeuralNetwork::getOutputNode(const unsigned int src_output_index)
{
	return *(network_output_nodes + src_output_index);
}



void ArtificialNeuralNetwork::resetNetworkTime()
{
	for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
	{
		(*(network_neurons + neuron_index))->resetNodeCurrentTime();
	}
	network_current_time = 1;
}
