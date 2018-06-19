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

	void setFilename(const char * src_filename);
	void setFilename(NODE_SCALAR<char *> * src_node_filename);

protected:
	void performOperation();

private:
	NODE_SCALAR<char*> default_filename;
};

#endif // LOAD_IMAGE_OPERATION_CLASS_H_INCLUDED
