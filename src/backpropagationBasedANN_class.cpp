#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "random_numbers_generator.h"
#include "backpropagationBasedANN_class.h"

backpropagationBasedANN::backpropagationBasedANN()
{
	computeEpoch = &backpropagationBasedANN::computeEpoch_gradient_descent;

	weights_deltas = NULL;
	network_weights_values = NULL;
	network_weights_derivatives_values = NULL;
	
	network_weights_pointers_manager = NULL;
	network_weights_derivatives_pointers_manager = NULL;

	memory_already_allocated = false;
	hessian_matrix_was_required = false;

	previous_weights_values = NULL;
	jacobian_error_derivative_product = NULL;
	hessian_matrix = NULL;
	previous_jacobian_error_derivative_product = NULL;
	previous_hessian_matrix = NULL;

	mu_value = 0.001;
	max_mu_value = 1e10;
	mu_increasing_factor = 10.0;
	mu_decreasing_factor = 0.1;

	momentums = 0.9;
	learning_rates = 0.05;

	random_number_generator_seed = initSeed(777);

	omp_set_num_threads(2);

#ifdef _OPENMP
#pragma omp parallel
	{
#pragma omp master
		{
			available_threads = omp_get_num_threads();
		}
	}
#endif
}



backpropagationBasedANN::~backpropagationBasedANN()
{
	if (memory_already_allocated)
	{
		for (unsigned int weight_index = 0; weight_index < weights_count; weight_index++)
		{
			free(*(network_weights_derivatives_values + weight_index));
		}
		free(network_weights_derivatives_values);
		free(network_weights_values);
		
		if (hessian_matrix_was_required)
		{
			free(previous_weights_values);
			free(jacobian_error_derivative_product);
			free(hessian_matrix);
			free(previous_jacobian_error_derivative_product);
			free(previous_hessian_matrix);

#ifdef _OEPNMP
			deallocateLevenbergMarquardtParallel();
#endif //_OPENMP
		}
		else
		{
			free(weights_deltas);
		}

#ifdef _OEPNMP
		deallocateLossFunctionsParallel();
		deallocateTrainingDataParallel();
		deallocateNetworkArchitectureParallel();
#endif //_OPENMP
	}
	
	free(random_number_generator_seed);
}


#ifdef _OPENMP
int backpropagationBasedANN::allocateLossFunctionsParallel()
{
	// Allocate memory for the loss functions:
	loss_functions_head_node_threads = (LOSS_FUNCTION_LIST_NODE*)malloc(available_threads * sizeof(LOSS_FUNCTION_LIST_NODE));
	LOSS_FUNCTION_LIST_NODE * loss_functions_tail_node_threads;

	LOSS_FUNCTION_LIST_NODE * current_loss_function_node;
	LOSS_FUNCTION_LIST_NODE * next_loss_function_node = loss_functions_head_node.next_loss_function_node;

	while (next_loss_function_node)
	{
		current_loss_function_node = next_loss_function_node;
		next_loss_function_node = current_loss_function_node->next_loss_function_node;

		const unsigned int current_loss_function_index = current_loss_function_node->loss_function_pointer->getGlobalOutputIndex();

		for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
		{
			loss_functions_tail_node_threads = loss_functions_head_node_threads + thread_id;

			loss_functions_tail_node_threads->next_loss_function_node = new LOSS_FUNCTION_LIST_NODE;
			loss_functions_tail_node_threads = loss_functions_tail_node_threads->next_loss_function_node;

			loss_functions_tail_node_threads->next_loss_function_node = NULL;

			switch (current_loss_function_node->loss_function_pointer->getLossFunctionType())
			{
			case LossFunction::LF_L1_NORM:
				loss_functions_tail_node_threads->loss_function_pointer = new L1LossFunction(*(L1LossFunction*)(current_loss_function_node->loss_function_pointer));
				break;

			case LossFunction::LF_L2_NORM:
				loss_functions_tail_node_threads->loss_function_pointer = new L2LossFunction(*(L2LossFunction*)(current_loss_function_node->loss_function_pointer));
				break;

			case LossFunction::LF_CROSS_ENTROPY:
				loss_functions_tail_node_threads->loss_function_pointer = new crossEntropyLossFunction(*(crossEntropyLossFunction*)(current_loss_function_node->loss_function_pointer));
				break;
			}

			loss_functions_tail_node_threads->loss_function_pointer->setOutputNode(
				*(*(network_output_nodes_threads + thread_id) + current_loss_function_index));
			
			loss_functions_tail_node_threads->loss_function_pointer->setGlobalOutputIndex(current_loss_function_index);
			
			loss_functions_tail_node_threads->loss_function_pointer->setGroundtruth(groundtruth_master_pointer_threads + thread_id);
		}
	}

	return 1;
}



int backpropagationBasedANN::allocateNetworkArchitectureParallel()
{
	network_weights_values_threads = (double**)malloc(available_threads * sizeof(double*));
	network_weights_derivatives_values_threads = (double***)malloc(available_threads * sizeof(double**));

	network_weights_pointers_manager_threads = (double***)malloc(available_threads * sizeof(double**));
	network_weights_derivatives_pointers_manager_threads = (double****)malloc(available_threads * sizeof(double***));

	// Allocate memory for the copied architectures:
	network_input_nodes_threads = (Input_pattern***)malloc(available_threads * sizeof(Input_pattern**));
	network_neurons_threads = (Neuron***)malloc(available_threads * sizeof(Neuron**));
	network_output_nodes_threads = (Neuron***)malloc(available_threads * sizeof(Neuron**));

	for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
	{
		*(network_weights_values_threads + thread_id) = (double*)malloc(weights_count * sizeof(double));
		*(network_weights_derivatives_values_threads + thread_id) = (double**)malloc(weights_count * sizeof(double*));

		*(network_weights_pointers_manager_threads + thread_id) = (double**)malloc(neurons_count + sizeof(double*));
		*(network_weights_derivatives_pointers_manager_threads + thread_id) = (double***)malloc(neurons_count * sizeof(double**));

		// Copy the network's architecture 'available_threads' times:
		*(network_input_nodes_threads + thread_id) = (Input_pattern**)malloc(inputs_count * sizeof(Input_pattern*));
		*(network_neurons_threads + thread_id) = (Neuron**)malloc(neurons_count * sizeof(Neuron*));
		*(network_output_nodes_threads + thread_id) = (Neuron**)malloc(outputs_count * sizeof(Neuron*));

		for (unsigned int input_index = 0; input_index < inputs_count; input_index++)
		{
			*(*(network_input_nodes_threads + thread_id) + input_index) = new Input_pattern(**(network_input_nodes + input_index));

			(*(*(network_input_nodes_threads + thread_id) + input_index))->setInputPointer(input_pattern_master_pointer_threads + thread_id);
		}

		for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
		{
			*(*(network_neurons_threads + thread_id) + neuron_index) = new Neuron(**(network_neurons + neuron_index));

			const unsigned int current_neuron_inputs_count = (*(network_neurons + neuron_index))->getInputsCount();

			for (unsigned int weighted_input_index = 1; weighted_input_index < current_neuron_inputs_count; weighted_input_index++)
			{
				// Referenciate the neuron's inputs to the current thread's neurons:
				const unsigned int current_input_global_index = (*(*(network_neurons_threads + thread_id) + neuron_index))->getInputNodeGlobalIndex(weighted_input_index);

				switch ((*(*(network_neurons_threads + thread_id) + neuron_index))->getInputType(weighted_input_index))
				{
				case Weight_node::WIT_BIAS:
					break;
				case Weight_node::WIT_PATTERN:
					(*(*(network_neurons_threads + thread_id) + neuron_index))->setInputNodePointer(
						*(*(network_input_nodes_threads + thread_id) + current_input_global_index),
						weighted_input_index);
					break;
				case Weight_node::WIT_NEURON:
					(*(*(network_neurons_threads + thread_id) + neuron_index))->setInputNodePointer(
						*(*(network_neurons_threads + thread_id) + current_input_global_index),
						weighted_input_index);
					break;
				}
			}
		}

		for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
		{
			const unsigned int current_input_global_index = (*(network_output_nodes + output_index))->getGlobalNodeIndex();

			*(*(network_output_nodes_threads + thread_id) + output_index) = *(*(network_neurons_threads + thread_id) + current_input_global_index);
		}
	}

	unsigned int weight_index_base = 0;
	for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
	{
		const unsigned int current_neuron_inputs_count = (*(network_neurons + neuron_index))->getInputsCount();

		for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
		{
			// Copy the initial values to the current thread weights values:
			*(*(network_weights_values_threads + thread_id) + weight_index_base) = *(network_weights_values + weight_index_base);

			*(*(network_weights_derivatives_values_threads + thread_id) + weight_index_base) = (double*)malloc(outputs_count * sizeof(double));
			for (unsigned int weight_index = 1; weight_index < current_neuron_inputs_count; weight_index++)
			{
				*(*(network_weights_values_threads + thread_id) + weight_index_base + weight_index) = *(network_weights_values + weight_index_base + weight_index);

				*(*(network_weights_derivatives_values_threads + thread_id) + weight_index_base + weight_index) = (double*)malloc(outputs_count * sizeof(double));
			}

			*(*(network_weights_pointers_manager_threads + thread_id) + neuron_index) = *(network_weights_values_threads + thread_id) + weight_index_base;
			*(*(network_weights_derivatives_pointers_manager_threads + thread_id) + neuron_index) = *(network_weights_derivatives_values_threads + thread_id) + weight_index_base;

			// Make the copied network ofneurons to use the threads values pointers managers:
			(*(*(network_neurons_threads + thread_id) + neuron_index))->makeExternalWeightValues(network_weights_pointers_manager_threads + thread_id, network_weights_derivatives_pointers_manager_threads + thread_id);
		}
		weight_index_base += current_neuron_inputs_count;
	}

	return 1;
}



int backpropagationBasedANN::allocateTrainingDataParallel()
{
	// Allocate memory for the training data batches:
	groundtruth_data_threads = (int***)malloc(available_threads * sizeof(int**));
	groundtruth_master_pointer_threads = (int**)malloc(available_threads * sizeof(int*));

	training_data_threads = (double***)malloc(available_threads * sizeof(double**));
	input_pattern_master_pointer_threads = (double**)malloc(available_threads * sizeof(double*));

	unsigned int pattern_index_base = 0;
	for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
	{
		const unsigned int current_thread_batch_size = ((thread_id + 1) * batch_size_per_thread) > training_data_size ? (training_data_size - thread_id*batch_size_per_thread) : batch_size_per_thread;

		*(groundtruth_data_threads + thread_id) = (int**)malloc(current_thread_batch_size * sizeof(int*));
		*(training_data_threads + thread_id) = (double**)malloc(current_thread_batch_size * sizeof(double*));

		for (unsigned int pattern_index = 0; pattern_index < current_thread_batch_size; pattern_index++, pattern_index_base++)
		{
			*(*(groundtruth_data_threads + thread_id) + pattern_index) = (int*)malloc(outputs_count * sizeof(int));
			memcpy(*(*(groundtruth_data_threads + thread_id) + pattern_index),
				*(groundtruth_data + pattern_index_base),
				outputs_count * sizeof(int));

			*(*(training_data_threads + thread_id) + pattern_index) = (double*)malloc(inputs_count * sizeof(double));
			memcpy(*(*(training_data_threads + thread_id) + pattern_index),
				*(training_data + pattern_index_base),
				inputs_count * sizeof(double));
		}
	}

	return 1;
}


int backpropagationBasedANN::allocateLevenbergMarquardtParallel()
{	
	// Allocate memory for the weights update directions:
	hessian_matrix_threads = (double**)malloc(available_threads * sizeof(double*));
	jacobian_error_derivative_product_threads = (double**)malloc(available_threads * sizeof(double*));
	for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
	{
		*(hessian_matrix_threads + thread_id) = (double*)malloc(weights_count * (weights_count + 1) / 2 * sizeof(double));
		*(jacobian_error_derivative_product_threads + thread_id) = (double*)malloc(weights_count * sizeof(double));
	}

	return 1;
}



void backpropagationBasedANN::deallocateLossFunctionsParallel()
{
	LOSS_FUNCTION_LIST_NODE * current_loss_function_node;
	LOSS_FUNCTION_LIST_NODE * next_loss_function_node;
	for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
	{
		next_loss_function_node = (loss_functions_head_node_threads + thread_id)->next_loss_function_node;

		while (next_loss_function_node)
		{
			current_loss_function_node = next_loss_function_node;
			next_loss_function_node = current_loss_function_node->next_loss_function_node;

			delete current_loss_function_node->loss_function_pointer;
			delete current_loss_function_node;
		}
	}

	free(loss_functions_head_node_threads);
}



void backpropagationBasedANN::deallocateNetworkArchitectureParallel()
{
	for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
	{
		for (unsigned int input_index = 0; input_index < inputs_count; input_index++)
		{
			delete *(*(network_input_nodes_threads + thread_id) + input_index);
		}

		for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
		{
			delete *(*(network_neurons_threads + thread_id) + neuron_index);
		}

		free(*(network_input_nodes_threads + thread_id));
		free(*(network_neurons_threads + thread_id));
		free(*(network_output_nodes_threads + thread_id));

		free(*(network_weights_values_threads + thread_id));
		free(*(network_weights_derivatives_values_threads + thread_id));

		free(*(network_weights_pointers_manager_threads + thread_id));
		free(*(network_weights_derivatives_pointers_manager_threads + thread_id));
	}

	free(network_weights_values_threads);
	free(network_weights_derivatives_values_threads);

	free(network_weights_pointers_manager_threads);
	free(network_weights_derivatives_pointers_manager_threads);

	free(network_input_nodes_threads);
	free(network_neurons_threads);
	free(network_output_nodes_threads);
}



void backpropagationBasedANN::deallocateTrainingDataParallel()
{
	for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
	{
		const unsigned int current_thread_batch_size = ((thread_id + 1) * batch_size_per_thread) > training_data_size ? (training_data_size - thread_id*batch_size_per_thread) : batch_size_per_thread;

		for (unsigned int pattern_index = 0; pattern_index < current_thread_batch_size; pattern_index++)
		{
			free(*(*(groundtruth_data_threads + thread_id) + pattern_index));
			free(*(*(training_data_threads + thread_id) + pattern_index));
		}

		free(*(groundtruth_data_threads + thread_id));
		free(*(training_data_threads + thread_id));
	}

	free(groundtruth_data_threads);
	free(groundtruth_master_pointer_threads);
	free(training_data_threads);
	free(input_pattern_master_pointer_threads);
}


void backpropagationBasedANN::deallocateLevenbergMarquardtParallel()
{
	for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
	{
		free(*(hessian_matrix_threads + thread_id));
		free(*(jacobian_error_derivative_product_threads + thread_id));
	}

	free(hessian_matrix_threads);
	free(jacobian_error_derivative_product_threads);
}
#endif //_OPENMP


int backpropagationBasedANN::allocateMethodMemory()
{
	/* Successfull allocation of memory returns 1, otherwise, returns 0 */
	if (memory_already_allocated)
	{
		return 1;
	}

	if (neurons_count == 0 || weights_count == 0 || inputs_count == 0 || outputs_count == 0)
	{
		return 0;
	}
	
	// Allocate memory for the weights of the network
	network_weights_values = (double*)malloc(weights_count * sizeof(double));
	network_weights_derivatives_values = (double**)malloc(weights_count * sizeof(double*));

	network_weights_pointers_manager = (double**)malloc(neurons_count * sizeof(double*));
	network_weights_derivatives_pointers_manager = (double***)malloc(neurons_count * sizeof(double**));
	unsigned int weight_index_base = 0;
	for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
	{
		const unsigned int current_neuron_inputs_count = (*(network_neurons + neuron_index))->getInputsCount();

		*(network_weights_derivatives_values + weight_index_base) = (double*)malloc(outputs_count * sizeof(double));

		// Initialize the bias in 0.0
		*(network_weights_values + weight_index_base) = 0.0;
		for (unsigned int weight_index = 1; weight_index < current_neuron_inputs_count; weight_index++)
		{
			*(network_weights_derivatives_values + weight_index_base + weight_index) = (double*)malloc(outputs_count * sizeof(double));

			// Initialize the weights with a normal standard distribution
			*(network_weights_values + weight_index_base + weight_index) = anorm_est(random_number_generator_seed);
		}

		*(network_weights_pointers_manager + neuron_index) = network_weights_values + weight_index_base;
		*(network_weights_derivatives_pointers_manager + neuron_index) = network_weights_derivatives_values + weight_index_base;

		(*(network_neurons + neuron_index))->makeExternalWeightValues(&network_weights_pointers_manager, &network_weights_derivatives_pointers_manager);

		weight_index_base += current_neuron_inputs_count;
	}


	if (hessian_matrix_was_required)
	{
		// Allocate memory for the weights update directions:
		hessian_matrix = (double*)malloc(weights_count * (weights_count + 1) / 2 * sizeof(double));
		previous_hessian_matrix = (double*)malloc(weights_count * (weights_count + 1) / 2 * sizeof(double));

		jacobian_error_derivative_product = (double*)malloc(weights_count * sizeof(double));
		previous_jacobian_error_derivative_product = (double*)malloc(weights_count * sizeof(double));
		previous_weights_values = (double*)malloc(weights_count * sizeof(double));

#ifdef _OPENMP
		batch_size_per_thread = (training_data_size + 1) / available_threads;
		allocateTrainingDataParallel();
		allocateNetworkArchitectureParallel();
		allocateLossFunctionsParallel();
		allocateLevenbergMarquardtParallel();
#endif //_OPENMP
	}
	else
	{
		// Allocate memory for the weights update directions:
		weights_deltas = (double*)malloc(weights_count * sizeof(double));
	}

	memory_already_allocated = true;
	return 1;
}



bool backpropagationBasedANN::computeEpoch_gradient_descent()
{
	// Reset the time to prevent overflow
	LOSS_FUNCTION_LIST_NODE * current_loss_function_node;
	LOSS_FUNCTION_LIST_NODE * next_loss_function_node;

	double squared_gradient_norm;

	// Reset the weights and bias delta values to 0:
	memset(weights_deltas, 0, weights_count * sizeof(double));

	TIMERS;
	GETTIME_INI;
	double total_epoch_loss = 0.0;
	for (unsigned int pattern_index = 0; pattern_index < training_data_size; pattern_index++, network_current_time++)
	{
		// Use the current pattern to feed the network:
		setInputPatternPointer(*(training_data + pattern_index));
		groundtruth_master_pointer = *(groundtruth_data + pattern_index);

		// Perform the Feed forward:
		next_loss_function_node = loss_functions_head_node.next_loss_function_node;
		while (next_loss_function_node)
		{
			current_loss_function_node = next_loss_function_node;
			next_loss_function_node = current_loss_function_node->next_loss_function_node;

			total_epoch_loss += current_loss_function_node->loss_function_pointer->computeLossWithDerivatives(network_current_time);

			// Backpropagate the error contribution:
			current_loss_function_node->loss_function_pointer->backpropagateErrorDerivative();
		}

		// Backpropagate the neuron's error contribution:
		for (int neuron_index = (neurons_count - 1); neuron_index >= 0; neuron_index--)
		{
			(*(network_neurons + neuron_index))->backpropagateNodeError();
		}
		squared_gradient_norm = 0.0;
		for (unsigned int weight_index = 0; weight_index < weights_count; weight_index++)
		{
			double weight_error_contribution = 0.0;
			for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
			{
				weight_error_contribution += *(*(network_weights_derivatives_values + weight_index) + output_index);
			}

			*(weights_deltas + weight_index) = *(weights_deltas + weight_index) * momentums + learning_rates * weight_error_contribution;

			// update the weights and bias values:
			*(network_weights_values + weight_index) = *(network_weights_values + weight_index) - *(weights_deltas + weight_index);
			squared_gradient_norm += *(weights_deltas + weight_index) * *(weights_deltas + weight_index);
		}
	}
	GETTIME_FIN;
	printf("Pattern processed in %f s\n", DIFTIME);

	current_loss = total_epoch_loss / (double)training_data_size;

	if ((current_loss < target_loss) || (squared_gradient_norm < 1e-16))
	{
		return false;
	}

	return true;
}


#ifndef _OPENMP
bool backpropagationBasedANN::computeEpoch_levenberg_marquardt()
{
	// Reset the time to prevent overflow
	LOSS_FUNCTION_LIST_NODE * current_loss_function_node;
	LOSS_FUNCTION_LIST_NODE * next_loss_function_node;
	
	double squared_gradient_norm;
	unsigned int worsening_count = 0;
	double training_epoch_loss;
	double total_epoch_loss = 0.0;
	unsigned int output_index;

	// Set the hessian and jacobian arrays to 0, to perform a cummulated sum of their values:
	memset(hessian_matrix, 0, weights_count * (weights_count + 1) / 2 * sizeof(double));
	memset(jacobian_error_derivative_product, 0, weights_count * sizeof(double));

	// Perform the feed-forward process for each pattern in the training set:
	for (unsigned int pattern_index = 0; pattern_index < training_data_size; pattern_index++, network_current_time++)
	{
		// Use the current pattern to feed the network:
		setInputPatternPointer(*(training_data + pattern_index));
		groundtruth_master_pointer = *(groundtruth_data + pattern_index);

		// Perform the Feed forward:
		next_loss_function_node = loss_functions_head_node.next_loss_function_node;
		output_index = 0;
		while(next_loss_function_node)
		{
			current_loss_function_node = next_loss_function_node;
			next_loss_function_node = current_loss_function_node->next_loss_function_node;

			total_epoch_loss += current_loss_function_node->loss_function_pointer->computeLossWithDerivatives(network_current_time);

			/* This is necessary to back propagate only the error contribution 
			to each isolated loss function.
			*/
			(*(network_output_nodes + output_index))->addNodeErrorContribution(1.0, output_index);

			output_index++;
		}

		// Backpropagate the neuron's error contribution:
		for (int neuron_index = (neurons_count - 1); neuron_index >= 0; neuron_index--)
		{
			// Backpropagate the error contribution:
			(*(network_neurons + neuron_index))->backpropagateNodeError();
		}

		for (unsigned int weight_index = 0; weight_index < weights_count; weight_index++)
		{
			// Compute the product between the Jacobian matrix and the vector of errors:
			output_index = 0;
			next_loss_function_node = loss_functions_head_node.next_loss_function_node;
			double error_contribution_product = 0.0;
			while(next_loss_function_node)
			{
				current_loss_function_node = next_loss_function_node;
				next_loss_function_node = current_loss_function_node->next_loss_function_node;

				*(jacobian_error_derivative_product + weight_index) =
					*(jacobian_error_derivative_product + weight_index) +
					*(*(network_weights_derivatives_values + weight_index) + output_index) *
					current_loss_function_node->loss_function_pointer->getErrorDerivative();

				/* Compute the hessian matrix entry that corresponds to 
				the product of the current weight and the first weight of the network:
				*/
				error_contribution_product += *(*(network_weights_derivatives_values + weight_index) + output_index) *
					*(*network_weights_derivatives_values + output_index);

				output_index++;
			}
			const unsigned int weight_index_base = weight_index * (weight_index + 1) / 2;
			*(hessian_matrix + weight_index_base) = *(hessian_matrix + weight_index_base) + error_contribution_product;

			// Compute the jacobian matrix product with its self transpose:
			for (unsigned int weight_index_j = 0; weight_index_j <= weight_index; weight_index_j++)
			{
				double error_contribution_product = 0.0;
				for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
				{
					error_contribution_product += *(*(network_weights_derivatives_values + weight_index) + output_index) *
						*(*(network_weights_derivatives_values + weight_index_j) + output_index);
				}
				
				*(hessian_matrix + weight_index_base + weight_index_j) = *(hessian_matrix + weight_index_base + weight_index_j) + error_contribution_product;
			}
		}
	}

	current_loss = total_epoch_loss / (double)training_data_size;

	// Save the current computed hessian matrix in case that it is singular:
	memcpy(previous_hessian_matrix, hessian_matrix, weights_count * (weights_count + 1) / 2 * sizeof(double));
	memcpy(previous_jacobian_error_derivative_product, jacobian_error_derivative_product, weights_count * sizeof(double));
	memcpy(previous_weights_values, network_weights_values, weights_count * sizeof(double));
	do
	{
		do
		{
			// Perform the Cholesky's factorization to obtain L * L^T = H + mu * I
			// Solve L * z = J^T*e at the same time:
			for (unsigned int weight_index_i = 0; weight_index_i < weights_count; weight_index_i++)
			{
				double jacobian_factorized_hessian_product = 0.0;
				const unsigned int weight_index_base_i = weight_index_i * (weight_index_i + 1) / 2;
				double row_squared_sum = 0.0;

				for (unsigned int weight_index_j = 0; weight_index_j < weight_index_i; weight_index_j++)
				{
					const unsigned int weight_index_base_j = weight_index_j * (weight_index_j + 1) / 2;
					double row_product_sum = 0.0;
					for (unsigned int weight_index_k = 0; weight_index_k < weight_index_j; weight_index_k++)
					{
						row_product_sum += 
							*(hessian_matrix + weight_index_base_j + weight_index_k) *
							*(hessian_matrix + weight_index_base_i + weight_index_k);
					}

					*(hessian_matrix + weight_index_base_i + weight_index_j) =
						(*(hessian_matrix + weight_index_base_i + weight_index_j) - row_product_sum) /
						*(hessian_matrix + weight_index_j + weight_index_j);

					// Compue the squared sum of the values of the i-th row of the hessian matrix:
					row_squared_sum += *(hessian_matrix + weight_index_base_i + weight_index_j) *
						*(hessian_matrix + weight_index_base_i + weight_index_j);

					/* Perform the product between the factorized hessian and the jacobian error, 
					to compute the i-th entry of vector 'z':
					*/
					jacobian_factorized_hessian_product += *(hessian_matrix + weight_index_base_i + weight_index_j) * *(jacobian_error_derivative_product + weight_index_j);
					
				}
				
				/* Check if the difference is greater than 0.0, 
				otherwise, the hessian matrix is singular:
				*/
				const double difference = *(hessian_matrix + weight_index_base_i + weight_index_i) + mu_value - row_squared_sum;
				if (difference < 1e-12) {
					// If the hessian matrix is not invertable, increase the value of the diagonal:
					mu_value *= mu_increasing_factor;
					// Copy only the values that were modified:
					memcpy(hessian_matrix, previous_hessian_matrix, (weight_index_base_i + weight_index_i)* sizeof(double));
					memcpy(jacobian_error_derivative_product, previous_jacobian_error_derivative_product, weight_index_i * sizeof(double));
					break;
				}

				*(hessian_matrix + weight_index_base_i + weight_index_i) = sqrt(difference);

				// Compute the i-th entry of the temporal 'z' vector
				*(jacobian_error_derivative_product + weight_index_i) = (*(jacobian_error_derivative_product + weight_index_i) - jacobian_factorized_hessian_product) / *(hessian_matrix + weight_index_base_i + weight_index_i);
			}
			break;
		} while (1);

		// Solve L^T * w = z to determine the new weights delta values:
		squared_gradient_norm = 0.0;
		for (int weight_index_i = (weights_count - 1); weight_index_i >= 0; weight_index_i--)
		{
			double jacobian_factorized_hessian_product = 0.0;
			const unsigned int weight_index_base_i = weight_index_i * (weight_index_i + 1) / 2;
			for (int weight_index_j = (weights_count - 1); weight_index_j > weight_index_i; weight_index_j--)
			{
				const unsigned int weight_index_base_j = weight_index_j * (weight_index_j + 1) / 2;
				jacobian_factorized_hessian_product += *(jacobian_error_derivative_product + weight_index_j) * *(hessian_matrix + weight_index_base_j + weight_index_i);
			}

			*(jacobian_error_derivative_product + weight_index_i) = (*(jacobian_error_derivative_product + weight_index_i) - jacobian_factorized_hessian_product) / *(hessian_matrix + weight_index_base_i + weight_index_i);

			// Update the weights:
			*(network_weights_values + weight_index_i) = *(network_weights_values + weight_index_i) - *(jacobian_error_derivative_product + weight_index_i);
			squared_gradient_norm += *(network_weights_values + weight_index_i) * 
				*(network_weights_values + weight_index_i);
		}

		// Evaluate the new weight values:
		training_epoch_loss = 0.0;

		for (unsigned int pattern_index = 0; pattern_index < training_data_size; pattern_index++, network_current_time++)
		{
			// Use the current pattern to feed the network:
			setInputPatternPointer(*(training_data + pattern_index));
			groundtruth_master_pointer = *(groundtruth_data + pattern_index);

			// Perform the Feed forward without the computation of the activation functions derivatives:
			next_loss_function_node = loss_functions_head_node.next_loss_function_node;
			while(next_loss_function_node)
			{
				current_loss_function_node = next_loss_function_node;
				next_loss_function_node = current_loss_function_node->next_loss_function_node;

				training_epoch_loss += current_loss_function_node->loss_function_pointer->computeLoss(network_current_time);
			}
		}

		training_epoch_loss /= (double)training_data_size;
		if (training_epoch_loss < current_loss)
		{
			mu_value *= mu_decreasing_factor;
			memcpy(previous_weights_values, network_weights_pointers_manager, weights_count * sizeof(double));
			break;
		}
		else
		{
			mu_value *= mu_increasing_factor;
			worsening_count++;
			if (worsening_count > 5)
			{
				/*  If the error has not decreased after 5 cycles,
				the current weights are passed to the following epoch to recompute the hessian matrix
				*/
				break;
			}
			else
			{
				/* If the error has not decreased,
				the diagonal of the hessian matrix is modified in order to change the descent direction
				*/
				memcpy(network_weights_pointers_manager, previous_weights_values, weights_count * sizeof(double));

				memcpy(hessian_matrix, previous_hessian_matrix, weights_count * (weights_count  + 1) / 2 * sizeof(double));

				memcpy(jacobian_error_derivative_product, previous_jacobian_error_derivative_product,
					weights_count * sizeof(double));
			}
		}
	} while (1);
	
	printf("Epoch computed successfully");

	if ((mu_value > max_mu_value) || (current_loss < target_loss) || (squared_gradient_norm < target_loss*target_loss))
	{
		return false;
	}

	return true;
}

#else //_OPENMP


bool backpropagationBasedANN::computeEpoch_levenberg_marquardt()
{
	double squared_gradient_norm;
	unsigned int worsening_count = 0;
	double training_epoch_loss;
	double total_epoch_loss = 0.0;

	double total_epoch_loss_parallel_temp = 0.0;

	const unsigned int batch_size_per_thread_parallel_temp = batch_size_per_thread;
	const unsigned int training_data_size_parallel_temp = training_data_size;
	const unsigned int neurons_count_parallel_temp = neurons_count;
	const unsigned int outputs_count_parallel_temp = outputs_count;
	const unsigned int weights_count_parallel_temp = weights_count;

	double *** network_weights_derivatives_values_threads_parallel_temp = 
		network_weights_derivatives_values_threads;

	double ** jacobian_error_derivative_product_threads_parallel_temp = 
		jacobian_error_derivative_product_threads;
	double ** hessian_matrix_threads_parallel_temp = hessian_matrix_threads;

	// Network architecture:
	Neuron *** network_neurons_threads_parallel_temp = network_neurons_threads;
	Neuron *** network_output_nodes_threads_parallel_temp = network_output_nodes_threads;

	// Trained network data:
	double *** training_data_threads_parallel_temp = training_data_threads;
	double ** input_pattern_master_pointer_threads_parallel_temp = 
		input_pattern_master_pointer_threads;

	int ** groundtruth_master_pointer_threads_parallel_temp = groundtruth_master_pointer_threads;
	int *** groundtruth_data_threads_parallel_temp = groundtruth_data_threads;

	LOSS_FUNCTION_LIST_NODE * loss_functions_head_node_threads_parallel_temp = 
		loss_functions_head_node_threads;

	unsigned int current_thread_network_time = network_current_time;
	// Perform the feed-forward process for each pattern in the training set (per thread):
#pragma omp parallel default(none) \
firstprivate(\
batch_size_per_thread_parallel_temp,\
training_data_size_parallel_temp,\
current_thread_network_time,\
neurons_count_parallel_temp,\
outputs_count_parallel_temp,\
weights_count_parallel_temp,\
network_weights_derivatives_values_threads_parallel_temp,\
jacobian_error_derivative_product_threads_parallel_temp,\
hessian_matrix_threads_parallel_temp,\
network_neurons_threads_parallel_temp,\
network_output_nodes_threads_parallel_temp,\
training_data_threads_parallel_temp,\
input_pattern_master_pointer_threads_parallel_temp,\
groundtruth_master_pointer_threads_parallel_temp,\
groundtruth_data_threads_parallel_temp,\
loss_functions_head_node_threads_parallel_temp\
)\
shared(\
total_epoch_loss_parallel_temp\
)
	{
		const int current_thread_id = omp_get_thread_num();

		double ** current_thread_network_weights_derivatives_values =
			*(network_weights_derivatives_values_threads_parallel_temp + current_thread_id);

		double * current_thread_jacobian_error_derivative_product =
			*(jacobian_error_derivative_product_threads_parallel_temp + current_thread_id);

		double * current_thread_hessian_matrix = 
			*(hessian_matrix_threads_parallel_temp + current_thread_id);

		Neuron ** current_thread_network_neurons =
			*(network_neurons_threads_parallel_temp + current_thread_id);

		Neuron ** current_thread_network_output_nodes =
			*(network_output_nodes_threads_parallel_temp + current_thread_id);

		double ** current_thread_training_data =
			*(training_data_threads_parallel_temp + current_thread_id);

		double ** current_thread_input_pattern_master_pointer =
			input_pattern_master_pointer_threads_parallel_temp + current_thread_id;

		int ** current_thread_groundtruth_master_pointer = 
			groundtruth_master_pointer_threads_parallel_temp + current_thread_id;

		int **	current_thread_groundtruth_data =
			*(groundtruth_data_threads_parallel_temp + current_thread_id);

		LOSS_FUNCTION_LIST_NODE * current_thread_loss_functions_head_node =
			loss_functions_head_node_threads_parallel_temp + current_thread_id;

		const unsigned int current_thread_batch_size = ((current_thread_id + 1) * batch_size_per_thread_parallel_temp) > training_data_size_parallel_temp ? (training_data_size_parallel_temp - current_thread_id*batch_size_per_thread_parallel_temp) : batch_size_per_thread_parallel_temp;

		// Set the hessian and jacobian arrays to 0, to perform a cummulated sum of their values:
		memset(current_thread_hessian_matrix, 0,
			weights_count_parallel_temp * (weights_count_parallel_temp + 1) / 2 * sizeof(double));

		memset(current_thread_jacobian_error_derivative_product, 0,
			weights_count_parallel_temp * sizeof(double));

		LOSS_FUNCTION_LIST_NODE * current_loss_function_node;
		LOSS_FUNCTION_LIST_NODE * next_loss_function_node;

		double current_thread_total_epoch_loss = 0.0;
		unsigned int output_index;
		
		for (unsigned int pattern_index = 0; pattern_index < current_thread_batch_size; pattern_index++, current_thread_network_time++)
		{
			// Use the current pattern to feed the network:
			*current_thread_input_pattern_master_pointer = 
				*(current_thread_training_data + pattern_index);

			*current_thread_groundtruth_master_pointer = 
				*(current_thread_groundtruth_data + pattern_index);

			// Perform the Feed forward:
			next_loss_function_node = current_thread_loss_functions_head_node->next_loss_function_node;
			output_index = 0;
			while (next_loss_function_node)
			{
				current_loss_function_node = next_loss_function_node;
				next_loss_function_node = current_loss_function_node->next_loss_function_node;

				current_thread_total_epoch_loss += current_loss_function_node->loss_function_pointer->computeLossWithDerivatives(current_thread_network_time);

				/* This is necessary to back propagate only the error contribution
				to each isolated loss function.
				*/
				(*(current_thread_network_output_nodes + output_index))->addNodeErrorContribution(1.0, output_index);

				output_index++;
			}

			// Backpropagate the neuron's error contribution:
			for (int neuron_index = (neurons_count_parallel_temp - 1); neuron_index >= 0; neuron_index--)
			{
				// Backpropagate the error contribution:
				(*(current_thread_network_neurons + neuron_index))->backpropagateNodeError();
			}

			for (unsigned int weight_index = 0; weight_index < weights_count_parallel_temp; weight_index++)
			{
				// Compute the product between the Jacobian matrix and the vector of errors:
				output_index = 0;
				next_loss_function_node = current_thread_loss_functions_head_node->next_loss_function_node;

				double error_contribution_product = 0.0;
				while (next_loss_function_node)
				{
					current_loss_function_node = next_loss_function_node;
					next_loss_function_node = current_loss_function_node->next_loss_function_node;

					*(current_thread_jacobian_error_derivative_product + weight_index) =
						*(current_thread_jacobian_error_derivative_product + weight_index) +
						*(*(current_thread_network_weights_derivatives_values + weight_index) + output_index) *
						current_loss_function_node->loss_function_pointer->getErrorDerivative();

					/* Compute the hessian matrix entry that corresponds to
					the product of the current weight and the first weight of the network:
					*/
					error_contribution_product += 
						*(*(current_thread_network_weights_derivatives_values + weight_index) + output_index) *
						*(*(current_thread_network_weights_derivatives_values + weight_index) + output_index);

					output_index++;
				}
				const unsigned int weight_index_base = weight_index * (weight_index + 1) / 2;
				*(current_thread_hessian_matrix + weight_index_base) = 
					*(current_thread_hessian_matrix + weight_index_base) + error_contribution_product;

				// Compute the jacobian matrix product with its self transpose:
				for (unsigned int weight_index_j = 0; weight_index_j <= weight_index; weight_index_j++)
				{
					double error_contribution_product = 0.0;
					for (unsigned int output_index = 0; output_index < outputs_count_parallel_temp; output_index++)
					{
						error_contribution_product += 
							*(*(current_thread_network_weights_derivatives_values + weight_index) + output_index) *
							*(*(current_thread_network_weights_derivatives_values + weight_index_j) + output_index);
					}

					*(current_thread_hessian_matrix + weight_index_base + weight_index_j) =
						*(current_thread_hessian_matrix + weight_index_base + weight_index_j) +
						error_contribution_product;
				}
			}
		}

#pragma omp critical
		{
			total_epoch_loss_parallel_temp += current_thread_total_epoch_loss;
		}
	} // End of the parallel section
	current_loss = total_epoch_loss_parallel_temp / (double)training_data_size;

	// Join the hessian matrices of all the threads:
	memset(hessian_matrix, 0, weights_count * (weights_count + 1) / 2 * sizeof(double));
	memset(jacobian_error_derivative_product, 0, weights_count * sizeof(double));

	for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
	{
		for (unsigned int weight_index = 0; weight_index < weights_count; weight_index++)
		{
			*(hessian_matrix + weight_index) = *(hessian_matrix + weight_index) +
				*(*(hessian_matrix_threads + thread_id) + weight_index);

			*(jacobian_error_derivative_product + weight_index) = *(jacobian_error_derivative_product + weight_index) +
				*(*(jacobian_error_derivative_product_threads + thread_id) + weight_index);
		}

		for (unsigned int weight_index = weights_count; weight_index < (weights_count * (weights_count + 1) / 2); weight_index++)
		{
			*(hessian_matrix + weight_index) = *(hessian_matrix + weight_index) +
				*(*(hessian_matrix_threads + thread_id) + weight_index);
		}
	}
	
	LOSS_FUNCTION_LIST_NODE * current_loss_function_node;
	LOSS_FUNCTION_LIST_NODE * next_loss_function_node;

	// Save the current computed hessian matrix in case that it is singular:
	memcpy(previous_hessian_matrix, hessian_matrix, weights_count * (weights_count + 1) / 2 * sizeof(double));
	memcpy(previous_jacobian_error_derivative_product, jacobian_error_derivative_product, weights_count * sizeof(double));
	memcpy(previous_weights_values, network_weights_values, weights_count * sizeof(double));
	do
	{
		do
		{
			// Perform the Cholesky's factorization to obtain L * L^T = H + mu * I
			// Solve L * z = J^T*e at the same time:
			for (unsigned int weight_index_i = 0; weight_index_i < weights_count; weight_index_i++)
			{
				double jacobian_factorized_hessian_product = 0.0;
				const unsigned int weight_index_base_i = weight_index_i * (weight_index_i + 1) / 2;
				double row_squared_sum = 0.0;

				for (unsigned int weight_index_j = 0; weight_index_j < weight_index_i; weight_index_j++)
				{
					const unsigned int weight_index_base_j = weight_index_j * (weight_index_j + 1) / 2;
					double row_product_sum = 0.0;
					for (unsigned int weight_index_k = 0; weight_index_k < weight_index_j; weight_index_k++)
					{
						row_product_sum +=
							*(hessian_matrix + weight_index_base_j + weight_index_k) *
							*(hessian_matrix + weight_index_base_i + weight_index_k);
					}

					*(hessian_matrix + weight_index_base_i + weight_index_j) =
						(*(hessian_matrix + weight_index_base_i + weight_index_j) - row_product_sum) /
						*(hessian_matrix + weight_index_j + weight_index_j);

					// Compue the squared sum of the values of the i-th row of the hessian matrix:
					row_squared_sum += *(hessian_matrix + weight_index_base_i + weight_index_j) *
						*(hessian_matrix + weight_index_base_i + weight_index_j);

					/* Perform the product between the factorized hessian and the jacobian error,
					to compute the i-th entry of vector 'z':
					*/
					jacobian_factorized_hessian_product += *(hessian_matrix + weight_index_base_i + weight_index_j) * *(jacobian_error_derivative_product + weight_index_j);

				}

				/* Check if the difference is greater than 0.0,
				otherwise, the hessian matrix is singular:
				*/
				const double difference = *(hessian_matrix + weight_index_base_i + weight_index_i) + mu_value - row_squared_sum;
				if (difference < 1e-12) {
					// If the hessian matrix is not invertable, increase the value of the diagonal:
					mu_value *= mu_increasing_factor;
					// Copy only the values that were modified:
					memcpy(hessian_matrix, previous_hessian_matrix, (weight_index_base_i + weight_index_i) * sizeof(double));
					memcpy(jacobian_error_derivative_product, previous_jacobian_error_derivative_product, weight_index_i * sizeof(double));
					break;
				}

				*(hessian_matrix + weight_index_base_i + weight_index_i) = sqrt(difference);

				// Compute the i-th entry of the temporal 'z' vector
				*(jacobian_error_derivative_product + weight_index_i) = (*(jacobian_error_derivative_product + weight_index_i) - jacobian_factorized_hessian_product) / *(hessian_matrix + weight_index_base_i + weight_index_i);
			}
			break;
		} while (1);

		// Solve L^T * w = z to determine the new weights delta values:
		squared_gradient_norm = 0.0;
		for (int weight_index_i = (weights_count - 1); weight_index_i >= 0; weight_index_i--)
		{
			double jacobian_factorized_hessian_product = 0.0;
			const unsigned int weight_index_base_i = weight_index_i * (weight_index_i + 1) / 2;
			for (int weight_index_j = (weights_count - 1); weight_index_j > weight_index_i; weight_index_j--)
			{
				const unsigned int weight_index_base_j = weight_index_j * (weight_index_j + 1) / 2;
				jacobian_factorized_hessian_product += *(jacobian_error_derivative_product + weight_index_j) * *(hessian_matrix + weight_index_base_j + weight_index_i);
			}

			*(jacobian_error_derivative_product + weight_index_i) = (*(jacobian_error_derivative_product + weight_index_i) - jacobian_factorized_hessian_product) / *(hessian_matrix + weight_index_base_i + weight_index_i);

			// Update the weights:
			*(network_weights_values + weight_index_i) = *(network_weights_values + weight_index_i) - *(jacobian_error_derivative_product + weight_index_i);
			squared_gradient_norm += *(network_weights_values + weight_index_i) *
				*(network_weights_values + weight_index_i);
		}

		// Evaluate the new weight values:
		training_epoch_loss = 0.0;

		for (unsigned int pattern_index = 0; pattern_index < training_data_size; pattern_index++, network_current_time++)
		{
			// Use the current pattern to feed the network:
			setInputPatternPointer(*(training_data + pattern_index));
			groundtruth_master_pointer = *(groundtruth_data + pattern_index);

			// Perform the Feed forward without the computation of the activation functions derivatives:
			next_loss_function_node = loss_functions_head_node.next_loss_function_node;
			while (next_loss_function_node)
			{
				current_loss_function_node = next_loss_function_node;
				next_loss_function_node = current_loss_function_node->next_loss_function_node;

				training_epoch_loss += current_loss_function_node->loss_function_pointer->computeLoss(network_current_time);
			}
		}

		training_epoch_loss /= (double)training_data_size;
		if (training_epoch_loss < current_loss)
		{
			mu_value *= mu_decreasing_factor;
			memcpy(previous_weights_values, network_weights_pointers_manager, weights_count * sizeof(double));
			break;
		}
		else
		{
			mu_value *= mu_increasing_factor;
			worsening_count++;
			if (worsening_count > 5)
			{
				/*  If the error has not decreased after 5 cycles,
				the current weights are passed to the following epoch to recompute the hessian matrix
				*/
				break;
			}
			else
			{
				/* If the error has not decreased,
				the diagonal of the hessian matrix is modified in order to change the descent direction
				*/
				memcpy(network_weights_pointers_manager, previous_weights_values, weights_count * sizeof(double));

				memcpy(hessian_matrix, previous_hessian_matrix, weights_count * (weights_count + 1) / 2 * sizeof(double));

				memcpy(jacobian_error_derivative_product, previous_jacobian_error_derivative_product,
					weights_count * sizeof(double));
			}
		}
	} while (1);


	for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
	{
		memcpy(*(network_weights_values_threads + thread_id), network_weights_values, weights_count * sizeof(double));
	}

	printf("Epoch computed successfully");

	if ((mu_value > max_mu_value) || (current_loss < target_loss) || (squared_gradient_norm < target_loss*target_loss))
	{
		return false;
	}

	return true;
}
#endif //_OPENMP



double backpropagationBasedANN::trainNetwork(const int save_each_n_epochs, const int restart_time_each_n_epochs)
{
	allocateMethodMemory();
	TIMERS;
	double elapsed_time = 0.0;
	bool continue_training = true;
	while ((++current_epoch <= n_epochs) && continue_training)
	{
		if ((restart_time_each_n_epochs > 0) && !(current_epoch % restart_time_each_n_epochs))
		{
			LOSS_FUNCTION_LIST_NODE * current_loss_function_node;
			LOSS_FUNCTION_LIST_NODE * next_loss_function_node = loss_functions_head_node.next_loss_function_node;
			while (next_loss_function_node)
			{
				current_loss_function_node = next_loss_function_node;
				next_loss_function_node = current_loss_function_node->next_loss_function_node;

				current_loss_function_node->loss_function_pointer->resetErrorCurrentTime();
			}

#ifdef _OPENMP
			for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
			{
				next_loss_function_node = (loss_functions_head_node_threads + thread_id)->next_loss_function_node;
				while (next_loss_function_node)
				{
					current_loss_function_node = next_loss_function_node;
					next_loss_function_node = current_loss_function_node->next_loss_function_node;

					current_loss_function_node->loss_function_pointer->resetErrorCurrentTime();
				}

				for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
				{
					(*(*(network_neurons_threads + thread_id) + neuron_index))->resetNodeCurrentTime();
				}
			}
#endif //_OPENMP
		}
		GETTIME_INI;
		continue_training = (this->*computeEpoch)();
		GETTIME_FIN;
		if ((save_each_n_epochs > 0) && !(current_epoch % save_each_n_epochs))
		{
			saveNetworkState();
			saveState();
		}
		elapsed_time += DIFTIME;
		printf("Epochs: %i, average error = %.18f, epoch computation time = %f s, elapsed time = %f\n", current_epoch, current_loss, DIFTIME, elapsed_time);
	}


	return current_loss;
}



void backpropagationBasedANN::saveState()
{

}



void backpropagationBasedANN::setBackpropagationMethod(BACKPROPAGATION_METHOD src_weights_update_method)
{
	switch (src_weights_update_method)
	{
		case BPM_GRADIENT_DESCENT:
			computeEpoch = &backpropagationBasedANN::computeEpoch_gradient_descent;
			break;

		case BPM_LEVENBERG_MARQUARDT:
			computeEpoch = &backpropagationBasedANN::computeEpoch_levenberg_marquardt;
			hessian_matrix_was_required = true;
			break;
	}
}
