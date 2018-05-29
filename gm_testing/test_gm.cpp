#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../src/Neuron.h"
#include "../src/Input_pattern_class.h"
#include "../src/LossFunction_class.h"
#include "../src/L2LossFunction_class.h"

#include "../src/random_numbers_generator.h"
#include "../src/data_handler_class.h"
#include "../src/gradientMethods_class.h"
#include "../src/args_fercer.h"

int main(int argc, char * argv[])
{
	ARGUMENTS my_args;
	my_args.setArgc(argc);
	my_args.setArgv(argv);

	my_args.newArgument("Training data filename", "dr", "dataset-training", "NULL", false);
	my_args.newArgument("Testing data filename", "de", "dataset-testing", "NULL", true);
	my_args.newArgument("Iterations", "i", "iterations", 50, true);
	my_args.newArgument("First layer neurons count", "fl", "first-layer", 0, true);
	
	my_args.showHelp();

	data_handler training_data;
	training_data.setFilename(my_args.getArgumentCHAR("-dr"));
	training_data.loadData();

	const unsigned int inputs_count = training_data.getInputSize();
	const unsigned int outputs_count = training_data.getOutputSize();
	const unsigned int first_layer = my_args.getArgumentINT("-fl");
	const unsigned int neurons_count = first_layer + outputs_count;
	
	//const unsigned int variables_count = (inputs_count + 1)*first_layer + (first_layer + 1) * outputs_count;
	const unsigned int variables_count = (inputs_count + 1)*outputs_count;

	gradientMethods gradient_method;
	//gradient_method.setGradientMethod(gradientMethods::GRADIENT_DESCENT);
	gradient_method.setGradientMethod(gradientMethods::LEVENBERG_MARQUARDT);
	//gradient_method.setGradientMethod(gradientMethods::MINI_BATCH_GRADIENT_DESCENT);
	//gradient_method.setGradientMethod(gradientMethods::STOCHASTIC_GRADIENT_DESCENT);
	//gradient_method.setGradientMethod(gradientMethods::MINI_BATCH_STOCHASTIC_GRADIENT_DESCENT);

	gradient_method.setOutputsCount(outputs_count);
	gradient_method.setTrainingDataSize(training_data.getDataSize());
	gradient_method.setVariablesCount(variables_count);
	
	double * variables = (double*)malloc(variables_count * sizeof(double));
	double * outputs_derivatives = (double*)malloc(outputs_count*sizeof(double));
	double ** derivatives = (double**)malloc(variables_count * sizeof(double*));

	STAUS * my_seed = initSeed(0);

	for (unsigned int var_i = 0; var_i < variables_count; var_i++)
	{
		*(variables + var_i) = anorm_est(my_seed);
		*(derivatives + var_i) = (double*)malloc(outputs_count*sizeof(double));
	}
	*(variables) = 0.0;


	const unsigned int max_iterations = my_args.getArgumentINT("-i");

	gradient_method.setLearningRate(0.05);
	gradient_method.setMomentum(0.9);

	gradient_method.setVariablesDerivativesPointersManager(derivatives);
	gradient_method.setVariablesValuesPointerManager(variables);
	gradient_method.setOutputsDerivativesPointerManager(outputs_derivatives);
	gradient_method.setBatchSize(training_data.getDataSize() / 5);


	// Assembly the network 2 - 1
	int * groundtruth_master_pointer;
	double * input_pattern_master_pointer;

	double ** variables_values_neurons_pointers = (double**)malloc(neurons_count * sizeof(double*));
	double *** variables_derivatives_neurons_pointers = (double***)malloc(neurons_count * sizeof(double**));
	
	Input_pattern ** network_inputs = (Input_pattern**)malloc(inputs_count * sizeof(Input_pattern*));
	Neuron ** network_neurons = (Neuron**)malloc(neurons_count * sizeof(Neuron*));
	Neuron ** network_outputs = (Neuron**)malloc(outputs_count * sizeof(Neuron*));
	LossFunction ** loss_functions = (LossFunction**)malloc(outputs_count * sizeof(LossFunction*));


	for (unsigned int input_index = 0; input_index < inputs_count; input_index++)
	{
		*(network_inputs + input_index) = new Input_pattern(input_index);
		(*(network_inputs + input_index))->setInputPointer(&input_pattern_master_pointer);
	}

	for (unsigned int neuron_index = 0; neuron_index < first_layer; neuron_index++)
	{
		*(variables_derivatives_neurons_pointers + neuron_index) = derivatives + (inputs_count + 1) * neuron_index;
		*(variables_values_neurons_pointers + neuron_index) = variables + (inputs_count + 1) * neuron_index;

		*(network_neurons + neuron_index) = new Neuron(neuron_index, outputs_count, &variables_values_neurons_pointers, &variables_derivatives_neurons_pointers);

		(*(network_neurons + neuron_index))->addWeightedInput(Weight_node::WIT_BIAS, NULL, 0.0);
		for (unsigned int input_index = 0; input_index < inputs_count; input_index++)
		{
			(*(network_neurons + neuron_index))->addWeightedInput(Weight_node::WIT_PATTERN, *(network_inputs + input_index), 0.0);
		}
		(*(network_neurons + neuron_index))->setActivationFunction(ActivationFunctionBase::ACT_SIGMOID, NULL);
	}

	for (unsigned int neuron_index = 0; neuron_index < outputs_count; neuron_index++)
	{
		/*
		*(variables_derivatives_neurons_pointers + neuron_index + first_layer) = derivatives + (inputs_count + 1) * first_layer + (first_layer + 1) * neuron_index;
		*(variables_values_neurons_pointers + neuron_index + first_layer) = variables + (inputs_count + 1) * first_layer + (first_layer + 1) * neuron_index;
		*/
		*(variables_derivatives_neurons_pointers + neuron_index + first_layer) = derivatives + (inputs_count + 1) * neuron_index;
		*(variables_values_neurons_pointers + neuron_index + first_layer) = variables + (inputs_count + 1) * neuron_index;

		*(network_neurons + neuron_index + first_layer) = new Neuron(neuron_index + first_layer, outputs_count, &variables_values_neurons_pointers, &variables_derivatives_neurons_pointers);
		
		(*(network_neurons + neuron_index + first_layer))->addWeightedInput(Weight_node::WIT_BIAS, NULL, 0.0);
		for (unsigned int previous_layer_neuron_index = 0; previous_layer_neuron_index < first_layer; previous_layer_neuron_index++)
		{
			(*(network_neurons + neuron_index + first_layer))->addWeightedInput(Weight_node::WIT_NEURON, *(network_neurons + previous_layer_neuron_index), 0.0);
		}
		
		for (unsigned int previous_layer_neuron_index = 0; previous_layer_neuron_index < inputs_count; previous_layer_neuron_index++)
		{
			(*(network_neurons + neuron_index + first_layer))->addWeightedInput(Weight_node::WIT_PATTERN, *(network_inputs + previous_layer_neuron_index), 0.0);
		}
		
		*(network_outputs + neuron_index) = *(network_neurons + neuron_index + first_layer);
		(*(network_neurons + neuron_index + first_layer))->setActivationFunction(ActivationFunctionBase::ACT_SIGMOID, NULL);

		*(loss_functions + neuron_index) = new L2LossFunction;
		(*(loss_functions + neuron_index))->setGlobalOutputIndex(neuron_index);
		(*(loss_functions + neuron_index))->setOutputNode(*(network_outputs + neuron_index));
		(*(loss_functions + neuron_index))->setGroundtruth(&groundtruth_master_pointer);
		(*(loss_functions + neuron_index))->makeDerivativeExternal(outputs_derivatives);
	}

	double previous_error = training_data.getDataSize();
	unsigned long long network_time = 1;
	for (unsigned int iteration = 0; iteration < max_iterations; iteration++)
	{
		double mean_error = 0.0;
		for (unsigned int pattern_index = 0; pattern_index < training_data.getDataSize(); pattern_index++, network_time++)
		{
			input_pattern_master_pointer = *(training_data.getInputData() + pattern_index);
			groundtruth_master_pointer = *(training_data.getOutputData() + pattern_index);

			// Feed forward:
			for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
			{
				mean_error += (*(loss_functions + output_index))->computeLossWithDerivatives(network_time);
				(*(network_outputs + output_index))->addNodeErrorContribution(1.0, output_index);
			}

			// Backpropagation:
			for (int neuron_index = (neurons_count - 1); neuron_index >= 0; neuron_index--)
			{
				(*(network_neurons + neuron_index))->backpropagateNodeError();
			}
			gradient_method.updateDeltasValues();
		}
		mean_error /= (double)training_data.getDataSize();
		const double squared_gradient_norm = gradient_method.updateVariablesValues();

		printf("MSE = %f, previous MSE = %f\n", mean_error, previous_error);
		while (!gradient_method.confirmDescent(previous_error - mean_error))
		{
			mean_error = 0.0;
			for (unsigned int pattern_index = 0; pattern_index < training_data.getDataSize(); pattern_index++, network_time++)
			{
				input_pattern_master_pointer = *(training_data.getInputData() + pattern_index);
				groundtruth_master_pointer = *(training_data.getOutputData() + pattern_index);
				for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
				{
					mean_error += (*(loss_functions + output_index))->computeLoss(network_time);
				}
			}
			mean_error /= (double)training_data.getDataSize();
			printf("MSE = %f\n", mean_error);
		}
		previous_error = mean_error;
	}

	for (unsigned int pattern_index = 0; pattern_index < training_data.getDataSize(); pattern_index++, network_time++)
	{
		input_pattern_master_pointer = *(training_data.getInputData() + pattern_index);

		const double var_1 = *(input_pattern_master_pointer);
		const double var_2 = *(input_pattern_master_pointer + 1);
		const int groundtruth = *(*(training_data.getOutputData() + pattern_index));

		const double prediction = (*(network_outputs))->getInput(network_time);

		printf("[%i]\t%f\t%f\t-> %f | predicted = %f | class %i\n", pattern_index, var_1, var_2, prediction, (prediction > 0.5) ? 1.0 : 0.0, groundtruth);
	}
	
	for (unsigned int var_i = 0; var_i < 3; var_i++)
	{
		printf("var[%i] = %f\n", var_i, *(variables + var_i));
		free(*(derivatives + var_i));
	}
	free(derivatives);
	free(variables);
	free(outputs_derivatives);
	free(my_seed);

	return 0;
}