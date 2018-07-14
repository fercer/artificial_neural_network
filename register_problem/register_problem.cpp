#include <stdlib.h>
#include <stdio.h>

#include <math.h>

#include "process_visualizer_class.h"
#include "../src/random_numbers_generator.h"
#include "../src/data_handler_class.h"
#include "../src/args_fercer.h"
#include "image_process_handler_class.h"

int main(int argc, char * argv[])
{
	NODE_SCALAR<double> test_node;
	test_node.setNodeScalarName("Node 1");
	test_node.setScalarValue(10.23);

	PROCESS_VISUALIZER visualizer;
	visualizer.addNodeOperation(&test_node);
	visualizer.showProcess();

	return 0;
}