#ifndef ARTIFICIALNEURALNETWORK_H_INCLUDED
#define ARTIFICIALNEURALNETWORK_H_INCLUDED

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <rapidxml.hpp>

#ifdef _OPENMP
	#include <omp.h>
	#define TIMERS double t_ini, t_fin
	#define GETTIME_INI t_ini = omp_get_wtime()
	#define GETTIME_FIN t_fin = omp_get_wtime()
	#define DIFTIME (t_fin - t_ini)
	#define OMP_ENABLED true
#else
	#if defined _WIN32 | defined _WIN64
		#define TIMERS time_t t_ini, t_fin
		#define GETTIME_INI time (&t_ini)
		#define GETTIME_FIN time (&t_fin)
		#define DIFTIME difftime(t_fin, t_ini)
	#else
		#include <sys/time.h>
		#define TIMERS struct timeval t_ini, t_fin
		#define GETTIME_INI gettimeofday( &t_ini, NULL)
		#define GETTIME_FIN gettimeofday( &t_fin, NULL)
		#define DIFTIME ((t_fin.tv_sec*1e6 + t_fin.tv_usec) - (t_ini.tv_sec*1e6 + t_ini.tv_usec) )/ 1e6
	#endif
	#define omp_get_num_threads() 1
	#define omp_set_num_threads(cores)
	#define omp_get_thread_num() 0
	#define OMP_ENABLED false
#endif

#include "Neuron.h"
#include "Input_pattern_class.h"

class ArtificialNeuralNetwork
{
public:
	ArtificialNeuralNetwork();
	
	~ArtificialNeuralNetwork();

	// Checks if the network is readyto be run
	bool checkNetworkState();

	// Prints the current solution (a 2D image with the hyperplanes that separates the data is intended)
	void printSolution();

	// Set the weights of the network
	void setNetworkWeights(double * src_weights_and_bias, const bool deep_copy = false);

	// Set the bias of the network
	void setNetworkWeightsDerivatives(double ** src_weights_and_bias_derivatives, const bool deep_copy = false);

	// Sets the filename where the states are saved
	void setNetworkLogFilename(const char * src_filename);

	// Loads the architecture of the network, and the value of all the bias and weights
	void loadNetworkData(const char * src_filename);

	// Saves the current state of the network to a file
	void saveNetworkState();

	// Predicts the output using the passed data:
	void predict(double * test_input, double * dst_prediction);//, const double threshold);
	
	Input_node * getOutputNode(const unsigned int src_output_index);

	double ** getNetworkWeightsDerivatives();
	double * getNetworkWeights();

	void resetNetworkTime();

protected:
	unsigned int inputs_count;
	unsigned int outputs_count;
	unsigned int neurons_count;
	unsigned int weights_count;
	
	Input_pattern ** network_input_nodes;
	Neuron ** network_neurons;
	Neuron ** network_output_nodes;

	double ** network_weights_derivative_values;
	double * network_weights_values;
	unsigned long long network_current_time;

private:
	char ann_log_filename[512];

	bool network_weights_derivatives_allocated;
	bool network_weights_allocated;

	// Allocates the memory for the weights and biases arrays
	void allocateNetworkMemory();
	void dumpWeightsToNetwork();

	void addInputNode(const unsigned int src_input_position);
	void addNeuron();
	void addOutputNode(const unsigned int src_neuron_position);
};

#endif //ARTIFICIALNEURALNETWORK_H_INCLUDED
