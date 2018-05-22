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

	random_number_generator_seed = initSeed(778);

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
		}
		else
		{
			free(weights_deltas);
		}
	}
	
	free(random_number_generator_seed);
}


#ifdef _OPENMP
int backpropagationBasedANN::allocateNetworkArchitectureParallel()
{
	ann_threads = (ArtificialNeuralNetwork**) malloc(available_threads * sizeof(ArtificialNeuralNetwork*));

	network_weights_derivatives_pointers_manager_threads = (double****)malloc(available_threads * sizeof(double***));
	network_weights_pointers_manager_threads = (double***)malloc(available_threads * sizeof(double**));

	network_weights_derivatives_values_threads = (double***)malloc(available_threads * sizeof(double**));
	network_weights_values_threads = (double**)malloc(available_threads * sizeof(double*));

	for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
	{
		*(ann_threads + thread_id) = new ArtificialNeuralNetwork(*my_ann);

		*(network_weights_pointers_manager_threads + thread_id) = (double**)malloc(neurons_count * sizeof(double*));
		*(network_weights_derivatives_pointers_manager_threads + thread_id) = (double***)malloc(neurons_count * sizeof(double**));

		*(network_weights_derivatives_values_threads + thread_id) = (double**)malloc(weights_count * sizeof(double*));
		*(network_weights_values_threads + thread_id) = (double*)malloc(weights_count * sizeof(double));
		
		memcpy(*(network_weights_values_threads + thread_id),
			network_weights_values, weights_count * sizeof(double));

		unsigned int weight_index_base = 0;
		for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
		{
			*(*(network_weights_pointers_manager_threads + thread_id) + neuron_index) =
			*(network_weights_values_threads + thread_id) + weight_index_base;

			*(*(network_weights_derivatives_pointers_manager_threads + thread_id) + neuron_index) =
				*(network_weights_derivatives_values_threads + thread_id) + weight_index_base;

			const unsigned int current_neuron_inputs_count = my_ann->getWeightedInputsInNeuron(neuron_index);
			for (unsigned int weight_index = 0; weight_index < current_neuron_inputs_count; weight_index++, weight_index_base++)
			{
				*(*(network_weights_derivatives_values_threads + thread_id) + weight_index_base) =
					(double*)malloc(outputs_count * sizeof(double));
			}
		}

		(*(ann_threads + thread_id))->setNetworkWeightsAndDerivatives(network_weights_pointers_manager_threads + thread_id, network_weights_derivatives_pointers_manager_threads + thread_id, false);
	}

	return 1;
}



int backpropagationBasedANN::allocateTrainingDataParallel()
{
	// Allocate memory for the training data batches:
	groundtruth_data_threads = (int***)malloc(available_threads * sizeof(int**));

	training_data_threads = (double***)malloc(available_threads * sizeof(double**));

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


void backpropagationBasedANN::deallocateNetworkArchitectureParallel()
{
	for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
	{
		unsigned int weight_index_base = 0;
		for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
		{
			const unsigned int current_neuron_inputs_count = (*(ann_threads + thread_id))->getWeightedInputsInNeuron(neuron_index);
			for (unsigned int weight_index = 0; weight_index < current_neuron_inputs_count; weight_index++, weight_index_base++)
			{
				free(*(*(network_weights_derivatives_values_threads + thread_id) + weight_index_base));
			}

			free(*(*(network_weights_derivatives_pointers_manager_threads + thread_id) + neuron_index));
			free(*(*(network_weights_pointers_manager_threads + thread_id) + neuron_index));
		}
		free(*(network_weights_derivatives_pointers_manager_threads + thread_id));
		free(*(network_weights_pointers_manager_threads + thread_id));
		free(*(network_weights_derivatives_values_threads + thread_id));
		free(*(network_weights_values_threads + thread_id));

		delete *(ann_threads + thread_id);
	}

	free(ann_threads);
}



void backpropagationBasedANN::deallocateTrainingDataParallel()
{
	for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
	{
		const unsigned int current_thread_batch_size =
			(batch_size_per_thread * (thread_id + 1) > training_data_size) ?
			batch_size_per_thread * (thread_id + 1) :
			(training_data_size - batch_size_per_thread * thread_id);

		for (unsigned int pattern_index = 0; pattern_index < current_thread_batch_size; pattern_index++)
		{
			free(*(*(groundtruth_data_threads + thread_id) + pattern_index));
			free(*(*(training_data_threads + thread_id) + pattern_index));
		}

		free(*(groundtruth_data_threads + thread_id));
		free(*(training_data_threads + thread_id));
	}

	free(groundtruth_data_threads);
	free(training_data_threads);
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
		const unsigned int current_neuron_inputs_count = my_ann->getWeightedInputsInNeuron(neuron_index);

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

		weight_index_base += current_neuron_inputs_count;
	}

	my_ann->setNetworkWeightsAndDerivatives(&network_weights_pointers_manager, 
		&network_weights_derivatives_pointers_manager, false);

#ifdef _OPENMP
	batch_size_per_thread = (unsigned int)ceil((double)training_data_size / (double)available_threads);
	allocateTrainingDataParallel();
	allocateNetworkArchitectureParallel();
#endif //_OPENMP

	if (hessian_matrix_was_required)
	{
		// Allocate memory for the weights update directions:
		hessian_matrix = (double*)malloc(weights_count * (weights_count + 1) / 2 * sizeof(double));
		previous_hessian_matrix = (double*)malloc(weights_count * (weights_count + 1) / 2 * sizeof(double));

		jacobian_error_derivative_product = (double*)malloc(weights_count * sizeof(double));
		previous_jacobian_error_derivative_product = (double*)malloc(weights_count * sizeof(double));
		previous_weights_values = (double*)malloc(weights_count * sizeof(double));

#ifdef _OPENMP
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
	double squared_gradient_norm;

	// Reset the weights and bias delta values to 0:
	memset(weights_deltas, 0, weights_count * sizeof(double));

	TIMERS;
	GETTIME_INI;
	double total_epoch_loss = 0.0;
	for (unsigned int pattern_index = 0; pattern_index < training_data_size; pattern_index++)
	{
		// Use the current pattern to feed the network:
		my_ann->assignInputPatternPointer(*(training_data + pattern_index));
		my_ann->assignGroundtruthPointer(*(groundtruth_data + pattern_index));

		// Perform the Feed forward:
		total_epoch_loss += my_ann->computeNetworkLossWithDerivatives();
		my_ann->backPropagateErrors();

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
	printf("Epoch computed successfully, gradient norm = %f, first weight = %f\n", squared_gradient_norm, *network_weights_values);

	if ((current_loss < target_loss) || (squared_gradient_norm < target_loss*target_loss))
	{
		return false;
	}

	return true;
}


#ifndef _OPENMP
bool backpropagationBasedANN::computeEpoch_levenberg_marquardt()
{	
	double squared_gradient_norm;
	unsigned int worsening_count = 0;
	double training_epoch_loss;
	double total_epoch_loss = 0.0;

	// Set the hessian and jacobian arrays to 0, to perform a cummulated sum of their values:
	memset(hessian_matrix, 0, weights_count * (weights_count + 1) / 2 * sizeof(double));
	memset(jacobian_error_derivative_product, 0, weights_count * sizeof(double));
	
	// Perform the feed-forward process for each pattern in the training set:
	for (unsigned int pattern_index = 0; pattern_index < training_data_size; pattern_index++)
	{
		// Use the current pattern to feed the network:
		my_ann->assignInputPatternPointer(*(training_data + pattern_index));
		my_ann->assignGroundtruthPointer(*(groundtruth_data + pattern_index));

		// Perform the Feed forward:
		total_epoch_loss += my_ann->computeNetworkLossWithDerivatives(true);

		// Backpropagate the neuron's error contribution:
		my_ann->backPropagateErrors();

		for (unsigned int weight_index_i = 0; weight_index_i < weights_count; weight_index_i++)
		{
			// Compute the product between the Jacobian matrix and the vector of errors:
			double error_contribution_product = 0.0;
			for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
			{
				*(jacobian_error_derivative_product + weight_index_i) =
					*(jacobian_error_derivative_product + weight_index_i) +
					*(*(network_weights_derivatives_values + weight_index_i) + output_index) *
					my_ann->getLossFunctionErrorContribution(output_index);

				/* Compute the hessian matrix entry that corresponds to 
				the product of the current weight and the first weight of the network:
				*/
				error_contribution_product += *(*(network_weights_derivatives_values + weight_index_i) + output_index) *
					*(*network_weights_derivatives_values + output_index);
			}
			
			const unsigned int weight_index_base = weight_index_i * (weight_index_i + 1) / 2;
			*(hessian_matrix + weight_index_base) = *(hessian_matrix + weight_index_base) + error_contribution_product;

			// Compute the jacobian matrix product with its self transpose:
			for (unsigned int weight_index_j = 1; weight_index_j <= weight_index_i; weight_index_j++)
			{
				double error_contribution_product = 0.0;
				for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
				{
					error_contribution_product += *(*(network_weights_derivatives_values + weight_index_i) + output_index) *
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
			unsigned int weight_index_i;
			for (weight_index_i = 0; weight_index_i < weights_count; weight_index_i++)
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
						*(hessian_matrix + weight_index_base_j + weight_index_j);

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
					memcpy(jacobian_error_derivative_product, previous_jacobian_error_derivative_product, (weight_index_i) * sizeof(double));
					break;
				}

				*(hessian_matrix + weight_index_base_i + weight_index_i) = sqrt(difference);

				// Compute the i-th entry of the temporal 'z' vector
				*(jacobian_error_derivative_product + weight_index_i) = (*(jacobian_error_derivative_product + weight_index_i) - jacobian_factorized_hessian_product) / *(hessian_matrix + weight_index_base_i + weight_index_i);
			}

			if (weight_index_i == weights_count)
			{
				break;
			}
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
			squared_gradient_norm += *(jacobian_error_derivative_product + weight_index_i) *
				*(jacobian_error_derivative_product + weight_index_i);
		}

		// Evaluate the new weight values:
		training_epoch_loss = 0.0;

		for (unsigned int pattern_index = 0; pattern_index < training_data_size; pattern_index++)
		{
			// Use the current pattern to feed the network:
			my_ann->assignInputPatternPointer(*(training_data + pattern_index));
			my_ann->assignGroundtruthPointer(*(groundtruth_data + pattern_index));

			// Perform the Feed forward without the computation of the activation functions derivatives:
			training_epoch_loss += my_ann->computeNetworkLoss();
		}

		training_epoch_loss /= (double)training_data_size;
		if (training_epoch_loss < current_loss)
		{
			mu_value *= mu_decreasing_factor;
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
				memcpy(network_weights_values, previous_weights_values, weights_count * sizeof(double));

				memcpy(hessian_matrix, previous_hessian_matrix, weights_count * (weights_count  + 1) / 2 * sizeof(double));

				memcpy(jacobian_error_derivative_product, previous_jacobian_error_derivative_product,
					weights_count * sizeof(double));
			}
		}
	} while (1);
	
	printf("Epoch computed successfully, gradient norm = %f, first weight = %f\n", squared_gradient_norm, *network_weights_values);

	if ((mu_value > max_mu_value) || (current_loss < target_loss) || (squared_gradient_norm < target_loss*target_loss))
	{
		return false;
	}

	return true;
}


#else //_OPENMP
bool backpropagationBasedANN::computeEpoch_levenberg_marquardt()
{
	double total_epoch_loss = 0.0;

	const unsigned int training_data_size_thread_private = training_data_size;
	const unsigned int batch_size_per_thread_thread_private = batch_size_per_thread;
	const unsigned int weights_count_thread_private = weights_count;
	const unsigned int outputs_count_thread_private = outputs_count;

	double *** training_data_thread_shared = training_data_threads;
	int *** groundtruth_data_thread_shared = groundtruth_data_threads;
	ArtificialNeuralNetwork ** ann_thread_shared = ann_threads;

	double ** jacobian_error_derivative_product_thread_shared = jacobian_error_derivative_product_threads;
	double *** network_weights_derivatives_values_thread_shared = network_weights_derivatives_values_threads;
	double ** hessian_matrix_thread_shared = hessian_matrix_threads;

#pragma omp parallel default(none)\
firstprivate(\
training_data_size_thread_private,\
batch_size_per_thread_thread_private,\
weights_count_thread_private,\
outputs_count_thread_private\
)\
shared(\
total_epoch_loss,\
training_data_thread_shared,\
groundtruth_data_thread_shared,\
ann_thread_shared,\
jacobian_error_derivative_product_thread_shared,\
network_weights_derivatives_values_thread_shared,\
hessian_matrix_thread_shared\
)
	{
		/// Set the thread pointers to a simpler way to handle:
		const unsigned int current_thread_id = omp_get_thread_num();
		double ** current_thread_training_data = *(training_data_thread_shared + current_thread_id);
		int ** current_thread_groundtruth_data = *(groundtruth_data_thread_shared + current_thread_id);
		ArtificialNeuralNetwork * current_thread_ann = *(ann_thread_shared + current_thread_id);

		double * current_thread_jacobian_error_derivative_product = *(jacobian_error_derivative_product_thread_shared + current_thread_id);
		double ** current_thread_network_weights_derivatives_values =
			*(network_weights_derivatives_values_thread_shared + current_thread_id);
		double * current_thread_hessian_matrix = *(hessian_matrix_thread_shared + current_thread_id);

		const unsigned int current_thread_batch_size =
			(((current_thread_id + 1) * batch_size_per_thread_thread_private) > training_data_size_thread_private) ?
			(training_data_size_thread_private - current_thread_id * batch_size_per_thread_thread_private) :
			batch_size_per_thread_thread_private;

		// Set the hessian and jacobian arrays to 0, to perform a cummulated sum of their values:
		memset(current_thread_hessian_matrix, 0, weights_count_thread_private * (weights_count_thread_private + 1) / 2 * sizeof(double));
		memset(current_thread_jacobian_error_derivative_product, 0, weights_count_thread_private * sizeof(double));
		
		double current_thread_total_epoch_loss = 0.0;

		// Perform the feed-forward process for each pattern in the training set:
		for (unsigned int pattern_index = 0; pattern_index < current_thread_batch_size; pattern_index++)
		{
			// Use the current pattern to feed the network:
			current_thread_ann->assignInputPatternPointer(*(current_thread_training_data + pattern_index));
			current_thread_ann->assignGroundtruthPointer(*(current_thread_groundtruth_data + pattern_index));

			// Perform the Feed forward:
			current_thread_total_epoch_loss += current_thread_ann->computeNetworkLossWithDerivatives(true);

			// Backpropagate the neuron's error contribution:
			current_thread_ann->backPropagateErrors();

			for (unsigned int weight_index_i = 0; weight_index_i < weights_count_thread_private; weight_index_i++)
			{
				// Compute the product between the Jacobian matrix and the vector of errors:
				double error_contribution_product = 0.0;
				for (unsigned int output_index = 0; output_index < outputs_count_thread_private; output_index++)
				{
					*(current_thread_jacobian_error_derivative_product + weight_index_i) =
						*(current_thread_jacobian_error_derivative_product + weight_index_i) +
						*(*(current_thread_network_weights_derivatives_values + weight_index_i) + output_index) *
						current_thread_ann->getLossFunctionErrorContribution(output_index);

					/* Compute the hessian matrix entry that corresponds to
					the product of the current weight and the first weight of the network:
					*/
					error_contribution_product +=
						*(*(current_thread_network_weights_derivatives_values + weight_index_i) + output_index) *
						*(*current_thread_network_weights_derivatives_values + output_index);
				}

				const unsigned int weight_index_base = weight_index_i * (weight_index_i + 1) / 2;
				*(current_thread_hessian_matrix + weight_index_base) = *(current_thread_hessian_matrix + weight_index_base) + error_contribution_product;

				// Compute the jacobian matrix product with its self transpose:
				for (unsigned int weight_index_j = 1; weight_index_j <= weight_index_i; weight_index_j++)
				{
					double error_contribution_product = 0.0;
					for (unsigned int output_index = 0; output_index < outputs_count_thread_private; output_index++)
					{
						error_contribution_product +=
							*(*(current_thread_network_weights_derivatives_values + weight_index_i) + output_index) *
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
			total_epoch_loss = total_epoch_loss + current_thread_total_epoch_loss;
		}
	}

	// Set the hessian and jacobian arrays to 0, to perform a cummulated sum of their values:
	memset(hessian_matrix, 0, weights_count * (weights_count + 1) / 2 * sizeof(double));
	memset(jacobian_error_derivative_product, 0, weights_count * sizeof(double));

	for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
	{
		for (unsigned int weight_index = 0; weight_index < weights_count; weight_index++)
		{
			*(hessian_matrix + weight_index) = *(hessian_matrix + weight_index) + *(*(hessian_matrix_threads + thread_id) + weight_index);
			*(jacobian_error_derivative_product + weight_index) = *(jacobian_error_derivative_product + weight_index) + *(*(jacobian_error_derivative_product_threads + thread_id) + weight_index);
		}

		for (unsigned int weight_index = weights_count; weight_index < (weights_count * (weights_count + 1) / 2); weight_index++)
		{
			*(hessian_matrix + weight_index) = *(hessian_matrix + weight_index) + *(*(hessian_matrix_threads + thread_id) + weight_index);
		}
	}
	
	current_loss = total_epoch_loss / (double)training_data_size;

	// Save the current computed hessian matrix in case that it is singular:
	memcpy(previous_hessian_matrix, hessian_matrix, weights_count * (weights_count + 1) / 2 * sizeof(double));
	memcpy(previous_jacobian_error_derivative_product, jacobian_error_derivative_product, weights_count * sizeof(double));
	memcpy(previous_weights_values, network_weights_values, weights_count * sizeof(double));

	double training_epoch_loss;
	double squared_gradient_norm;
	unsigned int worsening_count = 0;
	do
	{
		do
		{
			// Perform the Cholesky's factorization to obtain L * L^T = H + mu * I
			// Solve L * z = J^T*e at the same time:
			unsigned int weight_index_i;
			for (weight_index_i = 0; weight_index_i < weights_count; weight_index_i++)
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
						*(hessian_matrix + weight_index_base_j + weight_index_j);

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
					memcpy(jacobian_error_derivative_product, previous_jacobian_error_derivative_product, (weight_index_i) * sizeof(double));
					break;
				}

				*(hessian_matrix + weight_index_base_i + weight_index_i) = sqrt(difference);

				// Compute the i-th entry of the temporal 'z' vector
				*(jacobian_error_derivative_product + weight_index_i) = (*(jacobian_error_derivative_product + weight_index_i) - jacobian_factorized_hessian_product) / *(hessian_matrix + weight_index_base_i + weight_index_i);
			}

			if (weight_index_i == weights_count)
			{
				break;
			}
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
			squared_gradient_norm += *(jacobian_error_derivative_product + weight_index_i) *
				*(jacobian_error_derivative_product + weight_index_i);
		}

		for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
		{
			memcpy(*(network_weights_values_threads + thread_id), network_weights_values, weights_count * sizeof(double));
		}

		// Evaluate the new weight values:
		training_epoch_loss = 0.0;

#pragma omp parallel default(none)\
firstprivate(\
training_data_size_thread_private,\
batch_size_per_thread_thread_private\
)\
shared(\
training_epoch_loss,\
training_data_thread_shared,\
groundtruth_data_thread_shared,\
ann_thread_shared\
)
		{
			/// Set the thread pointers to a simpler way to handle:
			const unsigned int current_thread_id = omp_get_thread_num();
			double ** current_thread_training_data = *(training_data_thread_shared + current_thread_id);
			int ** current_thread_groundtruth_data = *(groundtruth_data_thread_shared + current_thread_id);
			ArtificialNeuralNetwork * current_thread_ann = *(ann_thread_shared + current_thread_id);

			const unsigned int current_thread_batch_size =
				(((current_thread_id + 1) * batch_size_per_thread_thread_private) > training_data_size_thread_private) ?
				(training_data_size_thread_private - current_thread_id * batch_size_per_thread_thread_private) :
				batch_size_per_thread_thread_private;

			double current_thread_training_epoch_loss = 0.0;
			for (unsigned int pattern_index = 0; pattern_index < current_thread_batch_size; pattern_index++)
			{
				// Use the current pattern to feed the network:
				current_thread_ann->assignInputPatternPointer(*(current_thread_training_data + pattern_index));
				current_thread_ann->assignGroundtruthPointer(*(current_thread_groundtruth_data + pattern_index));

				// Perform the Feed forward without the computation of the activation functions derivatives:
				current_thread_training_epoch_loss += current_thread_ann->computeNetworkLoss();
			}
#pragma omp critical
			{
				training_epoch_loss = training_epoch_loss + current_thread_training_epoch_loss;
			}
		}

		training_epoch_loss /= (double)training_data_size;
		if (training_epoch_loss < current_loss)
		{
			mu_value *= mu_decreasing_factor;
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
				memcpy(network_weights_values, previous_weights_values, weights_count * sizeof(double));

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

	printf("Epoch computed successfully, gradient norm = %f, first weight = %f\n", squared_gradient_norm, *network_weights_values);

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
			my_ann->resetNetworkTime();
#ifdef _OPENMP
			for (unsigned int thread_id = 0; thread_id < available_threads; thread_id++)
			{
				(*(ann_threads + thread_id))->resetNetworkTime();
			}
#endif //_OPENMP
		}
		GETTIME_INI;
		continue_training = (this->*computeEpoch)();
		GETTIME_FIN;
		if ((save_each_n_epochs > 0) && !(current_epoch % save_each_n_epochs))
		{
			my_ann->saveNetworkState();
			saveState();
		}
		elapsed_time += DIFTIME;
		printf("Epochs: %i, average error = %.18f, epoch computation time = %f s, elapsed time = %f\n", current_epoch, current_loss, DIFTIME, elapsed_time);
	}


	return current_loss;
}



void backpropagationBasedANN::saveState()
{
	my_ann->saveNetworkState();
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
