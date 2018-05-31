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
	my_args.newArgument("Iterations", "i", "iterations", 20, true);
	
	my_args.showHelp();

	IMG_DATA * src_img = loadImagePGM(my_args.getArgumentCHAR("-dr"));
	IMG_DATA * trg_img = loadImagePGM(my_args.getArgumentCHAR("-de"));

	printf("width = %i, height = %i\n", src_img->width, src_img->height);
	printf("UL = (%i, %i), UR = (%i, %i), LR = (%i, %i), LL = (%i, %i)\n", src_img->UL_x, src_img->UL_y, src_img->UR_x, src_img->UR_y, src_img->LR_x, src_img->LR_y, src_img->LL_x, src_img->LL_y);
	
	*(src_img->image_data + src_img->UL_y * src_img->width + src_img->UL_x) = src_img->max_value;
	*(src_img->image_data + src_img->UR_y * src_img->width + src_img->UR_x) = src_img->max_value;
	*(src_img->image_data + src_img->LR_y * src_img->width + src_img->LR_x) = src_img->max_value;
	*(src_img->image_data + src_img->LL_y * src_img->width + src_img->LL_x) = src_img->max_value;
	saveImagePGM("unrotated_temp.pgm", src_img);

	const double degrees = 15.0;
	IMG_DATA * dst_img = rotateBicubic(src_img, cos(MY_PI * degrees / 180.0), -sin(MY_PI * degrees / 180.0), sin(MY_PI * degrees / 180.0), cos(MY_PI * degrees / 180.0));
	
	printf("UL = (%i, %i), UR = (%i, %i), LR = (%i, %i), LL = (%i, %i)\n", dst_img->UL_x, dst_img->UL_y, dst_img->UR_x, dst_img->UR_y, dst_img->LR_x, dst_img->LR_y, dst_img->LL_x, dst_img->LL_y);

	*(dst_img->image_data + dst_img->UL_y * dst_img->width + dst_img->UL_x) = dst_img->max_value;
	*(dst_img->image_data + dst_img->UR_y * dst_img->width + dst_img->UR_x) = dst_img->max_value;
	*(dst_img->image_data + dst_img->LR_y * dst_img->width + dst_img->LR_x) = dst_img->max_value;
	*(dst_img->image_data + dst_img->LL_y * dst_img->width + dst_img->LL_x) = dst_img->max_value;

	saveImagePGM("rotated_temp.pgm", dst_img);

	return 0;

	const unsigned int width = src_img->width;
	const unsigned int height = src_img->height;
	const unsigned int inputs_count = (unsigned int)(width * height);
	const unsigned int outputs_count = 1;

	const unsigned int variables_count = 6;

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
	*(variables) = 1.0;
	*(variables + 1) = 0.0;
	*(variables + 2) = 0.0;
	*(variables + 3) = 0.0;
	*(variables + 4) = 1.0;
	*(variables + 5) = 0.0;

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
		IMG_DATA * tmp_img = rotateBicubic(src_img, *(variables), *(variables + 1), *(variables + 3), *(variables + 4));
		IMG_DATA * dx_img = computeDerivativesX(tmp_img);
		IMG_DATA * dy_img = computeDerivativesY(tmp_img);

		double mean_error = 0.0;

		// Compute the loss only on the intersection of the both images:
		const unsigned int src_width = tmp_img->width;
		const unsigned int src_height = tmp_img->height;
		const unsigned int trg_width = trg_img->width;
		const unsigned int trg_height = trg_img->height;

		const unsigned int src_half_width_left = (unsigned int)floor((double)src_width / 2.0);
		const unsigned int src_half_height_upper = (unsigned int)floor((double)src_height / 2.0);

		const unsigned int ULs_x = -src_half_width_left + (unsigned int)floor(*(variables + 2));
		const unsigned int ULs_y = -src_half_height_upper + (unsigned int)floor(*(variables + 5));
		const unsigned int LRs_x = ULs_x + src_width;
		const unsigned int LRs_y = ULs_y + src_height;

		const unsigned int ULt_x = -(unsigned int)floor((double)trg_width / 2.0);
		const unsigned int ULt_y = -(unsigned int)floor((double)trg_height / 2.0);
		const unsigned int LRt_x = ULt_x + trg_width;
		const unsigned int LRt_y = ULt_y + trg_height;

		unsigned int xs_ini, xs_end;
		unsigned int ys_ini, ys_end;
		unsigned int xt_ini, yt_ini;

		if (ULs_x > ULt_x)
		{
			xs_ini = 0;
			xt_ini = ULs_x - ULt_x;
		}
		else
		{
			xs_ini = ULt_x - ULs_x;
			xt_ini = 0;
		}

		if (ULs_y > ULt_y)
		{
			ys_ini = 0;
			yt_ini = ULs_y - ULt_y;
		}
		else
		{
			ys_ini = ULt_y - ULs_y;
			yt_ini = 0;
		}

		if (LRs_x < LRt_x)
		{
			xs_end = src_width - 1; // To make it inclusive
		}
		else
		{
			xs_end = LRt_x - ULs_x - 1;
		}

		if (LRs_y < LRt_y)
		{
			ys_end = src_height - 1; // To make it inclusive
		}
		else
		{
			ys_end = LRt_y - ULs_y - 1;
		}

		const unsigned int computable_width = xs_end - xs_ini;
		const unsigned int computable_height = ys_end - ys_ini;
		const double out_limit_penalty = src_width * src_height - computable_width * computable_height;

		for (unsigned int y = 0; y <= computable_height; y++)
		{
			for (unsigned int x = 0; x <= computable_width; x++)
			{
				mean_error += computeLossPerPixel(x, y, tmp_img, trg_img, *(variables + 2), *(variables + 5), dx_img, dy_img, outputs_derivatives, derivatives, xs_ini, ys_ini, xt_ini, yt_ini);
				gradient_method.updateDeltasValues();
			}
		}
		const double squared_gradient_norm = gradient_method.updateVariablesValues();

		printf("MSE = %f, previous MSE = %f\n", mean_error, previous_error);
		while (!gradient_method.confirmDescent(previous_error - mean_error))
		{			
			// Compute the derivatives and transform the source image:
			IMG_DATA * tmp_img_LM = rotateBicubic(src_img, *(variables), *(variables + 1), *(variables + 3), *(variables + 4));
			mean_error = computeLoss(tmp_img_LM, trg_img, *(variables + 2), *(variables + 5)) / (double)inputs_count;

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

		sprintf(filename, "res_%i.pgm", iteration);
		saveImagePGM(filename, tmp_img);

		sprintf(filename, "dx_res_%i.pgm", iteration);
		saveImagePGM(filename, dx_img);

		sprintf(filename, "dy_res_%i.pgm", iteration);
		saveImagePGM(filename, dy_img);

		free(tmp_img->image_data);
		free(tmp_img);
		free(dx_img->image_data);
		free(dx_img);
		free(dy_img->image_data);
		free(dy_img);
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

	free(src_img->image_data);
	free(src_img);
	free(trg_img->image_data);
	free(trg_img);

	return 0;
}