#include <stdlib.h>
#include <stdio.h>

#include "ArtificialNeuralNetwork.h"
#include "backpropagationBasedANN_class.h"
#include "data_handler_class.h"

int main(int argc, char * argv[])
{
	backpropagationBasedANN bpt_ann_test;
	//bpt_ann_test.loadNetworkData("XOR_trained_network.xml");
	//bpt_ann_test.loadNetworkData("Trained_network_test.xml");
	bpt_ann_test.loadNetworkData("iris_network.xml");

	data_handler training_data;
	//training_data.setFilename("trainingDataXOR_slim.txt");
	training_data.setFilename("training_set.dat");
	training_data.loadData();

	const unsigned int training_size = training_data.getDataSize();
	const unsigned int inputs_count = training_data.getInputSize();
	const unsigned int outputs_count = training_data.getOutputSize();

	double **training_patterns_database = training_data.getInputData();
	int **training_outputs_database = training_data.getOutputData();
	
	bpt_ann_test.setBackpropagationMethod(backpropagationBasedANN::BPM_LEVENBERG_MARQUARDT);
	bpt_ann_test.setEpochs(1795);
	bpt_ann_test.setLearningRates(0.05);
	bpt_ann_test.setMomentums(0.9);

	bpt_ann_test.setTargetLoss(0.01);
	bpt_ann_test.setTrainingDataSize(training_size);
	bpt_ann_test.setTrainingData(training_patterns_database);
	bpt_ann_test.setGroundtruthData(training_outputs_database);

	bpt_ann_test.setNetworkLogFilename("Trained_network_test.xml");

	for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
	{
		bpt_ann_test.assignLossFunction(LossFunction::LF_L2_NORM);
	}

	bpt_ann_test.trainNetwork();
	bpt_ann_test.saveState();

	ArtificialNeuralNetwork trained_ann = bpt_ann_test.getTrainedANN();

	trained_ann.setNetworkLogFilename("testing_network_architecture.xml");
	trained_ann.saveNetworkState();


	data_handler testing_data;
	testing_data.setFilename("testing_set.dat");
	testing_data.loadData();

	const unsigned int testing_size = testing_data.getDataSize();

	double * prediction = (double*)malloc(outputs_count * sizeof(double));

	double **testing_patterns_database = testing_data.getInputData();
	int **testing_outputs_database = testing_data.getOutputData();

	for (unsigned int pattern_index = 0; pattern_index < testing_size; pattern_index++)
	{
		double *testing_pattern = *(testing_patterns_database + pattern_index);
		int *ground_truth = *(testing_outputs_database + pattern_index);

		trained_ann.predict(testing_pattern, prediction);

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