#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "../src/random_numbers_generator.h"
#include "gradientMethods_class.h"

gradientMethods::gradientMethods()
{
	update_deltas_values_method_iteration = &gradientMethods::allocateMemoryAndUpdateDeltasValues;

	update_deltas_values_method = &gradientMethods::update_deltas_values_gradient_descent;
	update_variables_values_method = &gradientMethods::update_variables_values_gradient_descent;
	confirm_descent_method = &gradientMethods::confirm_descent_gradient_descent;

	deltas_values = NULL;
	previous_deltas_values = NULL;
	variables_values_pointer_manager = NULL;
	variables_derivatives_pointers_manager = NULL;
	
	memory_already_allocated = false;
	hessian_matrix_was_required = false;

	previous_variables_values = NULL;
	jacobian_error_derivative_product = NULL;
	hessian_matrix = NULL;
	previous_jacobian_error_derivative_product = NULL;
	previous_hessian_matrix = NULL;
	
	momentum = 0.9;
	learning_rate = 0.05;

	mu_value = 0.001;
	max_mu_value = 1e10;
	mu_increasing_factor = 10.0;
	mu_decreasing_factor = 0.1;
	worsening_count = 0;
	max_worsening_count = 5;
		
	batch_count = 0;
	mini_batch_selected_index = 0;
	batch_size = 0;

	variables_count = 0;
	outputs_count = 0;
	training_data_size = 0;

	random_number_generator_seed = initSeed(0);
}

gradientMethods::gradientMethods(const gradientMethods & src_gradient_method)
{
	this->update_deltas_values_method_iteration = src_gradient_method.update_deltas_values_method_iteration;
	this->update_deltas_values_method = src_gradient_method.update_deltas_values_method;
	this->update_variables_values_method = src_gradient_method.update_variables_values_method;
	this->confirm_descent_method = src_gradient_method.confirm_descent_method;

	this->momentum = src_gradient_method.momentum;
	this->learning_rate = src_gradient_method.learning_rate;

	this->mu_value = src_gradient_method.mu_value;
	this->max_mu_value = src_gradient_method.max_mu_value;
	this->mu_increasing_factor = src_gradient_method.mu_increasing_factor;
	this->mu_decreasing_factor = src_gradient_method.mu_decreasing_factor;
	this->worsening_count = src_gradient_method.worsening_count;
	this->max_worsening_count = src_gradient_method.max_worsening_count;

	this->batch_count = src_gradient_method.batch_count;
	this->mini_batch_selected_index = src_gradient_method.mini_batch_selected_index;
	this->batch_size = src_gradient_method.batch_count;

	this->variables_count = src_gradient_method.variables_count;
	this->outputs_count = src_gradient_method.outputs_count;
	this->training_data_size = src_gradient_method.training_data_size;

	this->deltas_values = NULL;
	this->previous_deltas_values = NULL;
	this->variables_values_pointer_manager = NULL;
	this->variables_derivatives_pointers_manager = NULL;

	this->memory_already_allocated = false;
	this->hessian_matrix_was_required = src_gradient_method.hessian_matrix_was_required;

	this->previous_variables_values = NULL;
	this->jacobian_error_derivative_product = NULL;
	this->hessian_matrix = NULL;
	this->previous_jacobian_error_derivative_product = NULL;
	this->previous_hessian_matrix = NULL;

	this->random_number_generator_seed = (STAUS*)malloc(sizeof(STAUS));
	this->random_number_generator_seed->lcg_seed = src_gradient_method.random_number_generator_seed->lcg_seed;
	this->random_number_generator_seed->z1 = src_gradient_method.random_number_generator_seed->z1;
	this->random_number_generator_seed->z2 = src_gradient_method.random_number_generator_seed->z2;
	this->random_number_generator_seed->z3 = src_gradient_method.random_number_generator_seed->z3;

	this->allocateMethodMemory();

	if (this->hessian_matrix_was_required)
	{
		memcpy(this->previous_variables_values,
			src_gradient_method.previous_variables_values,
			variables_count * sizeof(double));

		memcpy(this->jacobian_error_derivative_product,
			src_gradient_method.jacobian_error_derivative_product,
			variables_count * sizeof(double));

		memcpy(this->previous_jacobian_error_derivative_product,
			src_gradient_method.previous_jacobian_error_derivative_product,
			variables_count * sizeof(double));

		memcpy(this->hessian_matrix,
			src_gradient_method.hessian_matrix,
			variables_count * (variables_count + 1) / 2 * sizeof(double));

		memcpy(this->previous_hessian_matrix,
			src_gradient_method.previous_hessian_matrix,
			variables_count * (variables_count + 1) / 2 * sizeof(double));
	}
}



gradientMethods gradientMethods::operator=(const gradientMethods & src_gradient_method)
{
	if (this != &src_gradient_method)
	{
		this->update_deltas_values_method_iteration = src_gradient_method.update_deltas_values_method_iteration;
		this->update_deltas_values_method = src_gradient_method.update_deltas_values_method;
		this->update_variables_values_method = src_gradient_method.update_variables_values_method;
		this->confirm_descent_method = src_gradient_method.confirm_descent_method;

		this->momentum = src_gradient_method.momentum;
		this->learning_rate = src_gradient_method.learning_rate;

		this->mu_value = src_gradient_method.mu_value;
		this->max_mu_value = src_gradient_method.max_mu_value;
		this->mu_increasing_factor = src_gradient_method.mu_increasing_factor;
		this->mu_decreasing_factor = src_gradient_method.mu_decreasing_factor;
		this->worsening_count = src_gradient_method.worsening_count;
		this->max_worsening_count = src_gradient_method.max_worsening_count;

		this->batch_count = src_gradient_method.batch_count;
		this->mini_batch_selected_index = src_gradient_method.mini_batch_selected_index;
		this->batch_size = src_gradient_method.batch_count;

		this->variables_count = src_gradient_method.variables_count;
		this->outputs_count = src_gradient_method.outputs_count;
		this->training_data_size = src_gradient_method.training_data_size;

		this->deltas_values = NULL;
		this->previous_deltas_values = NULL;
		this->variables_values_pointer_manager = NULL;
		this->variables_derivatives_pointers_manager = NULL;

		this->memory_already_allocated = false;
		this->hessian_matrix_was_required = src_gradient_method.hessian_matrix_was_required;

		this->previous_variables_values = NULL;
		this->jacobian_error_derivative_product = NULL;
		this->hessian_matrix = NULL;
		this->previous_jacobian_error_derivative_product = NULL;
		this->previous_hessian_matrix = NULL;

		this->random_number_generator_seed = (STAUS*) malloc(sizeof(STAUS));
		this->random_number_generator_seed->lcg_seed = src_gradient_method.random_number_generator_seed->lcg_seed;
		this->random_number_generator_seed->z1 = src_gradient_method.random_number_generator_seed->z1;
		this->random_number_generator_seed->z2 = src_gradient_method.random_number_generator_seed->z2;
		this->random_number_generator_seed->z3 = src_gradient_method.random_number_generator_seed->z3;

		this->allocateMethodMemory();

		if (this->hessian_matrix_was_required)
		{
			memcpy(this->previous_variables_values,
				src_gradient_method.previous_variables_values,
				variables_count * sizeof(double));

			memcpy(this->jacobian_error_derivative_product,
				src_gradient_method.jacobian_error_derivative_product,
				variables_count * sizeof(double));

			memcpy(this->previous_jacobian_error_derivative_product,
				src_gradient_method.previous_jacobian_error_derivative_product,
				variables_count * sizeof(double));

			memcpy(this->hessian_matrix,
				src_gradient_method.hessian_matrix,
				variables_count * (variables_count + 1) / 2 * sizeof(double));

			memcpy(this->previous_hessian_matrix,
				src_gradient_method.previous_hessian_matrix,
				variables_count * (variables_count + 1) / 2 * sizeof(double));
		}
	}
	return *this;
}



gradientMethods::~gradientMethods()
{
	if (memory_already_allocated)
	{
		if (hessian_matrix_was_required)
		{
			free(previous_variables_values);
			free(jacobian_error_derivative_product);
			free(hessian_matrix);
			free(previous_jacobian_error_derivative_product);
			free(previous_hessian_matrix);
		}
		else
		{
			free(deltas_values);
			free(previous_deltas_values);
		}
	}

	free(random_number_generator_seed);
}



int gradientMethods::allocateMethodMemory()
{
	/* Successfull allocation of memory returns 1, otherwise, returns 0 */
	if (memory_already_allocated)
	{
		return 1;
	}

	if (variables_count == 0 || outputs_count == 0)
	{
		return 0;
	}
	

	if (hessian_matrix_was_required)
	{
		// Allocate memory for the weights update directions:
		hessian_matrix = (double*)calloc(variables_count * (variables_count + 1) / 2, sizeof(double));
		previous_hessian_matrix = (double*)malloc(variables_count * (variables_count + 1) / 2 * sizeof(double));

		jacobian_error_derivative_product = (double*)calloc(variables_count, sizeof(double));
		previous_jacobian_error_derivative_product = (double*)malloc(variables_count * sizeof(double));
		previous_variables_values = (double*)malloc(variables_count * sizeof(double));
	}
	else
	{
		// Allocate memory for the weights update directions:
		deltas_values = (double*)calloc(variables_count, sizeof(double));
		previous_deltas_values = (double*)calloc(variables_count, sizeof(double));
	}

	memory_already_allocated = true;
	return 1;
}



void gradientMethods::updateVariablesValuesBase()
{
	for (unsigned int variable_index = 0; variable_index < variables_count; variable_index++)
	{
		const double current_variable_delta = *(previous_deltas_values + variable_index) * momentum + 
			*(deltas_values + variable_index) * learning_rate;

		*(variables_values_pointer_manager + variable_index) = *(variables_values_pointer_manager + variable_index) - current_variable_delta;
		squared_gradient_norm += current_variable_delta * current_variable_delta;
		*(previous_deltas_values + variable_index) = current_variable_delta;
		*(deltas_values + variable_index) = 0.0;
	}
}



void gradientMethods::updateDeltasValuesBase(const double src_factor_value)
{
	for (unsigned int variable_index = 0; variable_index < variables_count; variable_index++)
	{
		double error_contribution = 0.0;
		for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
		{
			error_contribution += *(outputs_derivatives_pointer_manager + output_index) *
				*(*(variables_derivatives_pointers_manager + variable_index) + output_index);
		}
		*(deltas_values + variable_index) = *(deltas_values + variable_index) + error_contribution * src_factor_value;
	}
}



void gradientMethods::update_deltas_values_gradient_descent()
{
	updateDeltasValuesBase(1.0 / (double)training_data_size);
}


double gradientMethods::update_variables_values_gradient_descent()
{
	squared_gradient_norm = 0.0;
	updateVariablesValuesBase();
	return squared_gradient_norm;
}


bool gradientMethods::confirm_descent_gradient_descent(const double src_error_difference)
{
	return true;
}



void gradientMethods::update_deltas_values_levenberg_marquardt()
{
	for (unsigned int variable_index_i = 0; variable_index_i < variables_count; variable_index_i++)
	{
		// Compute the product between the Jacobian matrix and the vector of errors:
		double error_contribution_product = 0.0;
		for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
		{
			*(jacobian_error_derivative_product + variable_index_i) =
				*(jacobian_error_derivative_product + variable_index_i) +
				*(*(variables_derivatives_pointers_manager + variable_index_i) + output_index) *
				*(outputs_derivatives_pointer_manager + output_index);

			/* Compute the hessian matrix entry that corresponds to
			the product of the current weight and the first weight of the network:
			*/
			error_contribution_product +=
				*(*(variables_derivatives_pointers_manager + variable_index_i) + output_index) *
				*(*variables_derivatives_pointers_manager + output_index);
		}

		const unsigned int variable_index_base_i = variable_index_i * (variable_index_i + 1) / 2;
		*(hessian_matrix + variable_index_base_i) = *(hessian_matrix + variable_index_base_i) + error_contribution_product;

		// Compute the jacobian matrix product with its self transpose:
		for (unsigned int variable_index_j = 1; variable_index_j <= variable_index_i; variable_index_j++)
		{
			double error_contribution_product = 0.0;
			for (unsigned int output_index = 0; output_index < outputs_count; output_index++)
			{
				error_contribution_product += *(*(variables_derivatives_pointers_manager + variable_index_i) + output_index) *
					*(*(variables_derivatives_pointers_manager + variable_index_j) + output_index);
			}

			*(hessian_matrix + variable_index_base_i + variable_index_j) = *(hessian_matrix + variable_index_base_i + variable_index_j) + error_contribution_product;
		}
	}
}



double gradientMethods::update_variables_values_levenberg_marquardt()
{
	// Save the current computed hessian matrix in case that it is singular:
	memcpy(previous_hessian_matrix, hessian_matrix, variables_count * (variables_count + 1) / 2 * sizeof(double));
	memcpy(previous_jacobian_error_derivative_product, jacobian_error_derivative_product, variables_count * sizeof(double));
	memcpy(previous_variables_values, variables_values_pointer_manager, variables_count * sizeof(double));

	do
	{
		// Perform the Cholesky's factorization to obtain L * L^T = H + mu * I
		// Solve L * z = J^T*e at the same time:
		unsigned int variable_index_i;
		for (variable_index_i = 0; variable_index_i < variables_count; variable_index_i++)
		{
			double jacobian_factorized_hessian_product = 0.0;
			const unsigned int variable_index_base_i = variable_index_i * (variable_index_i + 1) / 2;
			double row_squared_sum = 0.0;

			for (unsigned int variable_index_j = 0; variable_index_j < variable_index_i; variable_index_j++)
			{
				const unsigned int variable_index_base_j = variable_index_j * (variable_index_j + 1) / 2;
				double row_product_sum = 0.0;
				for (unsigned int variable_index_k = 0; variable_index_k < variable_index_j; variable_index_k++)
				{
					row_product_sum +=
						*(hessian_matrix + variable_index_base_j + variable_index_k) *
						*(hessian_matrix + variable_index_base_i + variable_index_k);
				}

				*(hessian_matrix + variable_index_base_i + variable_index_j) =
					(*(hessian_matrix + variable_index_base_i + variable_index_j) - row_product_sum) /
					*(hessian_matrix + variable_index_base_j + variable_index_j);

				// Compue the squared sum of the values of the i-th row of the hessian matrix:
				row_squared_sum += *(hessian_matrix + variable_index_base_i + variable_index_j) *
					*(hessian_matrix + variable_index_base_i + variable_index_j);

				/* Perform the product between the factorized hessian and the jacobian error,
				to compute the i-th entry of vector 'z':
				*/
				jacobian_factorized_hessian_product += *(hessian_matrix + variable_index_base_i + variable_index_j) * *(jacobian_error_derivative_product + variable_index_j);
			}

			/* Check if the difference is greater than 0.0,
			otherwise, the hessian matrix is singular:
			*/
			const double difference = *(hessian_matrix + variable_index_base_i + variable_index_i) + mu_value - row_squared_sum;
			if (difference < 1e-12) {
				// If the hessian matrix is not invertable, increase the value of the diagonal:
				mu_value *= mu_increasing_factor;
				// Copy only the values that were modified:
				memcpy(hessian_matrix, previous_hessian_matrix, (variable_index_base_i + variable_index_i) * sizeof(double));
				memcpy(jacobian_error_derivative_product, previous_jacobian_error_derivative_product, (variable_index_i) * sizeof(double));
				break;
			}

			*(hessian_matrix + variable_index_base_i + variable_index_i) = sqrt(difference);

			// Compute the i-th entry of the temporal 'z' vector
			*(jacobian_error_derivative_product + variable_index_i) = (*(jacobian_error_derivative_product + variable_index_i) - jacobian_factorized_hessian_product) / *(hessian_matrix + variable_index_base_i + variable_index_i);
		}

		if (variable_index_i == variables_count)
		{
			break;
		}
	} while (1);

	// Solve L^T * w = z to determine the new weights delta values:
	squared_gradient_norm = 0.0;
	for (int variable_index_i = (variables_count - 1); variable_index_i >= 0; variable_index_i--)
	{
		double jacobian_factorized_hessian_product = 0.0;
		const unsigned int variable_index_base_i = variable_index_i * (variable_index_i + 1) / 2;
		for (int variable_index_j = (variables_count - 1); variable_index_j > variable_index_i; variable_index_j--)
		{
			const unsigned int weight_index_base_j = variable_index_j * (variable_index_j + 1) / 2;
			jacobian_factorized_hessian_product += *(jacobian_error_derivative_product + variable_index_j) * *(hessian_matrix + weight_index_base_j + variable_index_i);
		}

		*(jacobian_error_derivative_product + variable_index_i) = (*(jacobian_error_derivative_product + variable_index_i) - jacobian_factorized_hessian_product) / *(hessian_matrix + variable_index_base_i + variable_index_i);

		// Update the weights:
		*(variables_values_pointer_manager + variable_index_i) = 
			*(variables_values_pointer_manager + variable_index_i) - *(jacobian_error_derivative_product + variable_index_i);
		squared_gradient_norm += *(jacobian_error_derivative_product + variable_index_i) *
			*(jacobian_error_derivative_product + variable_index_i);
	}

	return squared_gradient_norm;
}



bool gradientMethods::confirm_descent_levenberg_marquardt(const double src_error_difference)
{
	if (src_error_difference > 0.0)
	{
		mu_value *= mu_decreasing_factor;
		memset(jacobian_error_derivative_product, 0, variables_count * sizeof(double));
		memset(hessian_matrix, 0, variables_count * (variables_count + 1) / 2 * sizeof(double));
		return true;
	}

	worsening_count++;
	if (worsening_count > max_worsening_count || mu_value > max_mu_value)
	{
		/*  If the error has not decreased after 5 cycles,
		the current weights are passed to the following epoch to recompute the hessian matrix
		*/
		worsening_count = 0;
		memset(jacobian_error_derivative_product, 0, variables_count * sizeof(double));
		memset(hessian_matrix, 0, variables_count * (variables_count + 1) / 2 * sizeof(double));
		return true;;
	}	
	
	/* If the error has not decreased,
	the diagonal of the hessian matrix is modified in order to change the descent direction
	*/
	memcpy(variables_values_pointer_manager, previous_variables_values, variables_count * sizeof(double));
	memcpy(hessian_matrix, previous_hessian_matrix, variables_count * (variables_count + 1) / 2 * sizeof(double));
	memcpy(jacobian_error_derivative_product, previous_jacobian_error_derivative_product,
		variables_count * sizeof(double));

	mu_value *= mu_increasing_factor;
	
	update_variables_values_levenberg_marquardt();

	return false;
}




void gradientMethods::update_deltas_values_mini_batch_gradient_descent()
{
	updateDeltasValuesBase(1.0/(double)batch_size);
	batch_count++;
	if ((batch_count % batch_size) == 0 || (batch_count >= training_data_size))
	{
		squared_gradient_norm = 0.0;
		updateVariablesValuesBase();
	}
}


double gradientMethods::update_variables_values_mini_batch_gradient_descent()
{
	return squared_gradient_norm;
}


bool gradientMethods::confirm_descent_mini_batch_gradient_descent(const double src_error_difference)
{
	batch_count = 0;
	return true;
}




void gradientMethods::update_deltas_values_stochastic_gradient_descent()
{
	updateDeltasValuesBase(1.0);
	squared_gradient_norm = 0.0;
	updateVariablesValuesBase();
}


double gradientMethods::update_variables_values_stochastic_gradient_descent()
{
	return squared_gradient_norm;
}


bool gradientMethods::confirm_descent_stochastic_gradient_descent(const double src_error_difference)
{
	return true;
}


void gradientMethods::update_deltas_values_mini_batch_stochastic_gradient_descent()
{
	batch_count++;	
	if ((batch_count % batch_size) == 0)
	{
		const unsigned int selected_index = (unsigned int)floor(HybTaus(0.0,
			(((batch_count + batch_size) >= training_data_size) ? (training_data_size - batch_count) : batch_size) - 1e-12, random_number_generator_seed));

		mini_batch_selected_index = batch_count + selected_index;
	}

	if (batch_count == mini_batch_selected_index)
	{
		updateDeltasValuesBase(1.0);
		squared_gradient_norm = 0.0;
		updateVariablesValuesBase();
	}
}


double gradientMethods::update_variables_values_mini_batch_stochastic_gradient_descent()
{
	return squared_gradient_norm;
}


bool gradientMethods::confirm_descent_mini_batch_stochastic_gradient_descent(const double src_error_difference)
{
	batch_count = 0;
	mini_batch_selected_index = (unsigned int)floor(HybTaus(0.0, (double)batch_size - 1e-12, random_number_generator_seed));
	return true;
}



void gradientMethods::setGradientMethod(const GRADIENT_METHOD src_gradient_method)
{
	switch (src_gradient_method)
	{
		case GRADIENT_DESCENT:
			update_deltas_values_method = &gradientMethods::update_deltas_values_gradient_descent;
			update_variables_values_method = &gradientMethods::update_variables_values_gradient_descent;
			confirm_descent_method = &gradientMethods::confirm_descent_gradient_descent;
			break;

		case LEVENBERG_MARQUARDT:
			update_deltas_values_method = &gradientMethods::update_deltas_values_levenberg_marquardt;
			update_variables_values_method = &gradientMethods::update_variables_values_levenberg_marquardt;
			confirm_descent_method = &gradientMethods::confirm_descent_levenberg_marquardt;
			hessian_matrix_was_required = true;
			break;

		case MINI_BATCH_GRADIENT_DESCENT:
			update_deltas_values_method = &gradientMethods::update_deltas_values_mini_batch_gradient_descent;
			update_variables_values_method = 
				&gradientMethods::update_variables_values_mini_batch_gradient_descent;
			confirm_descent_method = &gradientMethods::confirm_descent_mini_batch_gradient_descent;
			break;

		case STOCHASTIC_GRADIENT_DESCENT:
			update_deltas_values_method = &gradientMethods::update_deltas_values_stochastic_gradient_descent;
			update_variables_values_method = 
				&gradientMethods::update_variables_values_stochastic_gradient_descent;
			confirm_descent_method = &gradientMethods::confirm_descent_stochastic_gradient_descent;
			break;

		case MINI_BATCH_STOCHASTIC_GRADIENT_DESCENT:
			update_deltas_values_method = &gradientMethods::update_deltas_values_mini_batch_stochastic_gradient_descent;
			update_variables_values_method =
				&gradientMethods::update_variables_values_mini_batch_stochastic_gradient_descent;
			confirm_descent_method = &gradientMethods::confirm_descent_mini_batch_stochastic_gradient_descent;

			mini_batch_selected_index = (unsigned int)floor(HybTaus(0.0, (double)batch_size - 1e-12, random_number_generator_seed));

			break;
	}
}


void gradientMethods::setVariablesValuesPointerManager(double * src_variables_values_pointer_manager)
{
	variables_values_pointer_manager = src_variables_values_pointer_manager;
}


void gradientMethods::setVariablesDerivativesPointersManager(double ** src_variables_derivatives_pointer_manager)
{
	variables_derivatives_pointers_manager = src_variables_derivatives_pointer_manager;
}



void gradientMethods::setOutputsDerivativesPointerManager(double * src_outputs_derivatives_pointer_manager)
{
	outputs_derivatives_pointer_manager = src_outputs_derivatives_pointer_manager;
}


void gradientMethods::setTrainingDataSize(const unsigned int src_training_data_size)
{
	training_data_size = src_training_data_size;
	batch_size = (unsigned int)ceil((double)training_data_size / 10.0);
}


void gradientMethods::setOutputsCount(const unsigned int src_outputs_count)
{
	outputs_count = src_outputs_count;
}


void gradientMethods::setVariablesCount(const unsigned int src_variables_count)
{
	variables_count = src_variables_count;
}


void gradientMethods::setMaxMuValue(const double src_max_mu_value)
{
	max_mu_value = src_max_mu_value;
}


void gradientMethods::setMuIncreasingFactor(const double src_mu_increasing_factor)
{
	mu_increasing_factor = src_mu_increasing_factor;
}


void gradientMethods::setMuDecreasingFactor(const double src_mu_decreasing_factor)
{
	mu_decreasing_factor = src_mu_decreasing_factor;
}


void gradientMethods::setMaxWorseningCount(const unsigned int src_max_worsening_count)
{
	max_worsening_count = src_max_worsening_count;
}


void gradientMethods::setMomentum(const double src_momentum)
{
	momentum = src_momentum;
}


void gradientMethods::setLearningRate(const double src_learning_rate)
{
	learning_rate = src_learning_rate;
}


void gradientMethods::setBatchSize(const unsigned int src_batch_size)
{
	batch_size = src_batch_size;
}


void gradientMethods::allocateMemoryAndUpdateDeltasValues()
{
	// This method is run in the first iteration, to allocate the memory
	allocateMethodMemory();

	// Then it is de-referenciated to continue the computation normally with the selected method:
	update_deltas_values_method_iteration = update_deltas_values_method;
	updateDeltasValues();
}



void gradientMethods::updateDeltasValues()
{
	(this->*update_deltas_values_method_iteration)();
}


double gradientMethods::updateVariablesValues()
{
	return (this->*update_variables_values_method)();
}


bool gradientMethods::confirmDescent(const double src_error_difference)
{
	return (this->*confirm_descent_method)(src_error_difference);
}