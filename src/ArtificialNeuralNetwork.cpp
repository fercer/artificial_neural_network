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

	network_weights_allocated = false;
	network_weights_derivatives_allocated = false;

	network_weights_derivative_values = NULL;
	network_weights_values = NULL;

	sprintf(ann_log_filename, "NULL");
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

	if (network_weights_allocated)
	{
		free(network_weights_values);
	}

	if (network_weights_derivatives_allocated)
	{
		for (unsigned int weights_and_bias_index = 0; weights_and_bias_index < (weights_count + neurons_count); weights_and_bias_index++)
		{
			free(*(network_weights_derivative_values + weights_and_bias_index));
		}
		free(network_weights_derivative_values);
	}
}



// Checks if the network is readyto be run
bool ArtificialNeuralNetwork::checkNetworkState()
{
	return true;
}



// Prints the current solution (a 2D image with the hyperplanes that separates the data is intended)
void ArtificialNeuralNetwork::printSolution()
{
}



void ArtificialNeuralNetwork::allocateNetworkMemory()
{
	if (neurons_count == 0 || weights_count == 0)
	{
		return;
	}
	
	if (!network_weights_allocated)
	{
		network_weights_values = (double*)malloc((neurons_count + weights_count) * sizeof(double));
		network_weights_allocated = true;
	}

	if (!network_weights_derivatives_allocated)
	{
		// Allocate memory for the weights error contribution array:
		network_weights_derivative_values = (double**)malloc((neurons_count + weights_count) * sizeof(double*));
		for (unsigned int weight_and_bias_index = 0; weight_and_bias_index < (neurons_count + weights_count); weight_and_bias_index++)
		{
			*(network_weights_derivative_values + weight_and_bias_index) = (double*)malloc(outputs_count * sizeof(double));
		}

		// Memory successfully allocated
		network_weights_derivatives_allocated = true;
	}
}



void ArtificialNeuralNetwork::dumpWeightsToNetwork()
{
	unsigned int weight_and_bias_position = 0;
	for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
	{
		const unsigned int current_neuron_inputs_count = (*(network_neurons + neuron_index))->getInputsCount();
		for (unsigned int intput_index = 0; intput_index <= current_neuron_inputs_count; intput_index++)
		{
			*(network_weights_values + weight_and_bias_position + intput_index) = (*(network_neurons + neuron_index))->getWeightValue(intput_index);
		}

		(*(network_neurons + neuron_index))->assignExternalWeightErrorContributionPointer(network_weights_derivative_values + weight_and_bias_position);
		(*(network_neurons + neuron_index))->assignExternalWeightPointer(network_weights_values + weight_and_bias_position);
		weight_and_bias_position += current_neuron_inputs_count + 1;
	}
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
	*(network_input_nodes + inputs_count) = new Input_pattern;
	(*(network_input_nodes + inputs_count))->setInputPointerPosition(src_input_position);

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

	// Preevent the neuron to allocate memory for the error contribution array, and use the defined by this network:
	(*(network_neurons + neurons_count))->assignExternalWeightErrorContributionPointer(NULL);

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



void ArtificialNeuralNetwork::setNetworkWeights(double * src_weights_and_bias, const bool deep_copy)
{
	if (!deep_copy)
	{
		if (network_weights_allocated)
		{
			free(network_weights_values);
			network_weights_allocated = false;
		}
		network_weights_values = src_weights_and_bias;
		return;
	}

	allocateNetworkMemory();
	memcpy(network_weights_values, src_weights_and_bias, (weights_count + neurons_count) * sizeof(double));
}



void ArtificialNeuralNetwork::setNetworkWeightsDerivatives(double ** src_weights_and_bias_derivatives, const bool deep_copy)
{
	if (!deep_copy)
	{
		if (network_weights_derivatives_allocated)
		{
			free(network_weights_derivative_values);
			network_weights_derivatives_allocated = false;
		}
		network_weights_derivative_values = src_weights_and_bias_derivatives;
		return;
	}

	allocateNetworkMemory();
	for (unsigned int weighs_and_bias_index = 0; weighs_and_bias_index < (weights_count + neurons_count); weighs_and_bias_index++)
	{
		memcpy(*(network_weights_derivative_values + weighs_and_bias_index), *(src_weights_and_bias_derivatives + weighs_and_bias_index), outputs_count * sizeof(double));
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

	// Iterate over the Output
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

		Neuron::ACTIVATION_FUNCTION_TYPE activations_function_type;
		if (strcmp(activation_function_name, "ACT_IDENTITY") == 0)
		{
			activations_function_type = Neuron::ACT_IDENTITY;
		}
		else if (strcmp(activation_function_name, "ACT_SIGMOID") == 0)
		{
			activations_function_type = Neuron::ACT_SIGMOID;
		}
		else // ACT_HYPERBOLIC_TANGENT
		{
			activations_function_type = Neuron::ACT_HYPERBOLIC_TANGENT;
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
		const double bias_value_temp = atof(bias_node->first_attribute("value")->value());
		(*(network_neurons + neuron_position))->updateWeightValue(bias_value_temp, 0);

		for (xml_node<> * weight_node = neuron_node->first_node("Weight"); weight_node; weight_node = weight_node->next_sibling())
		{
			if (strcmp(weight_node->name(), "Weight") != 0)
			{
				continue;
			}

			const double weight_value_temp = atof(weight_node->first_attribute("value")->value());

			const int input_position_temp = atoi(weight_node->first_attribute("input_position")->value());

			char * input_connection_temp = weight_node->first_attribute("input_connection")->value();

			weights_count++;

			if (strcmp(input_connection_temp, "Pattern") == 0)
			{
				(*(network_neurons + neuron_position))->addInputNode(*(network_input_nodes + input_position_temp), weight_value_temp, Weight_node::WIT_PATTERN, input_position_temp);
			}
			else // Neuron
			{
				(*(network_neurons + neuron_position))->addInputNode(*(network_neurons + input_position_temp), weight_value_temp, Weight_node::WIT_NEURON, input_position_temp);
			}
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

	allocateNetworkMemory();

	dumpWeightsToNetwork();
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
		fprintf(fp_log, "\t<Input position = \"%i\"></Input>\n",(*(network_input_nodes + input_index))->getInputPointerPosition());
	}

	// Print the neurons with their respective weights and activation functions:
	for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
	{
		fprintf(fp_log, "\t<Neuron position = \"%i\">\n", neuron_index);
		fprintf(fp_log, "\t\t<Bias value = \"%.63f\"></Bias>\n", (*(network_neurons + neuron_index))->getWeightValue(0));
		for (unsigned int neuron_input_index = 1; neuron_input_index <= (*(network_neurons + neuron_index))->getInputsCount(); neuron_input_index++)
		{
			fprintf(fp_log, "\t\t<Weight value = \"%.63f\" input_connection = ",
			(*(network_neurons + neuron_index))->getWeightValue(neuron_input_index));

			switch ((*(network_neurons + neuron_index))->getInputType(neuron_input_index))
			{
			case Weight_node::WIT_NEURON:
				fprintf(fp_log, "\"Neuron\"");
				break;
			case Weight_node::WIT_PATTERN:
				fprintf(fp_log, "\"Pattern\"");
				break;
			}

			fprintf(fp_log, " input_position = \"%i\"></Weight>\n",
				(*(network_neurons + neuron_index))->getInputPosition(neuron_input_index));
		}

		// Print the activation function type:
		fprintf(fp_log, "\t\t<ActivationFunction type = ");
		switch ((*(network_neurons + neuron_index))->getActivationFunctionType())
		{
		case Neuron::ACT_SIGMOID:
			fprintf(fp_log, "\"ACT_SIGMOID\">\n");
			break;

		case Neuron::ACT_IDENTITY:
			fprintf(fp_log, "\"ACT_IDENTITY\">\n");
			break;

		case Neuron::ACT_HYPERBOLIC_TANGENT:
			fprintf(fp_log, "\"ACT_HYPERBOLIC_TANGENT\">\n");
			break;
		}

		// Print the activation function's parameters:
		for (unsigned int parameter_index = 0; parameter_index < (*(network_neurons + neuron_index))->getActivationFunctionParametersCount(); parameter_index++)
		{
			fprintf(fp_log, "\t\t\t<Parameter value = \"%.63f\"></Parameter>\n", 
				(*(network_neurons + neuron_index))->getActivationFunctionParameterValue(parameter_index));
		}
		fprintf(fp_log, "\t\t</ActivationFunction>\n");
		fprintf(fp_log, "\t</Neuron>\n");
	}

	// Print the inputs of the network:
	for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
	{
		fprintf(fp_log, "\t<Output input_position=\"%i\"></Output>\n", (*(network_output_nodes + output_index))->getNeuronPosition());
	}

	fprintf(fp_log, "</NeuralNetwork>");
	fclose(fp_log);
}



void ArtificialNeuralNetwork::predict(double * test_input, double * dst_prediction)//, const double threshold)
{
	// Assign the inputs to the testing array pointer
	for (unsigned int i = 0; i < inputs_count; i++)
	{
		(*(network_input_nodes + i))->setInputPointer(test_input);
	}

	// Save the output to the destination prediction array
	for (unsigned int i = 0; i < outputs_count; i++)
	{
		*(dst_prediction + i) = (*(network_output_nodes + i))->getInput(network_current_time);
	}

	network_current_time++;
}



Input_node * ArtificialNeuralNetwork::getOutputNode(const unsigned int src_output_index)
{
	return *(network_output_nodes + src_output_index);
}

double ** ArtificialNeuralNetwork::getNetworkWeightsDerivatives()
{
	return network_weights_derivative_values;
}

double * ArtificialNeuralNetwork::getNetworkWeights()
{
	return network_weights_values;
}

void ArtificialNeuralNetwork::resetNetworkTime()
{
	for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
	{
		(*(network_neurons + neuron_index))->resetNodeCurrentTime();
	}
	network_current_time = 1;
}
