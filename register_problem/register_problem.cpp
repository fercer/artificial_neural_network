#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../src/random_numbers_generator.h"
#include "../src/data_handler_class.h"
#include "../src/args_fercer.h"
#include "image_process_handler_class.h"

int main(int argc, char * argv[])
{
	LOAD_IMAGE_OPERATION load_op;
	load_op.assignNodeValue(0, "D:/Apps/artificial_neural_network/register_problem/typography/char_67.pgm");

	SAVE_IMAGE_SCALAR_OPERATION save_op;
	save_op.assignNodeValue(0, "test.pgm");
	save_op.setInputOperation(&load_op);
	save_op.getScalarValue();

	return 1;

	IMAGE_PROCESS_HANDLER image_process_1;
	image_process_1.setFilename("single_text.xml");
	image_process_1.loadProcess();
	printf("Running process 1 ...\n");
	image_process_1.runProcess();

	return 0;
}