#include "width_image_scalar_operation_class.h"

WIDTH_IMAGE_SCALAR_OPERATION::WIDTH_IMAGE_SCALAR_OPERATION()
{
}



WIDTH_IMAGE_SCALAR_OPERATION::WIDTH_IMAGE_SCALAR_OPERATION(const WIDTH_IMAGE_SCALAR_OPERATION & src_width_image_scalar_operation)
{
	copyFromImageScalarOperation(src_width_image_scalar_operation);
}



WIDTH_IMAGE_SCALAR_OPERATION WIDTH_IMAGE_SCALAR_OPERATION::operator=(const WIDTH_IMAGE_SCALAR_OPERATION & src_width_image_scalar_operation)
{
	if (this != &src_width_image_scalar_operation)
	{
		copyFromImageScalarOperation(src_width_image_scalar_operation);
	}

	return *this;
}



WIDTH_IMAGE_SCALAR_OPERATION::~WIDTH_IMAGE_SCALAR_OPERATION()
{
	// Nothing to deallocate
}



double WIDTH_IMAGE_SCALAR_OPERATION::performScalarOperation()
{
	return src_img->width;
}