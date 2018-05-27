#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
	my_args.newArgument("Iterations", "i", "iterations", 500, true);
	
	my_args.showHelp();

	data_handler training_data;
	training_data.setFilename(my_args.getArgumentCHAR("-dr"));
	training_data.loadData();
	
	gradientMethods gradient_method;
	gradient_method.setGradientMethod(gradientMethods::GRADIENT_DESCENT);
	//gradient_method.setGradientMethod(gradientMethods::LEVENBERG_MARQUARDT);
	//gradient_method.setGradientMethod(gradientMethods::MINI_BATCH_GRADIENT_DESCENT);
	//gradient_method.setGradientMethod(gradientMethods::STOCHASTIC_GRADIENT_DESCENT);
	//gradient_method.setGradientMethod(gradientMethods::MINI_BATCH_STOCHASTIC_GRADIENT_DESCENT);

	gradient_method.setOutputsCount(training_data.getOutputSize());
	gradient_method.setTrainingDataSize(training_data.getDataSize());
	gradient_method.setVariablesCount(3);

	double * variables = (double*)malloc(3 * sizeof(double));
	double * outputs_derivatives = (double*)malloc(sizeof(double));
	double ** derivatives = (double**)malloc(3 * sizeof(double*));

	STAUS * my_seed = initSeed(0);

	for (unsigned int var_i = 0; var_i < 3; var_i++)
	{
		*(variables + var_i) = anorm_est(my_seed);
		*(derivatives + var_i) = (double*)malloc(sizeof(double));
	}

	const unsigned int max_iterations = my_args.getArgumentINT("-i");

	gradient_method.setLearningRate(0.05);
	gradient_method.setMomentum(0.9);

	gradient_method.setVariablesDerivativesPointersManager(derivatives);
	gradient_method.setVariablesValuesPointerManager(variables);
	gradient_method.setOutputsDerivativesPointerManager(outputs_derivatives);
	gradient_method.setBatchSize(training_data.getDataSize() / 5);

	double previous_error = training_data.getDataSize();
	for (unsigned int iteration = 0; iteration < max_iterations; iteration++)
	{
		double mean_error = 0.0;
		for (unsigned int pattern_index = 0; pattern_index < training_data.getDataSize(); pattern_index++)
		{
			const double projection = *(variables) * *(*(training_data.getInputData() + pattern_index)) +
				*(variables + 1) * *(*(training_data.getInputData() + pattern_index) + 1) +
				*(variables + 2);
			const double prediction = 1.0 / (1.0 + exp(-projection));

			const int groundtruth = *(*(training_data.getOutputData() + pattern_index));
			const double differences = groundtruth - prediction;
			const double error = differences * differences / 2.0;
			*outputs_derivatives = -differences;
			mean_error += error / (double)training_data.getDataSize();

			**(derivatives) = *(*(training_data.getInputData() + pattern_index));
			**(derivatives + 1) = *(*(training_data.getInputData() + pattern_index) + 1);
			**(derivatives + 2) = 1.0;

			gradient_method.updateDeltasValues();
		}
		const double squared_gradient_norm = gradient_method.updateVariablesValues();

		while (!gradient_method.confirmDescent(previous_error - mean_error))
		{
			mean_error = 0.0;
			for (unsigned int pattern_index = 0; pattern_index < training_data.getDataSize(); pattern_index++)
			{
				const double projection = *(variables) * *(*(training_data.getInputData() + pattern_index)) +
					*(variables + 1) * *(*(training_data.getInputData() + pattern_index) + 1) +
					*(variables + 2);
				const double prediction = 1.0 / (1.0 + exp(-projection));
				const int groundtruth = *(*(training_data.getOutputData() + pattern_index));
				const double differences = groundtruth - prediction;
				const double error = differences * differences / 2.0;
				mean_error += error / (double)training_data.getDataSize();
			}
		}

		previous_error = mean_error;
		printf("MSE = %f, squared gradient = %f, [%f, %f, %f]\n", mean_error, squared_gradient_norm, *variables, *(variables + 1), *(variables + 2));
	}

	
	for (unsigned int var_i = 0; var_i < 3; var_i++)
	{
		printf("var[%i] = %f\n", var_i, *(variables + var_i));
		free(*(derivatives + var_i));
	}
	free(derivatives);
	free(variables);
	free(outputs_derivatives);

	return 0;
}