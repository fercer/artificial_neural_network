#ifndef TRAINEDARTIFICIALNEURALNETWORK_CLASS_H_INCLUDED
#define TRAINEDARTIFICIALNEURALNETWORK_CLASS_H_INCLUDED

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <string.h>

#include "L1LossFunction_class.h"
#include "L2LossFunction_class.h"
#include "crossEntropyLossFunction_class.h"

#include "ArtificialNeuralNetwork.h"

class TrainedArtificialNeuralNetwork:
	public ArtificialNeuralNetwork
{

public:
	typedef enum LOSS_FUNCTION_TYPE {
		LF_L1_NORM = 0,
		LF_L2_NORM = 1,
		LF_CROSS_ENTROPY = 2
		// > Add other functions here <
	}LOSS_FUNCTION_TYPE;

	TrainedArtificialNeuralNetwork()
	{
		current_loss = std::numeric_limits<double>::max();
		current_epoch = 0;

		learning_rates = 0.0;
		momentums = 0.0;

		loss_functions = NULL;
		loss_functions_count = 0;

		log_filename[0] = '\0';
		first_log_method_entry = true;

		target_loss = 1e-3;
	}

	~TrainedArtificialNeuralNetwork() {
		if (loss_functions_count)
		{
			for (unsigned int loss_function_index = 0; loss_function_index < loss_functions_count; loss_function_index++)
			{
				delete *(loss_functions + loss_function_index);
			}
			free(loss_functions);
			free(loss_functions_types);
		}
	}

	void setLogFilename(const char * src_log_filename)
	{
		sprintf(log_filename, "%s", src_log_filename);
	}

	void setLearningRates(double src_learning_rates)
	{
		learning_rates = src_learning_rates;
	}

	void setMomentums(double src_momentums)
	{
		momentums = src_momentums;
	}
	
	void setEpochs(unsigned int src_n_epochs)
	{
		n_epochs = src_n_epochs;
	}

	void setTrainingData(double ** src_training_data)
	{
		training_data = src_training_data;
	}

	void setGroundtruthData(int ** src_groundtruth_data)
	{
		groundtruth_data = src_groundtruth_data;
	}

	void setTrainingDataSize(const unsigned int src_training_data_size)
	{
		training_data_size = src_training_data_size;
	}

	// Finds the network weights throught the optimization method
	virtual double trainNetwork(int save_each_n_epochs = -1) = 0;

	// Saves the current state of the update method
	virtual void saveState() = 0;

	void assignLossFunction(const LOSS_FUNCTION_TYPE src_loss_function_type)
	{
		if (loss_functions_count > 0)
		{
			LossFunction ** swap_loss_functions = loss_functions;
			loss_functions = (LossFunction**)malloc((loss_functions_count + 1) * sizeof(LossFunction*));
			memmove(loss_functions, swap_loss_functions, loss_functions_count * sizeof(LossFunction*));
			free(swap_loss_functions);

			LOSS_FUNCTION_TYPE * swap_loss_functions_types = loss_functions_types;
			loss_functions_types = (LOSS_FUNCTION_TYPE *)malloc((loss_functions_count + 1) * sizeof(LOSS_FUNCTION_TYPE));
			memmove(loss_functions_types, swap_loss_functions_types, loss_functions_count * sizeof(LOSS_FUNCTION_TYPE));
			free(swap_loss_functions_types);
		}
		else
		{
			loss_functions = (LossFunction**)malloc(sizeof(LossFunction*));
			loss_functions_types = (LOSS_FUNCTION_TYPE *)malloc(sizeof(LOSS_FUNCTION_TYPE));
		}

		*(loss_functions_types + loss_functions_count) = src_loss_function_type;
		switch (src_loss_function_type)
		{
		case LF_L1_NORM:
			*(loss_functions + loss_functions_count) = new L1LossFunction;
			break;

		case LF_L2_NORM:
			*(loss_functions + loss_functions_count) = new L2LossFunction;
			break;

		case LF_CROSS_ENTROPY:
			*(loss_functions + loss_functions_count) = new crossEntropyLossFunction;
			break;
		}
		(*(loss_functions + loss_functions_count))->setOutputNode(*(network_output_nodes + loss_functions_count));
		loss_functions_count++;
	}

	void setTargetLoss(const double src_target_loss)
	{
		target_loss = src_target_loss;
	}

protected:
	char log_filename[512];
	bool first_log_method_entry;

	unsigned int training_data_size;
	unsigned int n_epochs;

	double target_loss;
	double current_loss;
	unsigned int current_epoch;

	double learning_rates;
	double momentums;

	double ** training_data;
	int ** groundtruth_data;

	LOSS_FUNCTION_TYPE * loss_functions_types;
	LossFunction ** loss_functions;
	unsigned int loss_functions_count;
};

#endif // TRAINEDARTIFICIALNEURALNETWORK_CLASS_H_INCLUDED