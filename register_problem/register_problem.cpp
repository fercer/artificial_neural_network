#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../src/random_numbers_generator.h"
#include "../src/data_handler_class.h"
#include "../src/args_fercer.h"

#include "div_image_operation_class.h"
#include "mult_image_operation_class.h"
#include "sum_image_operation_class.h"
#include "load_image_operation_class.h"
#include "save_image_operation_class.h"
#include "filter_image_operation_class.h"
#include "rotate_image_operation_class.h"
#include "sqroot_image_operation_class.h"
#include "max_image_scalar_operation_class.h"
#include "min_image_scalar_operation_class.h"
#include "sum_image_scalar_operation_class.h"

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

	SUM_IMAGE_OPERATION sum_ab;

	sum_ab.setImageName("sum a + b");
	sum_ab.setInputOperationA(&image_a_loader);
	sum_ab.setInputOperationB(&image_b_loader);

	MAX_IMAGE_SCALAR_OPERATION max_value_sum_ab;
	max_value_sum_ab.setInputOperation(&sum_ab);
	const double max_sum_value = max_value_sum_ab.getImageScalar();

	MIN_IMAGE_SCALAR_OPERATION min_value_sum_ab;
	min_value_sum_ab.setInputOperation(&sum_ab);
	const double min_sum_value = min_value_sum_ab.getImageScalar();

	printf("a + b; min = %f, max = %f\n", min_sum_value, max_sum_value);

	SAVE_IMAGE_OPERATION save_sum_ab;
	save_sum_ab.setImageName("sum_a_b.pgm");
	save_sum_ab.setInputOperationA(&sum_ab);
	save_sum_ab.getImageData();

	return 0;
}