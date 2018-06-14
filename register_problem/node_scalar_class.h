#ifndef NODE_SCALAR_CLASS_H_INCLUDED
#define NODE_SCALAR_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class NODE_SCALAR
{
public:
	NODE_SCALAR()
	{
		scalar_pointer_manager = &scalar_pointer;
		scalar_pointer = &scalar_value;
		array_position = 0;
		scalar_value = 0.0;
	}

	virtual ~NODE_SCALAR()
	{
		// Nothing to deallocate
	}

	void assignScalarPointerManager(double ** src_scalar_pointer_manager = NULL, const unsigned int src_array_position = 0)
	{
		if (!src_scalar_pointer_manager)
		{
			scalar_pointer_manager = src_scalar_pointer_manager;
			array_position = 0;
		}
		else
		{
			scalar_pointer_manager = src_scalar_pointer_manager;
			array_position = src_array_position;
		}
	}

	virtual double getScalarValue()
	{
		return *(*scalar_pointer_manager + array_position);
	}
	
private:
	double ** scalar_pointer_manager;
	double * scalar_pointer;
	double scalar_value;
	unsigned int array_position;
};

#endif // NODE_SCALAR_CLASS_H_INCLUDED
