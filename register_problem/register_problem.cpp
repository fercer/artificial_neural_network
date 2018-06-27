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

	image_process_1.setInputNodeValue(1, "save_B.pgm");
	image_process_1.setInputNodeValue(0, 0.0);
	image_process_1.runImageOperation(3);


	image_process_1.setInputNodeValue(1, "save_C.pgm");
	NODE_SCALAR<double> cos_phi(cos(15.0));
	NODE_SCALAR<double> sin_phi(sin(15.0));
	image_process_1.setInputNodeValue(0, 30.0);
	image_process_1.setInputNodeToImageOperation(2, 1, &sin_phi);
	image_process_1.setInputNodeToImageOperation(2, 3, &cos_phi);
	image_process_1.runImageOperation(3);


	return 0;
}