#ifndef GRADIENTMETHODS_CLASS_H_INCLUDED
#define GRADIENTMETHODS_CLASS_H_INCLUDED

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include <string.h>

#include <math.h>

#include <omp.h>

#include "random_numbers_generator.h"

class gradientMethods
{
public:
	typedef enum GRADIENT_METHOD {
		GRADIENT_DESCENT = 0,
		LEVENBERG_MARQUARDT = 1,
		MINI_BACTH_GRADIENT_DESCENT = 2,
		STOCHASTIC_GRADIENT_DESCENT = 3,
		MINI_BATCH_STOCHASTIC_GRADIENT_DESCENT = 4
	} GRADIENT_METHOD;
	
	gradientMethods();
	~gradientMethods();
	
	void setGradientMethod(const GRADIENT_METHOD src_gradient_method);

	void setVariablesValuesPointerManager(double * src_variables_values_pointer_manager);
	void setVariablesDerivativesPointersManager(double ** src_variables_derivatives_pointer_manager);
	void setOutputsDifferencePointerManager(double * src_outputs_difference_pointer_manager);

	void setTrainingDataSize(const unsigned int src_training_data_size);
	void setOutputsCount(const unsigned int src_outputs_count);
	void setVariablesCount(const unsigned int src_variables_count);

	void setMaxMuValue(const double src_max_mu_value);
	void setMuIncreasingFactor(const double src_mu_increasing_factor);
	void setMuDecreasingFactor(const double src_mu_decreasing_factor);
	void setMaxWorseningCount(const double src_max_worsening_count);

	void setMomentum(const double src_momentum);
	void setLearningRate(const double src_learning_rate);

	void setBatchSize(const unsigned int src_batch_size);

	void updateDeltasValues();
	double updateVariablesValues();
	bool confirmDescent(const double src_error_difference);

private:
	bool memory_already_allocated;
	bool hessian_matrix_was_required;
	
	unsigned int variables_count;
	unsigned int outputs_count;
	unsigned int training_data_size;

	double * variables_values_pointer_manager;
	double ** variables_derivatives_pointers_manager;
	double * outputs_difference_pointer_manager;
	double * deltas_values;
	
	double * jacobian_error_derivative_product;
	double * hessian_matrix;
	double * previous_variables_values;
	double * previous_jacobian_error_derivative_product;
	double * previous_hessian_matrix;

	double momentum;
	double learning_rate;
		
	double mu_value;
	double max_mu_value;
	double mu_increasing_factor;
	double mu_decreasing_factor;
	unsigned int worsening_count;
	unsigned int max_worsening_count;

	unsigned int batch_count;
	unsigned int batch_size;
	unsigned int global_count;

	int allocateMethodMemory();

	double updateVariablesValuesBase();
	void updateDeltasValuesBase(const double src_factor_value);


	void (gradientMethods::*update_deltas_values_method_iteration) ();
	void allocateMemoryAndUpdateDeltasValues();

	double (gradientMethods::*update_variables_values_method) ();
	void (gradientMethods::*update_deltas_values_method) ();
	bool (gradientMethods::*confirm_descent_method)(const double);

	void update_deltas_values_gradient_descent();
	double update_variables_values_gradient_descent();
	bool confirm_descent_gradient_descent(const double src_error_difference);

	void update_deltas_values_levenberg_marquardt();
	double update_variables_values_levenberg_marquardt();
	bool confirm_descent_levenberg_marquardt(const double src_error_difference);

	void update_mini_batch_gradient_descent();
};

#endif //BACKPROPAGATIONBASEDANN_CLASS_H_INCLUDED