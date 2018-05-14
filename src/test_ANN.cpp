#include <stdlib.h>
#include <stdio.h>

#include "ArtificialNeuralNetwork.h"
#include "backpropagationBasedANN_class.h"
#include "data_handler_class.h"

int main(int argc, char * argv[])
{
	backpropagationBasedANN bpt_ann_test;
	//bpt_ann_test.loadNetworkData("XOR_trained_network.xml");
	bpt_ann_test.loadNetworkData("Trained_network_test.xml");

	data_handler testing_data;
	testing_data.setFilename("trainingDataXOR_slim.txt");
	testing_data.loadData();

	const unsigned int testing_size = testing_data.getDataSize();
	const unsigned int inputs_count = testing_data.getInputSize();
	const unsigned int outputs_count = testing_data.getOutputSize();

	double **testing_patterns_database = testing_data.getInputData();
	int **testing_outputs_database = testing_data.getOutputData();
	
	bpt_ann_test.setBackpropagationMethod(backpropagationBasedANN::BPM_LEVENBERG_MARQUARDT);
	bpt_ann_test.setEpochs(1000);
	bpt_ann_test.setLearningRates(0.05);
	bpt_ann_test.setMomentums(0.9);
	bpt_ann_test.setTargetLoss(0.001);
	bpt_ann_test.setTrainingDataSize(testing_size);
	bpt_ann_test.setTrainingData(testing_patterns_database);
	bpt_ann_test.setGroundtruthData(testing_outputs_database);
	bpt_ann_test.setNetworkLogFilename("Trained_network_test.xml");

	for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
	{
		bpt_ann_test.assignLossFunction(LossFunction::LF_L2_NORM);
	}

	bpt_ann_test.trainNetwork();
	bpt_ann_test.saveNetworkState();

	double * prediction = (double*)malloc(outputs_count * sizeof(double));
	
	for (unsigned int pattern_index = 0; pattern_index < testing_size; pattern_index++)
	{
		double *testing_pattern = *(testing_patterns_database + pattern_index);
		int *ground_truth = *(testing_outputs_database + pattern_index);

		bpt_ann_test.predict(testing_pattern, prediction);

		for (unsigned int input_index = 0; input_index < inputs_count; input_index++)
		{
			printf("%f ", *(testing_pattern + input_index));
		}
		printf("\n");
		for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
		{
			printf("%f ", *(prediction + output_index));
		}
		printf("\n");
		for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
		{
			printf("%i ", *(ground_truth + output_index));
		}
		printf("\n-----------------------------\n");
	}

	free(prediction);

	return 1;
}