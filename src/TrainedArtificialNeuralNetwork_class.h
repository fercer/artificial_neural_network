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

class TrainedArtificialNeuralNetwork
{

public:
	TrainedArtificialNeuralNetwork()
	{
		current_loss = std::numeric_limits<double>::max();
		current_epoch = 0;

		learning_rates = 0.0;
		momentums = 0.0;

		log_filename[0] = '\0';
		first_log_method_entry = true;

		target_loss = 1e-3;

		my_ann = new ArtificialNeuralNetwork;
	}

	~TrainedArtificialNeuralNetwork()
	{
		delete my_ann;		
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
	virtual double trainNetwork(const int save_each_n_epochs = -1, const int restart_time_each_n_epochs = 1) = 0;

	// Saves the current state of the update method
	virtual void saveState() = 0;
	
	void setTargetLoss(const double src_target_loss)
	{
		target_loss = src_target_loss;
	}

	ArtificialNeuralNetwork getTrainedANN()
	{
		return *my_ann;
	}

	/// Interface to work with the artificial neural network:
	void setNetworkLogFilename(const char * src_filename)
	{
		my_ann->setNetworkLogFilename(src_filename);
	}

	void loadNetworkData(const char * src_filename)
	{
		my_ann->loadNetworkData(src_filename);

		inputs_count = my_ann->getInputsCount();
		outputs_count = my_ann->getOutputsCount();
		neurons_count = my_ann->getNeuronsCount();
		weights_count = my_ann->getWeightsCount();
	}

	void loadNetworkData(const ArtificialNeuralNetwork & src_ann)
	{
		*my_ann = src_ann;

		inputs_count = my_ann->getInputsCount();
		outputs_count = my_ann->getOutputsCount();
		neurons_count = my_ann->getNeuronsCount();
		weights_count = my_ann->getWeightsCount();
	}

	void assignLossFunction(const LossFunction::LOSS_FUNCTION_TYPE src_loss_function_type)
	{
		my_ann->assignLossFunction(src_loss_function_type);
	}
	
protected:
	ArtificialNeuralNetwork * my_ann;

	unsigned int inputs_count;
	unsigned int outputs_count;
	unsigned int neurons_count;
	unsigned int weights_count;

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
};

#endif // TRAINEDARTIFICIALNEURALNETWORK_CLASS_H_INCLUDED