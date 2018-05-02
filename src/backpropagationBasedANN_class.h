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

	double ** network_weights_values;
	double ** network_weights_derivatives_values;

	double *** network_weights_values_pointer;
	double *** network_weights_derivatives_values_pointer;

	double ** weights_deltas;
	
	double ** jacobian_error_derivative_product;
	double ** hessian_matrix;
	double ** previous_weights_values;
	double ** previous_jacobian_error_derivative_product;
	double ** previous_hessian_matrix;
	
	double mu_value;
	double max_mu_value;
	double mu_increasing_factor;
	double mu_decreasing_factor;

	STAUS * random_number_generator_seed;

	COMPUTE_EPOCH_METHOD computeEpoch;

	int allocateMethodMemory();

	bool computeEpoch_gradient_descent();
	bool computeEpoch_levenberg_marquardt();

	double productOutputErrorContribution(const unsigned int src_neuron_index_a, const unsigned int src_weighted_input_index_a, const unsigned int src_neuron_index_b, const unsigned int src_weighted_input_index_b, double *** src_network_weights_derivatives_values_master_pointer);

	double computeCholeskyRowValue(const unsigned int src_neuron_index, const unsigned int src_weighted_input_index, double ** src_hessian_matrix);
};

#endif //BACKPROPAGATIONBASEDANN_CLASS_H_INCLUDED