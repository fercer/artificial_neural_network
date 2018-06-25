#include "height_image_scalar_operation_class.h"

HEIGHT_IMAGE_SCALAR_OPERATION::HEIGHT_IMAGE_SCALAR_OPERATION()
{
}



HEIGHT_IMAGE_SCALAR_OPERATION::HEIGHT_IMAGE_SCALAR_OPERATION(const HEIGHT_IMAGE_SCALAR_OPERATION & src_height_image_scalar_operation)
{
	copyFromImageScalarOperation(src_height_image_scalar_operation);
}



HEIGHT_IMAGE_SCALAR_OPERATION HEIGHT_IMAGE_SCALAR_OPERATION::operator=(const HEIGHT_IMAGE_SCALAR_OPERATION & src_height_image_scalar_operation)
{
	if (this != &src_height_image_scalar_operation)
	{
		copyFromImageScalarOperation(src_height_image_scalar_operation);
	}

	return *this;
}



HEIGHT_IMAGE_SCALAR_OPERATION::~HEIGHT_IMAGE_SCALAR_OPERATION()
{

}



double HEIGHT_IMAGE_SCALAR_OPERATION::performScalarOperation()
{
	return src_img->height;
}