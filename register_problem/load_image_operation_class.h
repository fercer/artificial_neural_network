#ifndef LOAD_IMAGE_OPERATION_CLASS_H_INCLUDED
#define LOAD_IMAGE_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_operation_class.h"

class LOAD_IMAGE_OPERATION :
	public IMAGE_OPERATION
{
public:
	LOAD_IMAGE_OPERATION();
	LOAD_IMAGE_OPERATION(const LOAD_IMAGE_OPERATION & src_load_image_operation);
	LOAD_IMAGE_OPERATION operator = (const LOAD_IMAGE_OPERATION & src_load_image_operation);
	~LOAD_IMAGE_OPERATION();

protected:
	void performOperation();

private:
	void loadPGM_ascii(const char * src_filename);
	void loadPGM_raw(const char * src_filename);
};

#endif // LOAD_IMAGE_OPERATION_CLASS_H_INCLUDED
