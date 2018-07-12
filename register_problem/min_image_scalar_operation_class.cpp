#include "min_image_scalar_operation_class.h"

MIN_IMAGE_SCALAR_OPERATION::MIN_IMAGE_SCALAR_OPERATION()
{
}



MIN_IMAGE_SCALAR_OPERATION::MIN_IMAGE_SCALAR_OPERATION(const MIN_IMAGE_SCALAR_OPERATION & src_min_image_scalar_operation)
{
	copyFromImageScalarOperation(src_min_image_scalar_operation);
}



MIN_IMAGE_SCALAR_OPERATION MIN_IMAGE_SCALAR_OPERATION::operator=(const MIN_IMAGE_SCALAR_OPERATION & src_min_image_scalar_operation)
{
	if (this != &src_min_image_scalar_operation)
	{
		copyFromImageScalarOperation(src_min_image_scalar_operation);
	}

	return *this;
}



MIN_IMAGE_SCALAR_OPERATION::~MIN_IMAGE_SCALAR_OPERATION()
{

}



double MIN_IMAGE_SCALAR_OPERATION::performScalarOperation()
{
	double min = std::numeric_limits<double>::infinity();
	for (unsigned int xy = 0; xy < height * width; xy++)
	{
		if (min > *(src_img->image_data.double_image_data + xy))
		{
			min = *(src_img->image_data.double_image_data + xy);
		}
	}

	return min;
}