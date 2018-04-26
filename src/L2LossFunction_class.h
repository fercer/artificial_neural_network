#ifndef L2LOSSFUNCTION_CLASS_H_INCLUDED
#define L2LOSSFUNCTION_CLASS_H_INCLUDED

#include "LossFunction_class.h"

class L2LossFunction :
	public LossFunction
{
public:
	double computeLoss(const unsigned long long current_time);
};

#endif // L2LOSSFUNCTION_CLASS_H_INCLUDED