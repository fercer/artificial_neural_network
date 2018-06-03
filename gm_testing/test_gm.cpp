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
	my_args.newArgument("Iterations", "i", "iterations", 100, true);
	
	my_args.showHelp();

	IMG_DATA * src_img = loadImagePGM(my_args.getArgumentCHAR("-dr"));
	IMG_DATA * trg_img = loadImagePGM(my_args.getArgumentCHAR("-de"));

	const unsigned int width = src_img->width;
	const unsigned int height = src_img->height;
	const unsigned int inputs_count = (unsigned int)(width * height);
	const unsigned int outputs_count = 1;
	const unsigned int variables_count = 8;


	gradientMethods gradient_method;
	//gradient_method.setGradientMethod(gradientMethods::GRADIENT_DESCENT);
	gradient_method.setGradientMethod(gradientMethods::LEVENBERG_MARQUARDT);
	//gradient_method.setGradientMethod(gradientMethods::MINI_BATCH_GRADIENT_DESCENT);
	//gradient_method.setGradientMethod(gradientMethods::STOCHASTIC_GRADIENT_DESCENT);
	//gradient_method.setGradientMethod(gradientMethods::MINI_BATCH_STOCHASTIC_GRADIENT_DESCENT);


	gradient_method.setOutputsCount(outputs_count);
	gradient_method.setTrainingDataSize(1);
	gradient_method.setVariablesCount(variables_count);

	double * best_variables = (double*)malloc(variables_count * sizeof(double));
	double * variables = (double*)malloc(variables_count * sizeof(double));
	double * outputs_derivatives = (double*)malloc(outputs_count*sizeof(double));
	double ** derivatives = (double**)malloc(variables_count * sizeof(double*));

	STAUS * my_seed = initSeed(0);

	for (unsigned int var_i = 0; var_i < variables_count; var_i++)
	{
		*(variables + var_i) = 0.0;
		*(derivatives + var_i) = (double*)malloc(outputs_count*sizeof(double));
	}
	*(variables) = 0.8;
	*(variables + 1) = 1.0;

	*(variables + 2) = cos(0.0*MY_PI / 180.0);
	*(variables + 3) = -sin(0.0*MY_PI / 180.0);
	*(variables + 4) = sin(0.0*MY_PI / 180.0);
	*(variables + 5) = cos(0.0*MY_PI / 180.0);

	*(variables + 6) = 214.0;
	*(variables + 7) = 214.0;

	const unsigned int max_iterations = my_args.getArgumentINT("-i");

	gradient_method.setLearningRate(0.05);
	gradient_method.setMomentum(0.9);

	gradient_method.setVariablesDerivativesPointersManager(derivatives);
	gradient_method.setVariablesValuesPointerManager(variables);
	gradient_method.setOutputsDerivativesPointerManager(outputs_derivatives);
	gradient_method.setBatchSize(width);
		
	double previous_error = inputs_count;
	double best_fit = previous_error;
	
	char filename[512];
	for (unsigned int iteration = 0; iteration < max_iterations; iteration++)
	{
		// Compute the derivatives and transform the source image:
		IMG_DATA * tmp_img = rotateBicubic(src_img, *(variables+2) / *(variables), *(variables + 4) / *(variables), *(variables + 3) / *(variables + 1), *(variables + 5) / *(variables + 1));
		IMG_DATA * dx_img = computeDerivativesX(tmp_img);
		IMG_DATA * dy_img = computeDerivativesY(tmp_img);


		IMG_DATA * diff_img = diffImage(tmp_img, trg_img, *(variables + 6), *(variables + 7));
		
		if (iteration == 0)
			saveImagePGM("first_resp.pgm", diff_img);

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
				**(derivatives) = -*(dx_img->image_data + (y + ys_ini)*dx_img->width + x + xs_ini) *					((x + xs_ini) * *(variables + 2) + (y + ys_ini) * *(variables + 3));

				**(derivatives + 1) = -*(dy_img->image_data + (y + ys_ini)*dy_img->width + x + xs_ini) *					((x + xs_ini) * *(variables + 4) + (y + ys_ini) * *(variables + 5));

				**(derivatives + 2) = 0.0;//-*(dx_img->image_data + (y + ys_ini)*dx_img->width + x + xs_ini) *					*(variables) * (x + xs_ini);

				**(derivatives + 3) = 0.0;//-*(dx_img->image_data + (y + ys_ini)*dx_img->width + x + xs_ini) *					*(variables) * (y + ys_ini);

				**(derivatives + 4) = 0.0;//-*(dy_img->image_data + (y + ys_ini)*dy_img->width + x + xs_ini) *					*(variables + 1) * (x + xs_ini);

				**(derivatives + 5) = 0.0;//-*(dy_img->image_data + (y + ys_ini)*dy_img->width + x + xs_ini) *					*(variables + 1) * (y + ys_ini);

				**(derivatives + 6) = 0.0;//-*(dy_img->image_data + (y + ys_ini)*dy_img->width + x + xs_ini);

				**(derivatives + 7) = 0.0;//-*(dy_img->image_data + (y + ys_ini)*dy_img->width + x + xs_ini);

				gradient_method.updateDeltasValues();
			}
		}
		const double squared_gradient_norm = gradient_method.updateVariablesValues();
		printf("[%i/%i] MSE = %f, previous MSE = %f, squared gradient = %f\n", iteration, max_iterations, mean_error, previous_error, squared_gradient_norm);
		while (!gradient_method.confirmDescent(previous_error - mean_error))
		{	

			IMG_DATA * tmp_img_LM = rotateBicubic(src_img, *(variables + 2) / *(variables), *(variables + 4) / *(variables), *(variables + 3) / *(variables + 1), *(variables + 5) / *(variables + 1));
			// Compute the derivatives and transform the source image:
			mean_error = computeLoss(tmp_img_LM, trg_img, *(variables+6), *(variables+7));

			free(tmp_img_LM->image_data);
			free(tmp_img_LM);
		}

		previous_error = mean_error;
		if (best_fit > mean_error)
		{
			best_fit = mean_error;
			memcpy(best_variables, variables, variables_count * sizeof(double));
		}

		free(tmp_img->image_data);
		free(tmp_img);
		free(dx_img->image_data);
		free(dx_img);
		free(dy_img->image_data);
		free(dy_img);
		free(diff_img->image_data);
		free(diff_img);
	}

	IMG_DATA * tmp_img = rotateBicubic(src_img, *(variables + 2) / *(variables), *(variables + 4) / *(variables), *(variables + 3) / *(variables + 1), *(variables + 5) / *(variables + 1));
	IMG_DATA * diff_img = diffImage(tmp_img, trg_img, *(variables + 6), *(variables + 7));

	saveImagePGM("final_resp.pgm", diff_img);

	for (unsigned int var_i = 0; var_i < variables_count; var_i++)
	{
		printf("var[%i] = %f\n", var_i, *(variables + var_i));
		free(*(derivatives + var_i));
	}


	IMG_DATA * best_tmp_img = rotateBicubic(src_img, *(variables + 2) / *(variables), *(variables + 4) / *(variables), *(variables + 3) / *(variables + 1), *(variables + 5) / *(variables + 1));
	IMG_DATA * best_diff_img = diffImage(best_tmp_img, trg_img, *(best_variables + 6), *(best_variables + 7));

	saveImagePGM("best_resp.pgm", best_diff_img);

	printf("Best set:\n");
	for (unsigned int var_i = 0; var_i < variables_count; var_i++)
	{
		printf("var[%i] = %f\n", var_i, *(best_variables + var_i));
	}
	
	free(derivatives);
	free(variables);
	free(best_variables);
	free(outputs_derivatives);
	free(my_seed);

	free(src_img->image_data);
	free(src_img);
	free(trg_img->image_data);
	free(trg_img);

	free(tmp_img->image_data);
	free(tmp_img);
	free(diff_img->image_data);
	free(diff_img);

	free(best_tmp_img->image_data);
	free(best_tmp_img);
	free(best_diff_img->image_data);
	free(best_diff_img);
	return 0;
}