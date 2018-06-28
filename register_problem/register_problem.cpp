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
	printf("Running process 1 ...\n");
	image_process_1.runProcess();

	printf("Running process 2 ...\n");
	image_process_1.setInputNodeToImageScalarOperation(0, 0, (NODE_SCALAR<char*>*)NULL);
	image_process_1.setInputNodeValue(0, 0, 15.0);
	image_process_1.setInputNodeValue(1, "res_B.pgm");
	image_process_1.setInputNodeToImageScalarOperation(0, 0, (IMAGE_OPERATION*)NULL);
	image_process_1.runProcess();

	return 0;
}