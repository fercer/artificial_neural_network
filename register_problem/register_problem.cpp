#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../src/random_numbers_generator.h"
#include "../src/data_handler_class.h"
#include "../src/args_fercer.h"

#include "image_functions.h"

int main(int argc, char * argv[])
{
	ARGUMENTS my_args;
	my_args.setArgc(argc);
	my_args.setArgv(argv);

	my_args.newArgument("Training data filename", "dr", "dataset-training", "NULL", false);
	my_args.newArgument("Testing data filename", "de", "dataset-testing", "NULL", false);
	
	my_args.showHelp();

	IMG_DATA * src_img = loadImagePGM(my_args.getArgumentCHAR("-dr"));
	IMG_DATA * kernel_img = loadImagePGM(my_args.getArgumentCHAR("-de"));

	IMG_DATA * filter_resp = filterImage(src_img, kernel_img);

	computeImageMax(filter_resp);
	computeImageMin(filter_resp);

	saveImagePGM("filtering_result.pgm", filter_resp);

	freeImageData(src_img);
	freeImageData(kernel_img);
	freeImageData(filter_resp);

	return 0;
}