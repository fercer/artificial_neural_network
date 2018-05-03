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
	ArtificialNeuralNetwork(const ArtificialNeuralNetwork & src_ann);
	ArtificialNeuralNetwork & operator=(const ArtificialNeuralNetwork & src_ann);
	
	~ArtificialNeuralNetwork();

	// Prints the current solution (a 2D image with the hyperplanes that separates the data is intended)
	void printSolution();

	// Sets the filename where the states are saved
	void setNetworkLogFilename(const char * src_filename);

	// Loads the architecture of the network, and the value of all the bias and weights
	void loadNetworkData(const char * src_filename);

	// Saves the current state of the network to a file
	void saveNetworkState();

	// Predicts the output using the passed data:
	void predict(double * src_input_pattern_pointer, double * dst_prediction);//, const double threshold);

	Input_node * getOutputNode(const unsigned int src_output_index);

	void resetNetworkTime();

protected:
	unsigned int inputs_count;
	unsigned int outputs_count;
	unsigned int neurons_count;
	unsigned int weights_count;
	
	unsigned long long network_current_time;

	Input_pattern ** network_input_nodes;
	Neuron ** network_neurons;
	Neuron ** network_output_nodes;

	void setInputPatternPointer(double * src_input_pattern_pointer);

	// Set the weights of the network
	void setNetworkWeights(double *** src_weights_and_bias);

	// Set the bias of the network
	void setNetworkWeightsDerivatives(double **** src_weights_and_bias_derivatives);

	void predictWithDerivatives(double * src_input_pattern_pointer, double * dst_prediction);//, const double threshold);

private:
	char ann_log_filename[512];

	double * input_pattern_master_pointer;

	void addInputNode(const unsigned int src_input_position);
	void addNeuron();
	void addOutputNode(const unsigned int src_neuron_position);
};

#endif //ARTIFICIALNEURALNETWORK_H_INCLUDED
