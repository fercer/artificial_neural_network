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
	return 0;
}