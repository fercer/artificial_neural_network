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

	NODE_SCALAR<char*> image_a_filename(my_args.getArgumentCHAR("-a"));
	NODE_SCALAR<char*> image_b_filename(my_args.getArgumentCHAR("-b"));

	
	LOAD_IMAGE_OPERATION image_a_loader;
	image_a_loader.setFilename(&image_a_filename);
	LOAD_IMAGE_OPERATION image_b_loader;
	image_b_loader.setFilename(&image_b_filename);
			
	
	SUM_IMAGE_OPERATION sum_a_b;
	sum_a_b.setInputOperationA(&image_a_loader);
	sum_a_b.setInputOperationB(&image_b_loader);

	MIN_IMAGE_SCALAR_OPERATION min_sum_value;
	min_sum_value.setInputOperation(&sum_a_b);

	NODE_SCALAR<double> min_sum_value_node(min_sum_value.getScalarValue());

	ONES_IMAGE_OPERATION centering_sum_a_b;
	centering_sum_a_b.setWidth(sum_a_b.getImageData()->width);
	centering_sum_a_b.setHeight(sum_a_b.getImageData()->height);
	centering_sum_a_b.setWidth(&min_sum_value_node);

	MAX_IMAGE_SCALAR_OPERATION max_sum_value;
	max_sum_value.setInputOperation(&sum_a_b);
	
	SUM_IMAGE_OPERATION center_sum_a_b;
	center_sum_a_b.setInputOperationA(&sum_a_b);
	center_sum_a_b.setInputOperationB(&centering_sum_a_b);
	
	NODE_SCALAR<double> dynamic_range_sum(1.0/(max_sum_value.getScalarValue() - min_sum_value.getScalarValue()));

	printf("Dynamic range of the sum = %f\n", dynamic_range_sum.getScalarValue());

	SUM_IMAGE_OPERATION normalize_sum_a_b;
	normalize_sum_a_b.setParameterA(&dynamic_range_sum);
	normalize_sum_a_b.setInputOperationA(&center_sum_a_b);

	SAVE_IMAGE_OPERATION save_sum_a_b;
	save_sum_a_b.setFilename("sum_a_b.pgm");
	save_sum_a_b.setInputOperationA(&normalize_sum_a_b);

	save_sum_a_b.getImageData();
	
	return 0;
}