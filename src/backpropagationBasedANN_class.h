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
	double trainNetwork(const int save_each_n_epochs = -1, const int restart_time_each_n_epochs = -1);

	void saveState();

	void setBackpropagationMethod(BACKPROPAGATION_METHOD src_weights_update_method);

private:
	typedef bool (backpropagationBasedANN::*COMPUTE_EPOCH_METHOD) ();

	bool memory_already_allocated;
	bool hessian_matrix_was_required;
	
	double * network_weights_values;
	double ** network_weights_derivatives_values;

	double ** network_weights_pointers_manager;
	double *** network_weights_derivatives_pointers_manager;

#ifdef _OPENMP
	unsigned int available_threads;
	unsigned int batch_size_per_thread;

	double ** network_weights_values_threads;
	double *** network_weights_derivatives_values_threads;

	double *** network_weights_pointers_manager_threads;
	double **** network_weights_derivatives_pointers_manager_threads;

	double ** jacobian_error_derivative_product_threads;
	double ** hessian_matrix_threads;

	// Network architecture:
	Input_pattern *** network_input_nodes_threads;
	Neuron *** network_neurons_threads;
	Neuron *** network_output_nodes_threads;

	// Trained network data:
	double *** training_data_threads;
	double ** input_pattern_master_pointer_threads;

	int ** groundtruth_master_pointer_threads;
	int *** groundtruth_data_threads;

	LOSS_FUNCTION_LIST_NODE * loss_functions_head_node_threads;
#endif

	double * weights_deltas;
	
	double * jacobian_error_derivative_product;
	double * hessian_matrix;
	double * previous_weights_values;
	double * previous_jacobian_error_derivative_product;
	double * previous_hessian_matrix;

	double mu_value;
	double max_mu_value;
	double mu_increasing_factor;
	double mu_decreasing_factor;

	STAUS * random_number_generator_seed;

	COMPUTE_EPOCH_METHOD computeEpoch;

#ifdef _OPENMP
	int allocateLossFunctionsParallel();
	int allocateNetworkArchitectureParallel();
	int allocateTrainingDataParallel();
	int allocateLevenbergMarquardtParallel();

	void deallocateLossFunctionsParallel();
	void deallocateNetworkArchitectureParallel();
	void deallocateTrainingDataParallel();
	void deallocateLevenbergMarquardtParallel();
#endif //_OPENMP

	int allocateMethodMemory();

	bool computeEpoch_gradient_descent();
	bool computeEpoch_levenberg_marquardt();
};

#endif //BACKPROPAGATIONBASEDANN_CLASS_H_INCLUDED