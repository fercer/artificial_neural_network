#ifndef L1LOSSFUNCTION_CLASS_H_INCLUDED
#define L1LOSSFUNCTION_CLASS_H_INCLUDED

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <math.h>
#include "LossFunction_class.h"
class L1LossFunction :
	public LossFunction
{
public:
	L1LossFunction();
	L1LossFunction(const L1LossFunction & src_loss_function);
	L1LossFunction & operator= (const L1LossFunction & src_loss_function);
	~L1LossFunction();

	// Computes the error using the L1 loss function
	double computeLoss(const unsigned long long data_idx);
	double computeLossWithDerivatives(const unsigned long long data_idx);
	
	void dumpLossFunctionData(FILE * fp_network_data);
};

#endif //L1LOSSFUNCTION_CLASS_H_INCLUDED-