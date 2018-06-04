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

	unsigned int max_x, max_y;
	max_x = filter_resp->tail_roi->UL_x;
	max_y = filter_resp->tail_roi->UL_y;
	*(filter_resp->image_data + max_y * filter_resp->width + max_x) = filter_resp->min_value;

	max_x = filter_resp->tail_roi->UR_x;
	max_y = filter_resp->tail_roi->UR_y;
	*(filter_resp->image_data + max_y * filter_resp->width + max_x) = filter_resp->min_value;

	max_x = filter_resp->tail_roi->LR_x;
	max_y = filter_resp->tail_roi->LR_y;
	*(filter_resp->image_data + max_y * filter_resp->width + max_x) = filter_resp->min_value;

	max_x = filter_resp->tail_roi->LL_x;
	max_y = filter_resp->tail_roi->LL_y;
	*(filter_resp->image_data + max_y * filter_resp->width + max_x) = filter_resp->min_value;

	saveImagePGM("filtering_result.pgm", filter_resp);

	freeImageData(src_img);
	freeImageData(kernel_img);
	freeImageData(filter_resp);

	return 0;
}