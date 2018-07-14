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
	IMAGE_PROCESS_HANDLER img_process;
	img_process.setFilename("single_pool.xml");
	img_process.loadProcess();

	img_process.runProcess();

	/*
	NODE_SCALAR<double> test_node;
	test_node.setNodeScalarName("Node 1");
	test_node.setScalarValue(10.23);

	NODE_SCALAR<char*> test_node_2;
	test_node_2.setNodeScalarName("Node 2");
	test_node_2.setScalarValue("Testing node 2");
	
	PROCESS_VISUALIZER visualizer;
	visualizer.addNodeOperation(&test_node);
	visualizer.addNodeOperation(&test_node_2);
	visualizer.showProcess();
	*/
	return 0;
}