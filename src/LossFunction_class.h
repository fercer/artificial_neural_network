#ifndef LOSSFUNCTION_CLASS_H_INCLUDED
#define LOSSFUNCTION_CLASS_H_INCLUDED

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include "Input_node_class.h"


class LossFunction
{
public:
	typedef enum LOSS_FUNCTION_TYPE {
		LF_L1_NORM = 0,
		LF_L2_NORM = 1,
		LF_CROSS_ENTROPY = 2
		// > Add other functions here <
	}LOSS_FUNCTION_TYPE;

	LossFunction()
	{
		error_derivative = 0.0;
		error = 0.0;
		error_current_time = 0;
		global_output_index = 0;
	}

	~LossFunction()
	{
	}
	
	// Computes the loss according to the network output and the groundtruth
	virtual double computeLoss(const unsigned long long current_time) = 0;

	/* Computes the loss according to the network output and the groundtruth,
	and computes the derivatives for each neuron
	*/
	virtual double computeLossWithDerivatives(const unsigned long long current_time) = 0;

	// Designates the pointer from where the groundtruth data will be accessed
	void setGroundtruth(int ** src_groundtruth_pointer)
	{
		groundtruth_pointer = src_groundtruth_pointer;
	}
	
	// Designates the position in the pointer from where the groundtruth data will be accessed
	void setGlobalOutputIndex(const unsigned int src_global_output_index)
	{
		global_output_index = src_global_output_index;
	}

	// Returns the position in the pointer from where the groundtruth data will be accessed
	unsigned int getGlobalOutputIndex()
	{
		return global_output_index;
	}

	// Designates the pointer from where the output values will be accessed
	void setOutputNode(Input_node * src_output_pointer)
	{
		network_output_pointer = src_output_pointer;
	}

	void backpropagateErrorDerivative()
	{
		network_output_pointer->addNodeErrorContribution(error_derivative, global_output_index);
	}

	double getErrorDerivative()
	{
		return error_derivative;
	}

	double getDifference()
	{
		return difference;
	}


	LOSS_FUNCTION_TYPE getLossFunctionType()
	{
		return my_loss_function_type;
	}


	void resetErrorCurrentTime()
	{
		error_current_time = 0;
	}

protected:
	int ** groundtruth_pointer;
	Input_node * network_output_pointer;
	double difference;
	double error;
	double error_derivative;
	unsigned int global_output_index;
	unsigned long long error_current_time;

	LOSS_FUNCTION_TYPE my_loss_function_type;
};

#endif //LOSSFUNCTION_CLASS_H_INCLUDED