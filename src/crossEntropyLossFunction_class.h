#ifndef CROSSENTROPYLOSSFUNCTION_CLASS_H_INCLUDED
#define CROSSENTROPYLOSSFUNCTION_CLASS_H_INCLUDED

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <math.h>
#include "LossFunction_class.h"

class crossEntropyLossFunction :
	public LossFunction
{
public:
	crossEntropyLossFunction();
	crossEntropyLossFunction(const crossEntropyLossFunction & src_loss_function);
	crossEntropyLossFunction & operator=(const crossEntropyLossFunction & src_loss_function);
	~crossEntropyLossFunction();

	// Computes the error using the L1 loss function
	double computeLoss(const unsigned long long current_time);
	double computeLossWithDerivatives(const unsigned long long current_time);
};

#endif //CROSSENTROPYLOSSFUNCTION_CLASS_H_INCLUDED