#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../src/Neuron.h"
#include "../src/Input_pattern_class.h"
#include "../src/LossFunction_class.h"
#include "../src/L2LossFunction_class.h"

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
	my_args.newArgument("Testing data filename", "de", "dataset-testing", "NULL", false);
	my_args.newArgument("Iterations", "i", "iterations", 2000, true);
	
	my_args.showHelp();

	IMG_DATA * src_img = loadImagePGM(my_args.getArgumentCHAR("-dr"));
	IMG_DATA * trg_img = loadImagePGM(my_args.getArgumentCHAR("-de"));

	const unsigned int width = src_img->width;
	const unsigned int height = src_img->height;
	const unsigned int inputs_count = (unsigned int)(width * height);
	const unsigned int outputs_count = 1;
	const unsigned int variables_count = 1;


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
		*(derivatives + var_i) = (double*)malloc(outputs_count*sizeof(double));
	}
	*(variables) = 90.0 * MY_PI / 180.0;

	const unsigned int max_iterations = my_args.getArgumentINT("-i");

	gradient_method.setLearningRate(0.05);
	gradient_method.setMomentum(0.9);

	gradient_method.setVariablesDerivativesPointersManager(derivatives);
	gradient_method.setVariablesValuesPointerManager(variables);
	gradient_method.setOutputsDerivativesPointerManager(outputs_derivatives);
	gradient_method.setBatchSize(width);
		
	double previous_error = inputs_count;
	
	char filename[512];
	for (unsigned int iteration = 0; iteration < max_iterations; iteration++)
	{
		// Compute the derivatives and transform the source image:
		const double ctheta = cos(*(variables));
		const double stheta = sin(*(variables));
		IMG_DATA * tmp_img = rotateBicubic(src_img, ctheta, stheta, -stheta, ctheta);

		if (iteration == 0)
		saveImagePGM("first_resp.pgm", tmp_img);


		IMG_DATA * dx_img = computeDerivativesX(tmp_img);
		IMG_DATA * dy_img = computeDerivativesY(tmp_img);

		IMG_DATA * diff_img = diffImage(tmp_img, trg_img, 0.0, 0.0);
		
		const unsigned int computable_width = diff_img->width;
		const unsigned int computable_height = diff_img->height;
		const unsigned int xs_ini = diff_img->UL_x;
		const unsigned int ys_ini = diff_img->UL_y;

		double mean_error = 0.0;
		for (unsigned int y = 0; y < computable_height; y++)
		{
			for (unsigned int x = 0; x < computable_width; x++)
			{
				const double difference = *(diff_img->image_data + y*computable_width + x);
				mean_error += difference * difference / 2.0;
				*(outputs_derivatives) = difference;

				// Contribution to the error corresponding to the theta parameters:
				**(derivatives) = 
					-*(dx_img->image_data + (y + ys_ini)*dx_img->width + x + xs_ini) * (-stheta * (x + xs_ini) - ctheta * (y + ys_ini)) -
					-*(dy_img->image_data + (y + ys_ini)*dy_img->width + x + xs_ini) * (ctheta * (x + xs_ini) - stheta * (y + ys_ini));

				gradient_method.updateDeltasValues();
			}
		}
		const double squared_gradient_norm = gradient_method.updateVariablesValues();

		for (unsigned int variable_index = 0; variable_index < variables_count; variable_index++)
		{
			printf("%f\n", *(variables + variable_index));
		}
		printf("\n");
		printf("[%i/%i] MSE = %f, previous MSE = %f, squared gradient = %f\n", iteration, max_iterations, mean_error, previous_error, squared_gradient_norm);
		while (!gradient_method.confirmDescent(previous_error - mean_error))
		{	
			for (unsigned int variable_index = 0; variable_index < variables_count; variable_index++)
			{
				printf("%f\n", *(variables + variable_index));
			}
			printf("\n");

			// Compute the derivatives and transform the source image:
			const double ctheta = cos(*(variables));
			const double stheta = sin(*(variables));
			IMG_DATA * tmp_img_LM = rotateBicubic(src_img, ctheta, stheta, -stheta, ctheta);
			mean_error = computeLoss(tmp_img_LM, trg_img,0.0, 0.0);
			printf("MSE = %f\n", mean_error);
			free(tmp_img_LM->image_data);
			free(tmp_img_LM);
		}
		previous_error = mean_error;
		for (unsigned int variable_index = 0; variable_index < variables_count; variable_index++)
		{
			printf("%f\n", *(variables + variable_index));
		}
		printf("\n");

		free(tmp_img->image_data);
		free(tmp_img);
		free(dx_img->image_data);
		free(dx_img);
		free(dy_img->image_data);
		free(dy_img);
		free(diff_img->image_data);
		free(diff_img);
	}

	const double ctheta = cos(*(variables));
	const double stheta = sin(*(variables));
	IMG_DATA * tmp_img = rotateBicubic(src_img, ctheta, stheta, -stheta, ctheta);

	saveImagePGM("final_resp.pgm", tmp_img);

	for (unsigned int var_i = 0; var_i < variables_count; var_i++)
	{
		printf("var[%i] = %f\n", var_i, *(variables + var_i));
		free(*(derivatives + var_i));
	}
	free(derivatives);
	free(variables);
	free(outputs_derivatives);
	free(my_seed);

	free(src_img->image_data);
	free(src_img);
	free(trg_img->image_data);
	free(trg_img);

	return 0;
}