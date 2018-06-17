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

	return 0;
}