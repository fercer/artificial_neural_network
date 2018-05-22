#ifndef L2LOSSFUNCTION_CLASS_H_INCLUDED
#define L2LOSSFUNCTION_CLASS_H_INCLUDED

#include "LossFunction_class.h"

class L2LossFunction :
	public LossFunction
{
public:

	L2LossFunction();
	L2LossFunction(const L2LossFunction & src_loss_function);
	L2LossFunction & operator=(const L2LossFunction & src_loss_function);
	~L2LossFunction();

	double computeLoss(const unsigned long long current_time);
	double computeLossWithDerivatives(const unsigned long long current_time);
	void dumpLossFunctionData(FILE * fp_network_data);
};

#endif // L2LOSSFUNCTION_CLASS_H_INCLUDED