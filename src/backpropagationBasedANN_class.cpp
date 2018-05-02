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

	network_weights_values_pointer = &network_weights_values;
	network_weights_derivatives_values_pointer = &network_weights_derivatives_values;

	memory_already_allocated = false;
	hessian_matrix_was_required = false;

	previous_weights_values = NULL;
	jacobian_error_derivative_product = NULL;
	hessian_matrix = NULL;
	previous_jacobian_error_derivative_product = NULL;
	previous_hessian_matrix = NULL;

#ifdef _OPENMP
	groundtruth_data_parallel = NULL;
	training_data_parallel = NULL;

	network_input_nodes_parallel = NULL;
	network_neurons_parallel = NULL;
	network_outputs_parallel = NULL;
	loss_functions_parallel = NULL;

	hessian_matrix_parallel = NULL;
	jacobian_error_derivative_product_parallel = NULL;
	network_weights_derivative_values_parallel = NULL;
	network_weights_values_parallel = NULL;

	training_data_size_per_thread;
#endif // _OPENMP

	mu_value = 0.001;
	max_mu_value = 1e10;
	mu_increasing_factor = 10.0;
	mu_decreasing_factor = 0.1;

	momentums = 0.9;
	learning_rates = 0.05;

	random_number_generator_seed = initSeed(777);
}



backpropagationBasedANN::~backpropagationBasedANN()
{
	if (memory_already_allocated)
	{
		for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
		{
			free(*(network_weights_values + neuron_index));
			free(*(network_weights_derivatives_values + neuron_index));
		}
		free(network_weights_values);
		free(network_weights_derivatives_values);
		
		if (hessian_matrix_was_required)
		{
			for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
			{
				free(*(previous_weights_values + neuron_index));
				free(*(jacobian_error_derivative_product + neuron_index));
				free(*(hessian_matrix + neuron_index));
				free(*(previous_jacobian_error_derivative_product + neuron_index));
				free(*(previous_hessian_matrix + neuron_index));
			}
			free(previous_weights_values);
			free(jacobian_error_derivative_product);
			free(hessian_matrix);
			free(previous_jacobian_error_derivative_product);
			free(previous_hessian_matrix);
		}
		else
		{
			for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
			{
				free(*(weights_deltas + neuron_index));
			}
			free(weights_deltas);
		}
	}
	
	free(random_number_generator_seed);
}



/* This function will erase all the stored data to allocate the memory space */
int backpropagationBasedANN::allocateMethodMemory()
{
	/* Successfull allocation of memory returns 1, otherwise, returns 0 */
	if (memory_already_allocated)
	{
		return 1;
	}

	if (neurons_count == 0)
	{
		return 0;
	}

	// Allocate memory for the weights of the network
	network_weights_values = (double**)malloc(neurons_count * sizeof(double*));
	network_weights_derivatives_values = (double**)malloc(neurons_count * sizeof(double*));
	for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
	{
		const unsigned int current_neuron_inputs_count = (*(network_neurons + neuron_index))->getInputsCount();

		*(network_weights_values + neuron_index) = (double*)malloc(current_neuron_inputs_count * sizeof(double));
		*(network_weights_derivatives_values + neuron_index) = (double*)malloc(outputs_count * current_neuron_inputs_count * sizeof(double));

		(*(network_neurons + neuron_index))->makeExternalWeightValues(&network_weights_values_pointer, &network_weights_derivatives_values_pointer);

		// Initialize the Bias in 0.0:
		*(*(network_weights_values + neuron_index)) = 0.0;
		for (unsigned int input_index = 1; input_index < current_neuron_inputs_count; input_index++)
		{
			// Initialize the Weights with a random value normally distributed:
			*(*(network_weights_values + neuron_index) + input_index) = anorm_est(random_number_generator_seed);
		}
	}


	if (hessian_matrix_was_required)
	{
		// Allocate memory for the weights update directions:
		unsigned int accumulated_weight_index = 0;
		hessian_matrix = (double**)malloc(neurons_count * sizeof(double*));
		previous_hessian_matrix = (double**)malloc(neurons_count * sizeof(double*));

		jacobian_error_derivative_product = (double**)malloc(neurons_count * sizeof(double*));
		previous_jacobian_error_derivative_product = (double**)malloc(neurons_count * sizeof(double*));

		previous_weights_values = (double**)malloc(neurons_count * sizeof(double*));
		
		for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
		{
			const unsigned int current_neuron_inputs_count = (*(network_neurons + neuron_index))->getInputsCount();
			const unsigned int current_neuron_weights_block_size = accumulated_weight_index * current_neuron_inputs_count + current_neuron_inputs_count * (current_neuron_inputs_count + 1) / 2;

			*(previous_weights_values + neuron_index) = (double*)malloc(current_neuron_inputs_count * sizeof(double));

			*(jacobian_error_derivative_product + neuron_index) = (double*)malloc(current_neuron_inputs_count * sizeof(double));
			*(previous_jacobian_error_derivative_product + neuron_index) = (double*)malloc(current_neuron_inputs_count * sizeof(double));


			*(hessian_matrix + neuron_index) = (double*)malloc(current_neuron_weights_block_size * sizeof(double));
			*(previous_hessian_matrix + neuron_index) = (double*)malloc(current_neuron_weights_block_size * sizeof(double));

			accumulated_weight_index += current_neuron_inputs_count;
		}
	}
	else
	{
		// Allocate memory for the weights update directions:
		weights_deltas = (double**)malloc(neurons_count * sizeof(double*));

		for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
		{
			const unsigned int current_neuron_inputs_count = (*(network_neurons + neuron_index))->getInputsCount();

			*(weights_deltas + neuron_index) = (double*)malloc(current_neuron_inputs_count * sizeof(double));
		}
	}

	memory_already_allocated = true;
	return 1;
}



bool backpropagationBasedANN::computeEpoch_gradient_descent()
{
	// Reset the time to prevent overflow
	LOSS_FUNCTION_LIST_NODE * current_loss_function_node;
	LOSS_FUNCTION_LIST_NODE * next_loss_function_node = loss_functions_head_node.next_loss_function_node;
	while (next_loss_function_node)
	{
		current_loss_function_node = next_loss_function_node;
		next_loss_function_node = current_loss_function_node->next_loss_function_node;

		current_loss_function_node->loss_function_pointer->resetErrorCurrentTime();
	}

	double squared_gradient_norm;

	// Reset the weights and bias delta values to 0:
	for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
	{
		const unsigned int current_neuron_inputs_count = (*(network_neurons + neuron_index))->getInputsCount();
		memset(*(weights_deltas + neuron_index), 0, current_neuron_inputs_count * sizeof(double));
	}

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

			total_epoch_loss += current_loss_function_node->loss_function_pointer->computeLoss(network_current_time);

			// Backpropagate the error contribution:
			current_loss_function_node->loss_function_pointer->backpropagateErrorDerivative();
		}

		// Backpropagate the neuron's error contribution:
		for (int neuron_index = (neurons_count - 1); neuron_index >= 0; neuron_index--)
		{
			(*(network_neurons + neuron_index))->backpropagateNodeError();
		}
		

		squared_gradient_norm = 0.0;
		for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
		{
			const unsigned int current_neuron_inputs_count = (*(network_neurons + neuron_index))->getInputsCount();

			for (unsigned int weighted_input_index = 0; weighted_input_index < current_neuron_inputs_count; weighted_input_index++)
			{
				double weight_error_contribution = 0.0;
				for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
				{
					weight_error_contribution += *(*(network_weights_derivatives_values + neuron_index) + weighted_input_index * outputs_count + output_index);
				}
				*(*(weights_deltas + neuron_index) + weighted_input_index) = *(*(weights_deltas + neuron_index) + weighted_input_index) * momentums + learning_rates * weight_error_contribution;

				// update the weights and bias values:
				*(*(network_weights_values + neuron_index) + weighted_input_index) = *(*(network_weights_values + neuron_index) + weighted_input_index) - *(*(weights_deltas + neuron_index) + weighted_input_index);
				squared_gradient_norm += *(*(weights_deltas + neuron_index) + weighted_input_index) * *(*(weights_deltas + neuron_index) + weighted_input_index);
			}
		}
	}

	current_loss = total_epoch_loss / (double)training_data_size;

	if ((current_loss < target_loss) || (squared_gradient_norm < 1e-16))
	{
		return false;
	}

	return true;
}



double backpropagationBasedANN::productOutputErrorContribution(const unsigned int src_neuron_index_a, const unsigned int src_weighted_input_index_a, const unsigned int src_neuron_index_b, const unsigned int src_weighted_input_index_b, double *** src_network_weights_derivatives_values_master_pointer)
{
	double error_contribution_product = 0.0;
	for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
	{
		error_contribution_product +=
		*(*(*src_network_weights_derivatives_values_master_pointer + src_neuron_index_a) + outputs_count * src_weighted_input_index_a) * *(*(*src_network_weights_derivatives_values_master_pointer + src_neuron_index_b) + outputs_count * src_weighted_input_index_b);
	}

	return error_contribution_product;
}



double backpropagationBasedANN::computeCholeskyRowValue(const unsigned int src_neuron_index, const unsigned int src_weighted_input_index, double ** src_hessian_matrix)
{

}



bool backpropagationBasedANN::computeEpoch_levenberg_marquardt()
{
	// Reset the time to prevent overflow
	LOSS_FUNCTION_LIST_NODE * current_loss_function_node;
	LOSS_FUNCTION_LIST_NODE * next_loss_function_node = loss_functions_head_node.next_loss_function_node;
	while (next_loss_function_node)
	{
		current_loss_function_node = next_loss_function_node;
		next_loss_function_node = current_loss_function_node->next_loss_function_node;

		current_loss_function_node->loss_function_pointer->resetErrorCurrentTime();
	}
	
	double squared_gradient_norm;
	unsigned int worsening_count = 0;
	double training_epoch_loss;
	double total_epoch_loss = 0.0;
	unsigned int output_index;

	// Set the hessian and jacobian arrays to 0, to perform a cummulated sum of their values:
	unsigned int accumulated_weight_index = 0;
	for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
	{
		const unsigned int current_neuron_inputs_count = (*(network_neurons + neuron_index))->getInputsCount();
		const unsigned int current_neuron_weights_block_size = accumulated_weight_index * current_neuron_inputs_count + current_neuron_inputs_count * (current_neuron_inputs_count + 1) / 2;

		memset(*(hessian_matrix + neuron_index), 0, current_neuron_weights_block_size * sizeof(double));
		memset(*(jacobian_error_derivative_product + neuron_index), 0, current_neuron_inputs_count * sizeof(double));
	}

	// Perform the feed-forward process for each pattern in the training set:
	for (unsigned int pattern_index = 0; pattern_index < training_data_size; pattern_index++, network_current_time++)
	{
		// Use the current pattern to feed the network:
		setInputPatternPointer(*(training_data + pattern_index));

		// Perform the Feed forward:
		next_loss_function_node = loss_functions_head_node.next_loss_function_node;
		output_index = 0;
		while(next_loss_function_node)
		{
			current_loss_function_node = next_loss_function_node;
			next_loss_function_node = current_loss_function_node->next_loss_function_node;

			total_epoch_loss += current_loss_function_node->loss_function_pointer->computeLoss(network_current_time);

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

		for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
		{
			const unsigned int current_neuron_inputs_count = (*(network_neurons + neuron_index))->getInputsCount();

			// Compute the product between the Jacobian matrix and the vector of errors:
			for (unsigned int weighted_input_index = 0; weighted_input_index < current_neuron_inputs_count; weighted_input_index++)
			{
				output_index = 0;
				next_loss_function_node = loss_functions_head_node.next_loss_function_node;
				while(next_loss_function_node)
				{
					current_loss_function_node = next_loss_function_node;
					next_loss_function_node = current_loss_function_node->next_loss_function_node;

					*(*(jacobian_error_derivative_product + neuron_index) + weighted_input_index) =
						*(*(jacobian_error_derivative_product + neuron_index) + weighted_input_index) +
						*(*(network_weights_derivatives_values + neuron_index) + outputs_count * weighted_input_index + output_index) *
						current_loss_function_node->loss_function_pointer->getErrorDerivative();
				}

				// Compute the jacobian matrix product with its self transpose:
				accumulated_weight_index = 0;
				for (unsigned int neuron_index_j = 0; neuron_index_j < neuron_index; neuron_index_j++)
				{
					const unsigned int current_neuron_j_inputs_count = (*(network_neurons + neuron_index_j))->getInputsCount();

					for (unsigned int weighted_input_index_j = 0; weighted_input_index_j < current_neuron_j_inputs_count; weighted_input_index_j++)
					{
						*(*(hessian_matrix + neuron_index) + accumulated_weight_index + weighted_input_index_j) = productOutputErrorContribution(neuron_index, weighted_input_index, neuron_index_j, weighted_input_index_j, network_weights_derivatives_values_pointer);
					}
					accumulated_weight_index += current_neuron_j_inputs_count;
				}
				
				for (unsigned int weighted_input_index_j = 0; weighted_input_index_j <= weighted_input_index; weighted_input_index_j++)
				{
					*(*(hessian_matrix + neuron_index) + accumulated_weight_index + weighted_input_index_j) = productOutputErrorContribution(neuron_index, weighted_input_index, neuron_index, weighted_input_index_j, network_weights_derivatives_values_pointer);
				}
			}
		}
	}

	current_loss = total_epoch_loss / (double)training_data_size;

	// Save the current computed hessian matrix in case that it is singular:
	accumulated_weight_index = 0;
	for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
	{
		const unsigned int current_neuron_inputs_count = (*(network_neurons + neuron_index))->getInputsCount();
		const unsigned int current_neuron_weights_block_size = accumulated_weight_index * current_neuron_inputs_count + current_neuron_inputs_count * (current_neuron_inputs_count + 1) / 2;

		memcpy(*(previous_hessian_matrix + neuron_index),
			*(hessian_matrix + neuron_index),
			current_neuron_weights_block_size * sizeof(double));

		memcpy(*(previous_jacobian_error_derivative_product + neuron_index), 
			*(jacobian_error_derivative_product + neuron_index),
			current_neuron_inputs_count * sizeof(double));
	}

	do
	{
		do
		{
			// Perform the Cholesky's factorization to obtain L * L^T = H + mu * I
			// Solve L * z = J^T*e at the same time:

			// Solve L^T * w = z to determine the new weights delta values:
			break;
		} while (1);

		// Evaluate the new weight values:
		training_epoch_loss = 0.0;

		for (unsigned int pattern_index = 0; pattern_index < training_data_size; pattern_index++, network_current_time++)
		{
			// Use the current pattern to feed the network:
			setInputPatternPointer(*(training_data + pattern_index));
			groundtruth_master_pointer = *(groundtruth_data + pattern_index);

			// Perform the Feed forward:
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
			memcpy(previous_weights_values, network_weights_values, (weights_count + neurons_count) * sizeof(double));
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
				accumulated_weight_index = 0;
				for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
				{
					const unsigned int current_neuron_inputs_count = (*(network_neurons + neuron_index))->getInputsCount();
					const unsigned int current_neuron_weights_block_size = accumulated_weight_index * current_neuron_inputs_count + current_neuron_inputs_count * (current_neuron_inputs_count + 1) / 2;

					memcpy(*(network_weights_values + neuron_index),
						previous_weights_values,
						current_neuron_inputs_count * sizeof(double));

					memcpy(*(hessian_matrix + neuron_index),
						previous_hessian_matrix,
						accumulated_weight_index * sizeof(double));

					memcpy(*(jacobian_error_derivative_product + neuron_index),
						previous_jacobian_error_derivative_product,
						current_neuron_inputs_count * sizeof(double));
				}
			}
		}
	} while (1);
	
	if ((mu_value > max_mu_value) || (current_loss < target_loss) || (squared_gradient_norm < target_loss*target_loss))
	{
		return false;
	}

	return true;
}



double backpropagationBasedANN::trainNetwork(int save_each_n_epochs)
{
	allocateMethodMemory();

	bool continue_training = true;
	while (++current_epoch <= n_epochs && continue_training)
	{
		continue_training = (this->*computeEpoch)();
		if ((save_each_n_epochs > 0) && (current_epoch % save_each_n_epochs))
		{
			saveNetworkState();
			saveState();
		}
		printf("Epochs: %i, average error = %.18f\n", current_epoch, current_loss);
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
