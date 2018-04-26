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

	// Computes the error using the L1 loss function
	double computeLoss(const unsigned long long current_time);
};

#endif //CROSSENTROPYLOSSFUNCTION_CLASS_H_INCLUDED