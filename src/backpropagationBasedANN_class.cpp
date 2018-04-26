#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "random_numbers_generator.h"
#include "backpropagationBasedANN_class.h"

backpropagationBasedANN::backpropagationBasedANN()
{
	computeEpoch = &backpropagationBasedANN::computeEpoch_gradient_descent;

	weights_deltas = NULL;
	jacobian_error_derivative_product = NULL;
	hessian_matrix = NULL;

	network_current_time = 1;

	memory_already_allocated = false;
	hessian_matrix_was_required = false;
	previous_weights_values = NULL;
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
		if (hessian_matrix_was_required)
		{
			free(hessian_matrix);
			free(jacobian_error_derivative_product);
			free(previous_weights_values);
			free(previous_jacobian_error_derivative_product);
			free(previous_hessian_matrix);

#ifndef _OPENMP
			for (unsigned int thread_id = 0; thread_id < available_threads_count; thread_id++)
			{
				const unsigned int training_batch_size = (((thread_id + 1) * training_data_size_per_thread) <= training_data_size) ? training_data_size_per_thread : (training_data_size - thread_id * training_data_size_per_thread);
				for (unsigned int training_data_index = 0; training_data_index < training_batch_size; training_data_index++)
				{
					free(*(*(groundtruth_data_parallel + thread_id) + training_data_index));
					free(*(*(training_data_parallel + thread_id) + training_data_index));
				}

				free(*(groundtruth_data_parallel + thread_id));
				free(*(training_data_parallel + thread_id));
				
				for (unsigned int input_index = 0; input_index < inputs_count; input_index++)
				{
					delete *(*(network_input_nodes_parallel + thread_id) + input_index);
				}
				free(*(network_input_nodes_parallel + thread_id));

				unsigned int weights_and_bias_position = 0;
				for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
				{
					const unsigned int current_neuron_inputs_count = (*(*(network_neurons_parallel + thread_id) + neuron_index))->getInputsCount();

					for (unsigned int neuron_input_index = 0; neuron_input_index < current_neuron_inputs_count; neuron_input_index++, weights_and_bias_position++)
					{
						free(*(*(network_weights_derivative_values_parallel + thread_id) + weights_and_bias_position));
					}
					delete *(*(network_neurons_parallel + thread_id) + neuron_index);
				}

				free(*(network_neurons_parallel + thread_id));
				free(*(hessian_matrix_parallel + thread_id));
				free(*(jacobian_error_derivative_product_parallel + thread_id));
				free(*(network_weights_derivative_values_parallel + thread_id));
				free(*(network_weights_values_parallel + thread_id));
				
				for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
				{
					delete *(*(loss_functions_parallel + thread_id) + output_index);
				}
				free(*(loss_functions_parallel + thread_id));
				free(*(network_outputs_parallel + thread_id));
			}

			free(groundtruth_data_parallel);
			free(training_data_parallel);

			free(network_input_nodes_parallel);
			free(network_neurons_parallel);
			free(network_outputs_parallel);
			free(loss_functions_parallel);

			free(hessian_matrix_parallel);
			free(jacobian_error_derivative_product_parallel);
			free(network_weights_derivative_values_parallel);
			free(network_weights_values_parallel);
#endif // _OPENMP
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
	/*
	// Set the initial weights to a normal distibution of mean 0 and variance 1 for each value.
	for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
	{
		const unsigned int current_neuron_inputs_count = (*(network_neurons + neuron_index))->getInputsCount();
		(*(network_neurons + neuron_index))->updateWeightValue(0.0, 0);
		for (unsigned int neuron_input_index = 0; neuron_input_index < current_neuron_inputs_count; neuron_input_index++)
		{
			(*(network_neurons + neuron_index))->updateWeightValue(anorm_est(random_number_generator_seed), neuron_input_index + 1);
		}
	}
	*/
	if (hessian_matrix_was_required)
	{
		unsigned int weights_and_biases_for_allocation = weights_count + neurons_count;
		hessian_matrix = (double*)malloc(weights_and_biases_for_allocation * (weights_and_biases_for_allocation + 1) / 2 * sizeof(double));
		jacobian_error_derivative_product = (double*)malloc(weights_and_biases_for_allocation * sizeof(double));

		previous_weights_values = (double*)malloc(weights_and_biases_for_allocation * sizeof(double));
		previous_jacobian_error_derivative_product = (double*)malloc(weights_and_biases_for_allocation * sizeof(double));
		previous_hessian_matrix = (double*)malloc(weights_and_biases_for_allocation * (weights_and_biases_for_allocation + 1) / 2 * sizeof(double));

		memcpy(previous_weights_values, network_weights_values, weights_and_biases_for_allocation * sizeof(double));

#ifdef _OPENMP
#pragma omp parallel
		{
#pragma omp single
			available_threads_count = omp_get_num_threads();
		}

		groundtruth_data_parallel = (int***)malloc(available_threads_count * sizeof(int**));
		training_data_parallel = (double***)malloc(available_threads_count * sizeof(double**));

		network_input_nodes_parallel = (Input_pattern***)malloc(available_threads_count * sizeof(Input_pattern**));
		network_neurons_parallel = (Neuron***)malloc(available_threads_count * sizeof(Neuron**));
		network_outputs_parallel = (Neuron***)malloc(available_threads_count * sizeof(Neuron**));
		loss_functions_parallel = (LossFunction***)malloc(available_threads_count * sizeof(LossFunction**));

		hessian_matrix_parallel = (double**)malloc(available_threads_count * sizeof(double*));
		jacobian_error_derivative_product_parallel = (double**)malloc(available_threads_count * sizeof(double*));
		network_weights_derivative_values_parallel = (double***)malloc(available_threads_count * sizeof(double**));
		network_weights_values_parallel = (double**)malloc(available_threads_count * sizeof(double*));

		training_data_size_per_thread = (unsigned int)ceil((double)training_data_size / (double)available_threads_count);

		for (unsigned int thread_id = 0; thread_id < available_threads_count; thread_id++)
		{
			const unsigned int training_batch_size = (((thread_id + 1) * training_data_size_per_thread) <= training_data_size) ? training_data_size_per_thread : (training_data_size - thread_id * training_data_size_per_thread);

			*(groundtruth_data_parallel + thread_id) = (int**)malloc(training_batch_size * sizeof(int*));
			*(training_data_parallel + thread_id) = (double**)malloc(training_batch_size * sizeof(double*));
			for (unsigned int training_data_index = 0; training_data_index < training_batch_size; training_data_index++)
			{
				*(*(groundtruth_data_parallel + thread_id) + training_data_index) = (int*)malloc(outputs_count * sizeof(int));
				memcpy(*(*(groundtruth_data_parallel + thread_id) + training_data_index),
					*(groundtruth_data + training_data_index + thread_id * training_data_size_per_thread),
					outputs_count * sizeof(int));

				*(*(training_data_parallel + thread_id) + training_data_index) = (double*)malloc(inputs_count * sizeof(double));
				memcpy(*(*(training_data_parallel + thread_id) + training_data_index),
					*(training_data + training_data_index + thread_id * training_data_size_per_thread),
					inputs_count * sizeof(double));
			}

			*(network_input_nodes_parallel + thread_id) = (Input_pattern**)malloc(inputs_count * sizeof(Input_pattern*));
			*(network_neurons_parallel + thread_id) = (Neuron**)malloc(neurons_count * sizeof(Neuron*));
			*(network_outputs_parallel + thread_id) = (Neuron**)malloc(outputs_count * sizeof(Neuron*));
			*(loss_functions_parallel + thread_id) = (LossFunction**)malloc(outputs_count * sizeof(LossFunction*));
			*(hessian_matrix_parallel + thread_id) = (double*)malloc((weights_count + neurons_count) * (weights_count + neurons_count + 1) / 2 * sizeof(double));
			*(jacobian_error_derivative_product_parallel + thread_id) = (double*)malloc((weights_count + neurons_count) * sizeof(double));
			*(network_weights_derivative_values_parallel + thread_id) = (double**)malloc((weights_count + neurons_count) * sizeof(double*));
			*(network_weights_values_parallel + thread_id) = (double*)malloc((weights_count + neurons_count) * sizeof(double));
			memcpy(*(network_weights_values_parallel + thread_id), network_weights_values, (weights_count + neurons_count) * sizeof(double));

			for (unsigned int input_index = 0; input_index < inputs_count; input_index++)
			{
				*(*(network_input_nodes_parallel + thread_id) + input_index) = new Input_pattern;
				(*(*(network_input_nodes_parallel + thread_id) + input_index))->setInputPointerPosition((*(network_input_nodes + input_index))->getInputPointerPosition());
			}

			unsigned int weights_and_bias_position = 0;
			for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
			{
				*(*(network_weights_derivative_values_parallel + thread_id) + weights_and_bias_position) = (double*)malloc(outputs_count * sizeof(double));

				*(*(network_neurons_parallel + thread_id) + neuron_index) = new Neuron((*(network_neurons + neuron_index))->getNeuronPosition(), outputs_count);

				(*(*(network_neurons_parallel + thread_id) + neuron_index))->assignExternalWeightErrorContributionPointer(*(network_weights_derivative_values_parallel + thread_id) + weights_and_bias_position);
				(*(*(network_neurons_parallel + thread_id) + neuron_index))->assignExternalWeightPointer(*(network_weights_values_parallel + thread_id) + weights_and_bias_position);

				const unsigned int neuron_intputs_count = (*(network_neurons + neuron_index))->getInputsCount();
				for (unsigned int neuron_input_index = 1; neuron_input_index <= neuron_intputs_count; neuron_input_index++)
				{
					*(*(network_weights_derivative_values_parallel + thread_id) + weights_and_bias_position + neuron_input_index) = (double*)malloc(outputs_count * sizeof(double));

					if ((*(network_neurons + neuron_index))->getInputType(neuron_input_index) == Weight_node::WIT_NEURON)
					{
						(*(*(network_neurons_parallel + thread_id) + neuron_index))->addInputNode(
							*(*(network_neurons_parallel + thread_id) + (*(network_neurons + neuron_index))->getInputPosition(neuron_input_index)),
							0.0,
							(*(network_neurons + neuron_index))->getInputType(neuron_input_index),
							(*(network_neurons + neuron_index))->getInputPosition(neuron_input_index)
						);
					}
					else
					{
						(*(*(network_neurons_parallel + thread_id) + neuron_index))->addInputNode(
							*(*(network_input_nodes_parallel + thread_id) + (*(network_neurons + neuron_index))->getInputPosition(neuron_input_index)),
							0.0,
							(*(network_neurons + neuron_index))->getInputType(neuron_input_index),
							(*(network_neurons + neuron_index))->getInputPosition(neuron_input_index)
						);
					}
				}

				const unsigned int neuron_activation_function_parameters_count = (*(network_neurons + neuron_index))->getActivationFunctionParametersCount();

				double * parameters_array = NULL;

				if (neuron_activation_function_parameters_count)
				{
					parameters_array = (double*)malloc(neuron_activation_function_parameters_count * sizeof(double));
					for (unsigned int parameter_index = 0; parameter_index < neuron_activation_function_parameters_count; parameter_index++)
					{
						*(parameters_array + parameter_index) = (*(network_neurons + neuron_index))->getActivationFunctionParameterValue(parameter_index);
					}
				}

				(*(*(network_neurons_parallel + thread_id) + neuron_index))->setActivationFunction((*(network_neurons + neuron_index))->getActivationFunctionType(), parameters_array);

				if (neuron_activation_function_parameters_count)
				{
					free(parameters_array);
				}

				weights_and_bias_position += neuron_intputs_count + 1;
			}

			*(loss_functions_parallel + thread_id) = (LossFunction**)malloc(outputs_count * sizeof(LossFunction*));

			for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
			{
				*(*(network_outputs_parallel + thread_id) + output_index) = *(*(network_neurons_parallel + thread_id) + (*(network_output_nodes + output_index))->getNeuronPosition());
				switch (*(loss_functions_types + output_index))
				{
				case TrainedArtificialNeuralNetwork::LF_CROSS_ENTROPY:
					*(*(loss_functions_parallel + thread_id) + output_index) = new crossEntropyLossFunction;
					break;
				case TrainedArtificialNeuralNetwork::LF_L1_NORM:
					*(*(loss_functions_parallel + thread_id) + output_index) = new L1LossFunction;
					break;
				case TrainedArtificialNeuralNetwork::LF_L2_NORM:
					*(*(loss_functions_parallel + thread_id) + output_index) = new L2LossFunction;
					break;
				}
				(*(*(loss_functions_parallel + thread_id) + output_index))->setOutputNode(*(*(network_outputs_parallel + thread_id) + output_index));
			}
		}
#endif // _OPENMP
	}
	else
	{
		weights_deltas = (double*)malloc((neurons_count + weights_count) * sizeof(double));
	}

	memory_already_allocated = true;
	return 1;
}



bool backpropagationBasedANN::computeEpoch_gradient_descent()
{
	double squared_gradient_norm;

	// Reset the weights and bias delta values to 0:
	memset(weights_deltas, 0, (neurons_count + weights_count) * sizeof(double));

	double total_epoch_loss = 0.0;
	for (unsigned int pattern_index = 0; pattern_index < training_data_size; pattern_index++, network_current_time++)
	{
		// Use the current pattern to feed the network:
		for (unsigned int input_index = 0; input_index < inputs_count; input_index++)
		{
			(*(network_input_nodes + input_index))->setInputPointer(*(training_data + pattern_index));
		}

		// Perform the Feed forward:
		for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
		{
			(*(loss_functions + output_index))->setGroundtruth(*(groundtruth_data + pattern_index) + output_index);
			total_epoch_loss += (*(loss_functions + output_index))->computeLoss(network_current_time);

			// Backpropagate the error contribution:
			(*(loss_functions + output_index))->backpropagateErrorDerivative(output_index);
		}

		// Backpropagate the neuron's error contribution:
		for (int neuron_index = (neurons_count - 1); neuron_index >= 0; neuron_index--)
		{
			(*(network_neurons + neuron_index))->backpropagateNodeError();
		}

		squared_gradient_norm = 0.0;
		for (int weights_deltas_position = 0; weights_deltas_position < (weights_count + neurons_count); weights_deltas_position++)
		{
			double weight_error_contribution = 0.0;
			for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
			{
				weight_error_contribution += *(*(network_weights_derivative_values + weights_deltas_position) + output_index);
			}
			*(weights_deltas + weights_deltas_position) = *(weights_deltas + weights_deltas_position) * momentums +
				learning_rates * weight_error_contribution;

			// update the weights and bias values:
			*(network_weights_values + weights_deltas_position) = *(network_weights_values + weights_deltas_position) - *(weights_deltas + weights_deltas_position);
			squared_gradient_norm += *(weights_deltas + weights_deltas_position) * *(weights_deltas + weights_deltas_position);
		}
	}

	current_loss = total_epoch_loss / (double)training_data_size;

	if ((current_loss < target_loss) || (squared_gradient_norm < 1e-16))
	{
		return false;
	}

	return true;
}



bool backpropagationBasedANN::computeEpoch_levenberg_marquardt()
{

#ifndef _OPENMP
	resetNetworkTime();
	for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
	{
		(*(loss_functions + output_index))->resetErrorCurrentTime();
	}
#else // _OPENMP
	for (unsigned int thread_id = 0; thread_id < available_threads_count; thread_id++)
	{
		for (unsigned int neuron_index = 0; neuron_index < neurons_count; neuron_index++)
		{
			(*(*(network_neurons_parallel + thread_id) + neuron_index))->resetNodeCurrentTime();
		}

		for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
		{
			(*(*(loss_functions_parallel + thread_id) + output_index))->resetErrorCurrentTime();
		}
	}
#endif // _OPENMP
	
	double squared_gradient_norm;
	unsigned int worsening_count = 0;
	double training_epoch_loss;
	double total_epoch_loss = 0.0;

#ifndef _OPENMP
	memset(hessian_matrix, 0, (weights_count + neurons_count) * (weights_count + neurons_count + 1) / 2 * sizeof(double));
	memset(jacobian_error_derivative_product, 0, (weights_count + neurons_count) * sizeof(double));
	for (unsigned int pattern_index = 0; pattern_index < training_data_size; pattern_index++, network_current_time++)
	{
		// Use the current pattern to feed the network:
		for (unsigned int input_index = 0; input_index < inputs_count; input_index++)
		{
			(*(network_input_nodes + input_index))->setInputPointer(*(training_data + pattern_index));
		}

		// Perform the Feed forward:
		for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
		{
			(*(loss_functions + output_index))->setGroundtruth(*(groundtruth_data + pattern_index) + output_index);

			total_epoch_loss += (*(loss_functions + output_index))->computeLoss(network_current_time);

			(*(network_output_nodes + output_index))->addNodeErrorContribution(1.0, output_index);
		}

		// Backpropagate the neuron's error contribution:
		for (int neuron_index = (neurons_count - 1); neuron_index >= 0; neuron_index--)
		{
			// Backpropagate the error contribution:
			(*(network_neurons + neuron_index))->backpropagateNodeError();
		}

		for (unsigned int weights_and_bias_position_i = 0; weights_and_bias_position_i < (weights_count + neurons_count); weights_and_bias_position_i++)
		{
			const unsigned int weights_and_bias_position_base_i = weights_and_bias_position_i * (weights_and_bias_position_i + 1) / 2;
			double hessian_entry = 0.0;
			for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
			{
				*(jacobian_error_derivative_product + weights_and_bias_position_i) =
					*(jacobian_error_derivative_product + weights_and_bias_position_i) +
					*(*(network_weights_derivative_values + weights_and_bias_position_i) + output_index) *
					(*(loss_functions + output_index))->getErrorDerivative();

				hessian_entry += *(*(network_weights_derivative_values + weights_and_bias_position_i) + output_index) *
					*(*network_weights_derivative_values + output_index);
			}
			*(hessian_matrix + weights_and_bias_position_base_i) =
				*(hessian_matrix + weights_and_bias_position_base_i) + hessian_entry;
			

			for (unsigned int weights_and_bias_position_j = 1; weights_and_bias_position_j <= weights_and_bias_position_i; weights_and_bias_position_j++)
			{
				hessian_entry = 0.0;
				for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
				{
					hessian_entry += *(*(network_weights_derivative_values + weights_and_bias_position_i) + output_index) *
						*(*(network_weights_derivative_values + weights_and_bias_position_j) + output_index);
				}

				*(hessian_matrix + weights_and_bias_position_base_i + weights_and_bias_position_j) =
					*(hessian_matrix + weights_and_bias_position_base_i + weights_and_bias_position_j) + hessian_entry;
			}
		}
	}
#else	// _OPENMP
	const unsigned int training_data_size_parallel_temp = training_data_size;
	const unsigned int training_data_size_per_thread_parallel_temp = training_data_size_per_thread;
	const unsigned int inputs_count_parallel_temp = inputs_count;
	const unsigned int outputs_count_parallel_temp = outputs_count;
	const unsigned int neurons_count_parallel_temp = neurons_count;
	const unsigned int weights_count_parallel_temp = weights_count;

	int *** groundtruth_data_parallel_temp = groundtruth_data_parallel;
	double *** training_data_parallel_temp = training_data_parallel;
	Input_pattern *** network_input_nodes_parallel_temp = network_input_nodes_parallel;
	Neuron *** network_neurons_parallel_temp = network_neurons_parallel;
	Neuron *** network_outputs_parallel_temp = network_outputs_parallel;
	LossFunction *** loss_functions_parallel_temp = loss_functions_parallel;
	double ** hessian_matrix_parallel_temp = hessian_matrix_parallel;
	double ** jacobian_error_derivative_product_parallel_temp = jacobian_error_derivative_product_parallel;
	double *** network_weights_derivative_values_parallel_temp = network_weights_derivative_values_parallel;
	double ** network_weights_values_parallel_temp = network_weights_values_parallel;

	unsigned long long network_current_time_parallel_temp = network_current_time;

#pragma omp parallel default(none) firstprivate(training_data_size_parallel_temp,\
 training_data_size_per_thread_parallel_temp,\
 inputs_count_parallel_temp,\
 outputs_count_parallel_temp,\
 neurons_count_parallel_temp,\
 weights_count_parallel_temp)\
									shared(total_epoch_loss,\
 groundtruth_data_parallel_temp,\
 training_data_parallel_temp,\
 network_input_nodes_parallel_temp,\
 network_neurons_parallel_temp,\
 network_outputs_parallel_temp,\
 loss_functions_parallel_temp,\
 hessian_matrix_parallel_temp,\
 jacobian_error_derivative_product_parallel_temp,\
 network_weights_derivative_values_parallel_temp,\
 network_weights_values_parallel_temp,\
 network_current_time_parallel_temp)
	{
		const unsigned int current_threads_id = omp_get_thread_num();
		unsigned long long network_current_time_thread = network_current_time_parallel_temp;

		int ** ground_truth_thread = *(groundtruth_data_parallel_temp + current_threads_id);
		double ** training_data_thread = *(training_data_parallel_temp + current_threads_id);

		Input_pattern ** network_input_nodes_thread = *(network_input_nodes_parallel_temp + current_threads_id);
		Neuron ** network_neurons_thread = *(network_neurons_parallel_temp + current_threads_id);
		Neuron ** network_outputs_thread = *(network_outputs_parallel_temp + current_threads_id);
		LossFunction ** loss_functions_thread = *(loss_functions_parallel_temp + current_threads_id);

		double * hessian_matrix_thread = *(hessian_matrix_parallel_temp + current_threads_id);
		double * jacobian_error_derivative_product_thread = *(jacobian_error_derivative_product_parallel_temp + current_threads_id);
		double ** network_weights_derivative_values_thread = *(network_weights_derivative_values_parallel_temp + current_threads_id);
		double * network_weights_values_thread = *(network_weights_values_parallel_temp + current_threads_id);

		memset(hessian_matrix_thread, 0, (weights_count_parallel_temp + neurons_count_parallel_temp) * (weights_count_parallel_temp + neurons_count_parallel_temp + 1) / 2 * sizeof(double));
		memset(jacobian_error_derivative_product_thread, 0, (weights_count_parallel_temp + neurons_count_parallel_temp) * sizeof(double));

		double total_thread_loss = 0.0;
		const unsigned int training_batch_size = (((current_threads_id + 1) * training_data_size_per_thread_parallel_temp) < training_data_size_parallel_temp) ?
			training_data_size_per_thread_parallel_temp :
			(training_data_size_parallel_temp - current_threads_id * training_data_size_per_thread_parallel_temp);

		for (unsigned int pattern_index = 0; pattern_index < training_batch_size; pattern_index++, network_current_time_thread++)
		{
			for (unsigned int input_index = 0; input_index < inputs_count_parallel_temp; input_index++)
			{
				(*(network_input_nodes_thread + input_index))->setInputPointer(*(training_data_thread + pattern_index));
			}

			for (unsigned int output_index = 0; output_index < outputs_count_parallel_temp; output_index++)
			{
				(*(loss_functions_thread + output_index))->setGroundtruth(*(ground_truth_thread + pattern_index) + output_index);
				total_thread_loss += (*(loss_functions_thread + output_index))->computeLoss(network_current_time_thread);
				(*(network_outputs_thread + output_index))->addNodeErrorContribution(1.0, output_index);
			}

			for (int neuron_index = neurons_count_parallel_temp - 1; neuron_index >= 0; neuron_index--)
			{
				(*(network_neurons_thread + neuron_index))->backpropagateNodeError();
			}

			for (unsigned int weights_and_bias_position_i = 0; weights_and_bias_position_i < (weights_count_parallel_temp + neurons_count_parallel_temp); weights_and_bias_position_i++)
			{
				const unsigned int weights_and_bias_position_base_i = weights_and_bias_position_i * (weights_and_bias_position_i + 1) / 2;
				double hessian_entry = 0.0;
				for (unsigned int output_index = 0; output_index < outputs_count_parallel_temp; output_index++)
				{
					*(jacobian_error_derivative_product_thread + weights_and_bias_position_i) =
						*(jacobian_error_derivative_product_thread + weights_and_bias_position_i) +
						*(*(network_weights_derivative_values_thread + weights_and_bias_position_i) + output_index) *
						(*(loss_functions_thread + output_index))->getErrorDerivative();

					hessian_entry += *(*(network_weights_derivative_values_thread + weights_and_bias_position_i) + output_index) *
						*(*network_weights_derivative_values_thread + output_index);
				}
				*(hessian_matrix_thread + weights_and_bias_position_base_i) =
					*(hessian_matrix_thread + weights_and_bias_position_base_i) + hessian_entry;

				for (unsigned int weights_and_bias_position_j = 1; weights_and_bias_position_j <= weights_and_bias_position_i; weights_and_bias_position_j++)
				{
					hessian_entry = 0.0;
					for (unsigned int output_index = 0; output_index < outputs_count_parallel_temp; output_index++)
					{
						hessian_entry += *(*(network_weights_derivative_values_thread + weights_and_bias_position_i) + output_index) *
							*(*(network_weights_derivative_values_thread + weights_and_bias_position_j) + output_index);
					}

					*(hessian_matrix_thread + weights_and_bias_position_base_i + weights_and_bias_position_j) =
						*(hessian_matrix_thread + weights_and_bias_position_base_i + weights_and_bias_position_j) + hessian_entry;
				}
			}

		}
#pragma omp critical
		{
			total_epoch_loss += total_thread_loss;
			if (network_current_time_thread > network_current_time_parallel_temp)
			{
				network_current_time_parallel_temp = network_current_time_thread;
			}
		}
	}
	
	memset(hessian_matrix, 0, (weights_count + neurons_count) * (weights_count + neurons_count + 1) / 2 * sizeof(double));
	memset(jacobian_error_derivative_product, 0, (weights_count + neurons_count) * sizeof(double));
	for (unsigned int thread_id = 0; thread_id < available_threads_count; thread_id++)
	{
		for (unsigned int weights_and_bias_position_i = 0; weights_and_bias_position_i < (weights_count + neurons_count); weights_and_bias_position_i++)
		{
			const unsigned int weights_and_bias_position_base_i = weights_and_bias_position_i * (weights_and_bias_position_i + 1) / 2;
			for (unsigned int weights_and_bias_position_j = 0; weights_and_bias_position_j <= weights_and_bias_position_i; weights_and_bias_position_j++)
			{
				*(hessian_matrix + weights_and_bias_position_base_i + weights_and_bias_position_j) =
					*(hessian_matrix + weights_and_bias_position_base_i + weights_and_bias_position_j) +
					*(*(hessian_matrix_parallel + thread_id) + weights_and_bias_position_base_i + weights_and_bias_position_j);
			}

			*(jacobian_error_derivative_product + weights_and_bias_position_i) = *(jacobian_error_derivative_product + weights_and_bias_position_i) + *(*(jacobian_error_derivative_product_parallel + thread_id) + weights_and_bias_position_i);
		}
	}
	network_current_time = network_current_time_parallel_temp;

#endif // _OPENMP
	current_loss = total_epoch_loss / (double)training_data_size;

	// Save the current computed hessian matrix in case that it is singular:
	memcpy(previous_hessian_matrix, hessian_matrix, (weights_count + neurons_count) *  (weights_count + neurons_count + 1) / 2 * sizeof(double));
	memcpy(previous_jacobian_error_derivative_product, jacobian_error_derivative_product, (weights_count + neurons_count) * sizeof(double));
	do
	{
		do
		{
			// Perform the Cholesky's factorization to obtain L * L^T = H + mu * I
			// Solve L * z = J^T*e at the same time:
			for (unsigned int hessian_matrix_position_i = 0; hessian_matrix_position_i < (weights_count + neurons_count); hessian_matrix_position_i++)
			{
				const unsigned int hessian_position_base_i = hessian_matrix_position_i*(hessian_matrix_position_i + 1) / 2;

				// Sum of the squared values of the current row, necessary to obtain the diagonal value:
				double row_squared_sum = 0.0;
				double matrix_vector_product_sum = 0.0;
				for (unsigned int hessian_matrix_position_j = 0; hessian_matrix_position_j < hessian_matrix_position_i; hessian_matrix_position_j++)
				{
					const unsigned int hessian_position_base_j = hessian_matrix_position_j * (hessian_matrix_position_j + 1) / 2;
					double product_sum = 0.0;
					for (unsigned int hessian_matrix_position_k = 0; hessian_matrix_position_k < hessian_matrix_position_j; hessian_matrix_position_k++)
					{
						product_sum +=
							*(hessian_matrix + hessian_position_base_j + hessian_matrix_position_k) *  /* Column j of the matrix L */
							*(hessian_matrix + hessian_position_base_i + hessian_matrix_position_k);   /* Row i of the matrix L */
					}

					*(hessian_matrix + hessian_position_base_i + hessian_matrix_position_j) = (*(hessian_matrix + hessian_position_base_i + hessian_matrix_position_j) - product_sum) / *(hessian_matrix + hessian_position_base_j + hessian_matrix_position_j);
					row_squared_sum += *(hessian_matrix + hessian_position_base_i + hessian_matrix_position_j) * *(hessian_matrix + hessian_position_base_i + hessian_matrix_position_j);
					matrix_vector_product_sum += *(hessian_matrix + hessian_position_base_i + hessian_matrix_position_j) * *(jacobian_error_derivative_product + hessian_matrix_position_j);
				}

				// The diagonal value is incremented by the mu value in order to convert the matrix into invertible:
				if ((*(hessian_matrix + hessian_position_base_i + hessian_matrix_position_i) + mu_value - row_squared_sum) < 1e-12)
				{
					memcpy(hessian_matrix, previous_hessian_matrix, (weights_count + neurons_count) *  (weights_count + neurons_count + 1) / 2 * sizeof(double));
					memcpy(jacobian_error_derivative_product, previous_jacobian_error_derivative_product, (weights_count + neurons_count) * sizeof(double));
					mu_value *= mu_increasing_factor;
					continue;
				}

				*(hessian_matrix + hessian_position_base_i + hessian_matrix_position_i) = sqrt(*(hessian_matrix + hessian_position_base_i + hessian_matrix_position_i) + mu_value - row_squared_sum);
				*(jacobian_error_derivative_product + hessian_matrix_position_i) = (*(jacobian_error_derivative_product + hessian_matrix_position_i) - matrix_vector_product_sum) / *(hessian_matrix + hessian_position_base_i + hessian_matrix_position_i);
			}
			break;
		} while (1);
		
		squared_gradient_norm = 0.0;
		// Solve the equation system L^T * w = z
		for (int weights_and_bias_position_i = (weights_count + neurons_count - 1); weights_and_bias_position_i >= 0; weights_and_bias_position_i--)
		{
			double matrix_vector_product_sum = 0.0;
			unsigned int weight_and_bias_position_base_i = weights_and_bias_position_i * (weights_and_bias_position_i + 1) / 2;
			for (int weights_and_bias_position_j = (weights_count + neurons_count - 1); weights_and_bias_position_j > weights_and_bias_position_i; weights_and_bias_position_j--)
			{
				unsigned int hessian_matrix_position_base_j = weights_and_bias_position_j * (weights_and_bias_position_j + 1) / 2;
				matrix_vector_product_sum += *(hessian_matrix + hessian_matrix_position_base_j + weights_and_bias_position_i) * *(jacobian_error_derivative_product + weights_and_bias_position_j);
			}
			*(jacobian_error_derivative_product + weights_and_bias_position_i) = (*(jacobian_error_derivative_product + weights_and_bias_position_i) - matrix_vector_product_sum) / *(hessian_matrix + weight_and_bias_position_base_i + weights_and_bias_position_i);

			// Update the weights at the same time:
			*(network_weights_values + weights_and_bias_position_i) = *(network_weights_values + weights_and_bias_position_i) - (double)*(jacobian_error_derivative_product + weights_and_bias_position_i);
			squared_gradient_norm += *(jacobian_error_derivative_product + weights_and_bias_position_i) * *(jacobian_error_derivative_product + weights_and_bias_position_i);
		}

		// Evaluate the new weight values:
		training_epoch_loss = 0.0;
#ifndef _OPENMP
		for (unsigned int pattern_index = 0; pattern_index < training_data_size; pattern_index++, network_current_time++)
		{
			// Use the current pattern to feed the network:
			for (unsigned int input_index = 0; input_index < inputs_count; input_index++)
			{
				(*(network_input_nodes + input_index))->setInputPointer(*(training_data + pattern_index));
			}

			// Perform the Feed forward:
			for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
			{
				(*(loss_functions + output_index))->setGroundtruth(*(groundtruth_data + pattern_index) + output_index);
				training_epoch_loss += (*(loss_functions + output_index))->computeLoss(network_current_time);
			}
		}
#else // _OPENMP
		for (unsigned int thread_id = 0; thread_id < available_threads_count; thread_id++)
		{
			memcpy(*(network_weights_values_parallel + thread_id), network_weights_values, (weights_count + neurons_count) * sizeof(double));
		}

		const unsigned int training_data_size_parallel_temp = training_data_size;
		const unsigned int training_data_size_per_thread_parallel_temp = training_data_size_per_thread;
		const unsigned int inputs_count_parallel_temp = inputs_count;
		const unsigned int outputs_count_parallel_temp = outputs_count;
		const unsigned int neurons_count_parallel_temp = neurons_count;
		const unsigned int weights_count_parallel_temp = weights_count;

		int *** groundtruth_data_parallel_temp = groundtruth_data_parallel;
		double *** training_data_parallel_temp = training_data_parallel;
		Input_pattern *** network_input_nodes_parallel_temp = network_input_nodes_parallel;
		Neuron *** network_neurons_parallel_temp = network_neurons_parallel;
		Neuron *** network_outputs_parallel_temp = network_outputs_parallel;
		LossFunction *** loss_functions_parallel_temp = loss_functions_parallel;

		unsigned long long network_current_time_parallel_temp = network_current_time;

#pragma omp parallel default(none) firstprivate(training_data_size_parallel_temp,\
 training_data_size_per_thread_parallel_temp,\
 inputs_count_parallel_temp,\
 outputs_count_parallel_temp,\
 neurons_count_parallel_temp,\
 weights_count_parallel_temp)\
									shared(training_epoch_loss,\
 groundtruth_data_parallel_temp,\
 training_data_parallel_temp,\
 network_input_nodes_parallel_temp,\
 network_neurons_parallel_temp,\
 network_outputs_parallel_temp,\
 loss_functions_parallel_temp,\
 network_current_time_parallel_temp)
		{
			const unsigned int current_threads_id = omp_get_thread_num();
			unsigned long long network_current_time_thread = network_current_time_parallel_temp;

			int ** ground_truth_thread = *(groundtruth_data_parallel_temp + current_threads_id);
			double ** training_data_thread = *(training_data_parallel_temp + current_threads_id);

			Input_pattern ** network_input_nodes_thread = *(network_input_nodes_parallel_temp + current_threads_id);
			Neuron ** network_neurons_thread = *(network_neurons_parallel_temp + current_threads_id);
			Neuron ** network_outputs_thread = *(network_outputs_parallel_temp + current_threads_id);
			LossFunction ** loss_functions_thread = *(loss_functions_parallel_temp + current_threads_id);
						
			double training_thread_loss = 0.0;
			const unsigned int training_batch_size = (((current_threads_id + 1) * training_data_size_per_thread_parallel_temp) < training_data_size_parallel_temp) ?
				training_data_size_per_thread_parallel_temp :
				(training_data_size_parallel_temp - current_threads_id * training_data_size_per_thread_parallel_temp);

			for (unsigned int pattern_index = 0; pattern_index < training_batch_size; pattern_index++, network_current_time_thread++)
			{
				for (unsigned int input_index = 0; input_index < inputs_count_parallel_temp; input_index++)
				{
					(*(network_input_nodes_thread + input_index))->setInputPointer(*(training_data_thread + pattern_index));
				}

				for (unsigned int output_index = 0; output_index < outputs_count_parallel_temp; output_index++)
				{
					(*(loss_functions_thread + output_index))->setGroundtruth(*(ground_truth_thread + pattern_index) + output_index);
					training_thread_loss += (*(loss_functions_thread + output_index))->computeLoss(network_current_time_thread);
				}
			}
#pragma omp critical
			{
				training_epoch_loss += training_thread_loss;
				if (network_current_time_thread > network_current_time_parallel_temp)
				{
					network_current_time_parallel_temp = network_current_time_thread;
				}
			}
		}

		network_current_time = network_current_time_parallel_temp;
#endif // _OPENMP
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
				break;
			}
			else
			{
				memcpy(network_weights_values, previous_weights_values, (weights_count + neurons_count) * sizeof(double));	
				memcpy(hessian_matrix, previous_hessian_matrix, (weights_count + neurons_count) *  (weights_count + neurons_count + 1) / 2 * sizeof(double));
				memcpy(jacobian_error_derivative_product, previous_jacobian_error_derivative_product, (weights_count + neurons_count) * sizeof(double));
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
	}

	printf("Epochs: %i, average error = %.18f\n", current_epoch, current_loss);

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
