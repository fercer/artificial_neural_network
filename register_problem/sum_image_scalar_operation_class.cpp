#include "sum_image_scalar_operation_class.h"

SUM_IMAGE_SCALAR_OPERATION::SUM_IMAGE_SCALAR_OPERATION()
{
}



SUM_IMAGE_SCALAR_OPERATION::SUM_IMAGE_SCALAR_OPERATION(const SUM_IMAGE_SCALAR_OPERATION & src_sum_image_scalar_operation)
{
	copyFromImageScalarOperation(src_sum_image_scalar_operation);
}



SUM_IMAGE_SCALAR_OPERATION SUM_IMAGE_SCALAR_OPERATION::operator=(const SUM_IMAGE_SCALAR_OPERATION & src_sum_image_scalar_operation)
{
	if (this != &src_sum_image_scalar_operation)
	{
		copyFromImageScalarOperation(src_sum_image_scalar_operation);
	}

	return *this;
}



SUM_IMAGE_SCALAR_OPERATION::~SUM_IMAGE_SCALAR_OPERATION()
{
	// Nothing to deallocate
}



double SUM_IMAGE_SCALAR_OPERATION::performScalarOperation()
{
	double sum = 0.0;
	for (unsigned int xy = 0; xy < height * width; xy++)
	{
		sum += *(src_img->image_data + xy);
	}
	return sum;
}
