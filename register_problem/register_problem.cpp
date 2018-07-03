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
	image_process_1.setFilename("D:/Apps/artificial_neural_network/register_problem/test_cases/single_text.xml");
	//image_process_1.setFilename("C:/Users/cimat/artificial_neural_network/register_problem/test_cases/single_text.xml");
	image_process_1.loadProcess();
	printf("Running process 1 ...\n");
	image_process_1.runProcess();

	return 0;
}