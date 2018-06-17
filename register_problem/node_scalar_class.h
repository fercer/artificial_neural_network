#ifndef NODE_SCALAR_CLASS_H_INCLUDED
#define NODE_SCALAR_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

template<class class_value_type>
class NODE_SCALAR
{
public:
	NODE_SCALAR()
	{
		scalar_pointer_manager = &scalar_pointer;
		scalar_pointer = &scalar_value;
		array_position = 0;
	}

	virtual ~NODE_SCALAR()
	{
		// Nothing to deallocate
	}

	void assignScalarPointerManager(class_value_type ** src_scalar_pointer_manager = NULL, const unsigned int src_array_position = 0)
	{
		if (src_scalar_pointer_manager)
		{
			scalar_pointer_manager = src_scalar_pointer_manager;
			array_position = src_array_position;
		}
		else
		{
			scalar_pointer_manager = &scalar_pointer;
			array_position = 0;
		}
	}
	
	virtual class_value_type getScalarValue()
	{
		return *(*scalar_pointer_manager + array_position);
	}
	
private:
	class_value_type ** scalar_pointer_manager;
	class_value_type * scalar_pointer;
	class_value_type scalar_value;
	unsigned int array_position;
};



template<>
class NODE_SCALAR<char*>
{
public:
	NODE_SCALAR()
	{
		scalar_pointer_manager = &scalar_pointer;
		scalar_pointer = &scalar_value;
		scalar_value = (char*)malloc(512 * sizeof(char));
		array_position = 0;
	}

	virtual ~NODE_SCALAR()
	{
		free(scalar_pointer);
	}

	void assignScalarPointerManager(char*** src_scalar_pointer_manager = NULL, const unsigned int src_array_position = 0)
	{
		if (src_scalar_pointer_manager)
		{
			scalar_pointer_manager = src_scalar_pointer_manager;
			array_position = src_array_position;
		}
		else
		{
			scalar_pointer_manager = &scalar_pointer;
			array_position = 0;
		}
	}

	virtual char * getScalarValue()
	{
		return *(*scalar_pointer_manager + array_position);
	}

private:
	char*** scalar_pointer_manager;
	char** scalar_pointer;
	char* scalar_value;
	unsigned int array_position;
};

#endif // NODE_SCALAR_CLASS_H_INCLUDED
