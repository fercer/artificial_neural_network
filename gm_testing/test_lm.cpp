#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../src/random_numbers_generator.h"
#include "../src/data_handler_class.h"
#include "gradientMethods_class.h"
#include "../src/args_fercer.h"

#include "register_problem.h"

int main(int argc, char * argv[])
{
	ARGUMENTS my_args;
	my_args.setArgc(argc);
	my_args.setArgv(argv);

	my_args.newArgument("Training data filename", "dr", "dataset-training", "NULL", false);
	my_args.newArgument("Iterations", "i", "iterations", 1000, true);
	
	my_args.showHelp();

	data_handler training_data;
	training_data.setFilename(my_args.getArgumentCHAR("-dr"));
	training_data.loadData();
	
	const unsigned int patterns_count = training_data.getDataSize();
	const unsigned int outputs_count = training_data.getOutputSize();
	const unsigned int variables_count = 3;


	gradientMethods gradient_method;
	//gradient_method.setGradientMethod(gradientMethods::GRADIENT_DESCENT);
	gradient_method.setGradientMethod(gradientMethods::LEVENBERG_MARQUARDT);
	//gradient_method.setGradientMethod(gradientMethods::MINI_BATCH_GRADIENT_DESCENT);
	//gradient_method.setGradientMethod(gradientMethods::STOCHASTIC_GRADIENT_DESCENT);
	//gradient_method.setGradientMethod(gradientMethods::MINI_BATCH_STOCHASTIC_GRADIENT_DESCENT);


	gradient_method.setOutputsCount(outputs_count);
	gradient_method.setTrainingDataSize(patterns_count);
	gradient_method.setVariablesCount(variables_count);
	
	double * variables = (double*)malloc(variables_count * sizeof(double));
	double * outputs_derivatives = (double*)malloc(outputs_count*sizeof(double));
	double ** derivatives = (double**)malloc(variables_count * sizeof(double*));

	STAUS * my_seed = initSeed(0);

	for (unsigned int var_i = 0; var_i < variables_count; var_i++)
	{
		*(derivatives + var_i) = (double*)malloc(outputs_count*sizeof(double));
	}
	*(variables) = 1.4 + anorm(0.0, 0.5, my_seed);
	*(variables + 1) = 1.9 + anorm(0.0, 0.5, my_seed);
	*(variables + 2) = 0.0 + anorm(0.0, 1.0, my_seed);

	const unsigned int max_iterations = my_args.getArgumentINT("-i");

	gradient_method.setLearningRate(0.05);
	gradient_method.setMomentum(0.9);

	gradient_method.setVariablesDerivativesPointersManager(derivatives);
	gradient_method.setVariablesValuesPointerManager(variables);
	gradient_method.setOutputsDerivativesPointerManager(outputs_derivatives);
	gradient_method.setBatchSize(10);
		
	double previous_error = patterns_count*patterns_count;
	
	for (unsigned int iteration = 0; iteration < max_iterations; iteration++)
	{
		double sum_squared_error = 0.0;
		for (unsigned int pattern_index = 0; pattern_index < patterns_count; pattern_index++)
		{
			const double current_input = **(training_data.getInputData() + pattern_index);

			const double prediction = 
				cos(*(variables) * current_input) + sin(*(variables + 1) * current_input) + *(variables + 2);

			const double expected_output = **(training_data.getOutputFloatData() + pattern_index);
			const double difference = expected_output - prediction;
			const double pattern_error = difference * difference / 2.0;

			*(outputs_derivatives) = difference;

			**(derivatives) = sin(*(variables) * current_input) * current_input;
			**(derivatives + 1) = - cos(*(variables+1) * current_input) * current_input;
			**(derivatives + 2) = -1.0;

			gradient_method.updateDeltasValues();

			sum_squared_error += pattern_error;
		}
		double squared_graident_norm = gradient_method.updateVariablesValues();

		printf("[%i/%i] MSE = %f, previous MSE = %f, (%f, %f, %f)\n", iteration+1, max_iterations, sum_squared_error, previous_error, *(variables), *(variables+1), *(variables + 2));
		unsigned int sub_iteration = 0;
		while (!gradient_method.confirmDescent(previous_error - sum_squared_error))
		{
			sum_squared_error = 0.0;
			for (unsigned int pattern_index = 0; pattern_index < patterns_count; pattern_index++)
			{
				const double current_input = **(training_data.getInputData() + pattern_index);
				const double prediction =
					cos(*(variables)* current_input) + sin(*(variables + 1) * current_input) + *(variables + 2);

				const double expected_output = **(training_data.getOutputFloatData() + pattern_index);
				const double difference = expected_output - prediction;
				const double pattern_error = difference * difference / 2.0;
				sum_squared_error += pattern_error;
			}
		}

		if (previous_error > sum_squared_error)
		{
			previous_error = sum_squared_error;
		}

		printf("E = %f, Squared gradient = %f\n", sum_squared_error, squared_graident_norm);
		if (sum_squared_error < 1e-3)
		{
			break;
		}
	}

	for (unsigned int var_i = 0; var_i < variables_count; var_i++)
	{
		printf("Variable[%i] = %f\n", var_i, *(variables + var_i));
		free(*(derivatives + var_i));
	}

	free(variables);
	free(outputs_derivatives);
	free(derivatives);

	return 0;
}