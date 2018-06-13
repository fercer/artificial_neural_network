#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../src/random_numbers_generator.h"
#include "../src/data_handler_class.h"
#include "../src/args_fercer.h"

#include "div_image_operation_class.h"
#include "mult_image_operation_class.h"
#include "sum_image_operation_class.h"
#include "scalar_sum_image_operation_class.h"
#include "load_image_operation_class.h"
#include "save_image_operation_class.h"
#include "filter_image_operation_class.h"
#include "rotate_image_operation_class.h"
#include "translate_image_operation_class.h"
#include "sqroot_image_operation_class.h"
#include "max_image_scalar_operation_class.h"
#include "min_image_scalar_operation_class.h"
#include "sum_image_scalar_operation_class.h"
#include "ones_image_operation_class.h"

int main(int argc, char * argv[])
{
	ARGUMENTS my_args;
	my_args.setArgc(argc);
	my_args.setArgv(argv);

	my_args.newArgument("Image a filename", "a", "image-a", "NULL", false);
	my_args.newArgument("Image b filename", "b", "image-b", "NULL", false);
	
	my_args.showHelp();

	LOAD_IMAGE_OPERATION image_a_loader;
	LOAD_IMAGE_OPERATION image_b_loader;

	image_a_loader.setImageName(my_args.getArgumentCHAR("-a"));
	image_b_loader.setImageName(my_args.getArgumentCHAR("-b"));
	
	ONES_IMAGE_OPERATION ones_image;
	ones_image.setImageName("Ones 200 x 300");
	ones_image.setParameterA(200);
	ones_image.setParameterB(300);
	ones_image.setParameterC(0.0);

	TRANSLATE_IMAGE_OPERATION translate_image_a;
	translate_image_a.setImageName("trans image a");
	translate_image_a.setParameterA(-500.0);
	translate_image_a.setParameterB(-250.0);
	translate_image_a.setInputOperationA(&image_a_loader);

	ROTATE_IMAGE_OPERATION rotate_image_a;
	rotate_image_a.setImageName("rotate translated a");
	const double theta = 0.0 * MY_PI / 180.0;
	const double phi = 0.0 * MY_PI / 180.0;
	const double ctheta = cos(theta);
	const double stheta = sin(theta);
	const double cphi = cos(phi);
	const double sphi = sin(phi);
	rotate_image_a.setParameterA(ctheta);
	rotate_image_a.setParameterB(sphi);
	rotate_image_a.setParameterC(-stheta);
	rotate_image_a.setParameterD(cphi);
	rotate_image_a.setInputOperationA(&translate_image_a);
	
	SUM_IMAGE_OPERATION sum_ab;
	sum_ab.setImageName("sum a + b");
	sum_ab.setInputOperationA(&rotate_image_a);
	//sum_ab.setInputOperationB(&image_b_loader);

	MAX_IMAGE_SCALAR_OPERATION max_value_sum_ab;
	max_value_sum_ab.setInputOperation(&rotate_image_a);

	MIN_IMAGE_SCALAR_OPERATION min_value_sum_ab;
	min_value_sum_ab.setInputOperation(&rotate_image_a);

	printf("min = %f\n", min_value_sum_ab.getImageScalar());
	printf("max = %f\n", max_value_sum_ab.getImageScalar());

	SCALAR_SUM_IMAGE_OPERATION sum_minima;
	sum_minima.setImageName("sum minima");
	sum_minima.setInputOperationA(&rotate_image_a);
	sum_minima.setParameterA(-min_value_sum_ab.getImageScalar());

	SUM_IMAGE_OPERATION norm_sum_a_b;
	norm_sum_a_b.setImageName("div by range");
	norm_sum_a_b.setInputOperationA(&sum_minima);
	norm_sum_a_b.setParameterA(1.0 / (max_value_sum_ab.getImageScalar() - min_value_sum_ab.getImageScalar()));
	
	SAVE_IMAGE_OPERATION save_sum_ab;
	save_sum_ab.setImageName("sum_a_b.pgm");
	save_sum_ab.setInputOperationA(&norm_sum_a_b);
	save_sum_ab.getImageData();

	return 0;
}