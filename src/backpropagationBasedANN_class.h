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
		BPM_LEVENBERG_MARQUARDT = 1,
		BPM_MINI_BACTH_GRADIENT_DESCENT = 2,
		BPM_STOCHASTIC_GRADIENT_DESCENT = 3,
		BPM_MINI_BATCH_STOCHASTIC_GRADIENT_DESCENT = 4
	} BACKPROPAGATION_METHOD;
	
	backpropagationBasedANN();
	~backpropagationBasedANN();

	// Finds the network weights throught the optimization method
	double trainNetwork(const int save_each_n_epochs = -1, const int restart_time_each_n_epochs = -1);

	void saveState();

	void setBackpropagationMethod(const BACKPROPAGATION_METHOD src_weights_update_method);

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

	// Trained network data:
	double *** training_data_threads;
	int *** groundtruth_data_threads;

	ArtificialNeuralNetwork ** ann_threads;
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

	int * indices_order;
	bool randomize_training_indices;

#ifdef _OPENMP
	int allocateNetworkArchitectureParallel();
	int allocateTrainingDataParallel();
	int allocateLevenbergMarquardtParallel();

	void deallocateNetworkArchitectureParallel();
	void deallocateTrainingDataParallel();
	void deallocateLevenbergMarquardtParallel();
#endif //_OPENMP

	int allocateMethodMemory();

	bool computeEpoch_gradient_descent();
	bool computeEpoch_levenberg_marquardt();
	bool computeEpoch_mini_batch_gradient_descent();

	void randomizeTrainingIndices();
};

#endif //BACKPROPAGATIONBASEDANN_CLASS_H_INCLUDED