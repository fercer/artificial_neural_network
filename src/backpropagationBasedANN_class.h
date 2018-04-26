#ifndef BACKPROPAGATIONBASEDANN_CLASS_H_INCLUDED
#define BACKPROPAGATIONBASEDANN_CLASS_H_INCLUDED

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <string.h>

#include <math.h>

#include "random_numbers_generator.h"
#include "TrainedArtificialNeuralNetwork_class.h"

class backpropagationBasedANN :
	public TrainedArtificialNeuralNetwork
{
public:
	typedef enum BACKPROPAGATION_METHOD {
		BPM_GRADIENT_DESCENT = 0,
		BPM_LEVENBERG_MARQUARDT = 1
	} BACKPROPAGATION_METHOD;
	
	backpropagationBasedANN();
	~backpropagationBasedANN();

	// Finds the network weights throught the optimization method
	double trainNetwork(int save_each_n_epochs = -1);

	void saveState();

	void setBackpropagationMethod(BACKPROPAGATION_METHOD src_weights_update_method);


private:
	typedef bool (backpropagationBasedANN::*COMPUTE_EPOCH_METHOD) ();

	bool memory_already_allocated;
	bool hessian_matrix_was_required;
	double * weights_deltas;

	double * jacobian_error_derivative_product;
	double * hessian_matrix;
	double * previous_weights_values;
	double * previous_jacobian_error_derivative_product;
	double * previous_hessian_matrix;

#ifdef _OPENMP
	int *** groundtruth_data_parallel;
	double *** training_data_parallel;

	Input_pattern *** network_input_nodes_parallel;
	Neuron *** network_neurons_parallel;
	Neuron *** network_outputs_parallel;
	LossFunction *** loss_functions_parallel;

	double ** hessian_matrix_parallel;
	double ** jacobian_error_derivative_product_parallel;
	double *** network_weights_derivative_values_parallel;
	double ** network_weights_values_parallel;

	unsigned int training_data_size_per_thread;
	unsigned int available_threads_count;
#endif // _OPENMP

	double mu_value;
	double max_mu_value;
	double mu_increasing_factor;
	double mu_decreasing_factor;

	STAUS * random_number_generator_seed;

	COMPUTE_EPOCH_METHOD computeEpoch;

	int allocateMethodMemory();

	bool computeEpoch_gradient_descent();
	bool computeEpoch_levenberg_marquardt();
};

#endif //BACKPROPAGATIONBASEDANN_CLASS_H_INCLUDED