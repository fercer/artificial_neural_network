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

#include "register_problem.h"

int main(int argc, char * argv[])
{
	ARGUMENTS my_args;
	my_args.setArgc(argc);
	my_args.setArgv(argv);

	my_args.newArgument("Training data filename", "dr", "dataset-training", "NULL", false);
	my_args.newArgument("Testing data filename", "de", "dataset-testing", "NULL", false);
	my_args.newArgument("Iterations", "i", "iterations", 5, true);
	
	my_args.showHelp();

	double * org_img = loadImagePGM(my_args.getArgumentCHAR("-dr"));
	double * org_trg_img = loadImagePGM(my_args.getArgumentCHAR("-de"));

	double * org_temp = (double*)malloc(2*sizeof(double));
	*(org_temp) = *(org_trg_img) * 8;
	*(org_temp + 1) = *(org_trg_img + 1) * 8;

	double * trg_img = presetProblem(org_trg_img, org_temp);
	free(org_trg_img);
	free(org_temp);

	double * src_img = presetProblem(org_img, trg_img);
	free(org_img);

	const unsigned int width = *(src_img);
	const unsigned int height = *(src_img + 1);
	const unsigned int inputs_count = (unsigned int)(width * height);
	const unsigned int outputs_count = 1;

	const unsigned int variables_count = 3;

	gradientMethods gradient_method;
	//gradient_method.setGradientMethod(gradientMethods::GRADIENT_DESCENT);
	gradient_method.setGradientMethod(gradientMethods::LEVENBERG_MARQUARDT);
	//gradient_method.setGradientMethod(gradientMethods::MINI_BATCH_GRADIENT_DESCENT);
	//gradient_method.setGradientMethod(gradientMethods::STOCHASTIC_GRADIENT_DESCENT);
	//gradient_method.setGradientMethod(gradientMethods::MINI_BATCH_STOCHASTIC_GRADIENT_DESCENT);

	gradient_method.setOutputsCount(outputs_count);
	gradient_method.setTrainingDataSize(1);
	gradient_method.setVariablesCount(variables_count);
	
	double * variables = (double*)malloc(variables_count * sizeof(double));
	double * outputs_derivatives = (double*)malloc(outputs_count*sizeof(double));
	double ** derivatives = (double**)malloc(variables_count * sizeof(double*));

	STAUS * my_seed = initSeed(0);

	for (unsigned int var_i = 0; var_i < variables_count; var_i++)
	{
		*(variables + var_i) = 0.0;
		*(derivatives + var_i) = (double*)malloc(outputs_count*sizeof(double));
	}

	const unsigned int max_iterations = my_args.getArgumentINT("-i");

	gradient_method.setLearningRate(0.05);
	gradient_method.setMomentum(0.9);

	gradient_method.setVariablesDerivativesPointersManager(derivatives);
	gradient_method.setVariablesValuesPointerManager(variables);
	gradient_method.setOutputsDerivativesPointerManager(outputs_derivatives);
	gradient_method.setBatchSize(width);
		
	double previous_error = inputs_count;
	
	double * tmp_img = (double*)malloc((width * height + 4) * sizeof(double));
	double * dx_img = (double*)malloc((width * height + 4) * sizeof(double));
	double * dy_img = (double*)malloc((width * height + 4) * sizeof(double));

	*(tmp_img) = width;
	*(tmp_img + 1) = height;

	*(dx_img) = width;
	*(dx_img + 1) = height;

	*(dy_img) = width;
	*(dy_img + 1) = height;

	char filename[512];
	for (unsigned int iteration = 0; iteration < max_iterations; iteration++)
	{
		// Compute the derivatives and transform the source image:
		rotateBicubic(src_img, tmp_img, *(variables), *(variables + 1), *(variables + 2));
		computeDerivatives(tmp_img, dx_img, dy_img);

		double mean_error = 0.0;
		for (unsigned int pix_position = 0; pix_position < inputs_count; pix_position++)
		{			
			mean_error += computeLossPerPixel(pix_position, tmp_img, trg_img, *(variables), dx_img, dy_img, outputs_derivatives, derivatives);
			gradient_method.updateDeltasValues();
		}
		const double squared_gradient_norm = gradient_method.updateVariablesValues();

		printf("MSE = %f, previous MSE = %f\n", mean_error, previous_error);
		while (!gradient_method.confirmDescent(previous_error - mean_error))
		{			
			// Compute the derivatives and transform the source image:
			rotateBicubic(src_img, tmp_img, *(variables), *(variables + 1), *(variables + 2));
			computeDerivatives(tmp_img, dx_img, dy_img);

			mean_error = computeLoss(tmp_img, trg_img) / (double)inputs_count;
			printf("MSE = %f\n", mean_error);
		}
		previous_error = mean_error;
		for (unsigned int variable_index = 0; variable_index < variables_count; variable_index++)
		{
			printf("%f\n", *(variables + variable_index));
		}
		printf("\n");

		sprintf(filename, "res_%i.pgm", iteration);
		saveImagePGM(filename, tmp_img);

		sprintf(filename, "dx_res_%i.pgm", iteration);
		saveImagePGM(filename, dx_img);

		sprintf(filename, "dy_res_%i.pgm", iteration);
		saveImagePGM(filename, dy_img);
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

	free(src_img);
	free(trg_img);
	free(tmp_img);
	free(dx_img);
	free(dy_img);

	return 0;
}