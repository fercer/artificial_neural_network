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
	LossFunction()
	{
		error_derivative = 0.0;
		error = 0.0;
		error_current_time = 0;
	}

	~LossFunction()
	{
	}
	
	// Computes the loss according to the network output and the groundtruth
	virtual double computeLoss(const unsigned long long current_time) = 0;

	// Designates the pointer from where the groundtruth data will be accessed
	void setGroundtruth(int * src_groundtruth_pointer)
	{
		groundtruth_pointer = src_groundtruth_pointer;
	}

	// Designates the pointer from where the output values will be accessed
	void setOutputNode(Input_node * src_output_pointer)
	{
		network_output_pointer = src_output_pointer;
	}

	void backpropagateErrorDerivative(const unsigned int src_output_index)
	{
		network_output_pointer->addNodeErrorContribution(error_derivative, src_output_index);
	}

	double getErrorDerivative()
	{
		return error_derivative;
	}

	double getDifference()
	{
		return difference;
	}

	void resetErrorCurrentTime()
	{
		error_current_time = 0;
	}

protected:
	int * groundtruth_pointer;
	Input_node * network_output_pointer;
	double difference;
	double error;
	double error_derivative;
	unsigned long long error_current_time;
};

#endif //LOSSFUNCTION_CLASS_H_INCLUDED