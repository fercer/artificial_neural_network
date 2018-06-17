#include "max_image_scalar_operation_class.h"

MAX_IMAGE_SCALAR_OPERATION::MAX_IMAGE_SCALAR_OPERATION()
{
}



MAX_IMAGE_SCALAR_OPERATION::MAX_IMAGE_SCALAR_OPERATION(const MAX_IMAGE_SCALAR_OPERATION & src_max_image_scalar_operation)
{
	copyFromImageScalarOperation(src_max_image_scalar_operation);
}



MAX_IMAGE_SCALAR_OPERATION MAX_IMAGE_SCALAR_OPERATION::operator=(const MAX_IMAGE_SCALAR_OPERATION & src_max_image_scalar_operation)
{
	if (this != &src_max_image_scalar_operation)
	{
		copyFromImageScalarOperation(src_max_image_scalar_operation);
	}

	return *this;
}



MAX_IMAGE_SCALAR_OPERATION::~MAX_IMAGE_SCALAR_OPERATION()
{
	// Nothing to deallocate
}



double MAX_IMAGE_SCALAR_OPERATION::performScalarOperation()
{
	double max = std::numeric_limits<double>::lowest();
	for (unsigned int xy = 0; xy < height * width; xy++)
	{
		if (max < *(src_img->image_data + xy))
		{
			max = *(src_img->image_data + xy);
		}
	}
	
	return max;
}
