#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../src/random_numbers_generator.h"
#include "../src/data_handler_class.h"
#include "../src/args_fercer.h"
#include "image_process_handler_class.h"

int main(int argc, char * argv[])
{
	IMAGE_PROCESS_HANDLER image_process_1;
	image_process_1.setFilename("single_image_sumation.xml");
	image_process_1.loadProcess();
	printf("Running process ...\n");
	image_process_1.runProcess();
	printf("Running process ...\n");

	image_process_1.setInputNodeValue(2, "sum_a_b_2.pgm");
	image_process_1.setInputNodeValue(0, -30.0);
	image_process_1.runImageOperation(7);

	return 0;
}