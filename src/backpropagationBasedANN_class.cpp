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
		}
		else
		{
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
