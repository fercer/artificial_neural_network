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
	NODE_SCALAR<double> test_node_1;
	test_node_1.setNodeScalarName("Node 1");
	test_node_1.setScalarValue(10.23);

	NODE_SCALAR<double> test_node_2;
	test_node_2.setNodeScalarName("Node 2");
	test_node_2.setScalarValue(9.77);

	SCALAR_SUM_NODES_OPERATION test_node_sum;
	test_node_sum.setNodeScalarName("Sum");
	test_node_sum.assignNodeValue(0, &test_node_1);
	test_node_sum.assignNodeValue(1, &test_node_2);
	test_node_sum.getScalarValue();

	SCALAR_MULT_NODES_OPERATION test_node_mult;
	test_node_mult.setNodeScalarName("Mult");
	test_node_mult.assignNodeValue(0, &test_node_1);
	test_node_mult.assignNodeValue(1, &test_node_sum);
	test_node_mult.getScalarValue();

	SCALAR_SUM_NODES_OPERATION test_node_mult_2;
	test_node_mult_2.setNodeScalarName("Mult");
	test_node_mult_2.assignNodeValue(0, &test_node_mult);
	test_node_mult_2.assignNodeValue(1, &test_node_sum);
	test_node_mult_2.getScalarValue();

	PROCESS_VISUALIZER visualizer;
	visualizer.addNodeOperation(&test_node_1);
	visualizer.addNodeOperation(&test_node_2);
	visualizer.addNodeOperation(&test_node_sum);
	visualizer.addNodeOperation(&test_node_mult);
	visualizer.addNodeOperation(&test_node_mult_2);

	visualizer.showProcess();

	return 0;
}