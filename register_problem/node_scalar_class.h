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


	NODE_SCALAR(const class_value_type src_value)
	{
		scalar_pointer_manager = &scalar_pointer;
		scalar_pointer = &scalar_value;
		array_position = 0;

		scalar_value = src_value;
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

	virtual void setScalarValue(const class_value_type src_scalar_value)
	{
		*(*scalar_pointer_manager + array_position) = src_scalar_value;
	}

	void setNodeScalarName(const char * src_name)
	{
		strcpy(node_scalar_name, src_name);
	}

protected:
	void copyFromNodeScalar(const NODE_SCALAR<class_value_type>& src_node_scalar)
	{
		if (src_node_scalar.scalar_pointer_manager != &src_node_scalar.scalar_pointer)
		{
			this->scalar_pointer_manager = src_node_scalar.scalar_pointer_manager;
			this->array_position = src_node_scalar.array_position;
		}
		else
		{
			this->scalar_value = src_node_scalar.scalar_value;
			this->array_position = 0;
		}

		sprintf(node_scalar_name, "%s-copy", src_node_scalar.node_scalar_name);
	}

private:
	class_value_type ** scalar_pointer_manager;
	class_value_type * scalar_pointer;
	class_value_type scalar_value;
	unsigned int array_position;

	char node_scalar_name[64];
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

	NODE_SCALAR(const char * src_string)
	{
		scalar_pointer_manager = &scalar_pointer;
		scalar_pointer = &scalar_value;
		scalar_value = (char*)malloc(512 * sizeof(char));
		array_position = 0;

		strcpy(scalar_value, src_string);
	}

	virtual ~NODE_SCALAR()
	{
		free(scalar_value);
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

	virtual void setScalarValue(const char * src_scalar_value)
	{
		strcpy(*(*scalar_pointer_manager + array_position), src_scalar_value);
	}

	void setNodeScalarName(const char * src_name)
	{
		strcpy(node_scalar_name, src_name);
	}

protected:
	void copyFromNodeScalar(const NODE_SCALAR<char*>& src_node_scalar)
	{
		if (src_node_scalar.scalar_pointer_manager != &src_node_scalar.scalar_pointer)
		{
			this->scalar_pointer_manager = src_node_scalar.scalar_pointer_manager;
			this->array_position = src_node_scalar.array_position;
		}
		else
		{
			strcpy(this->scalar_value, src_node_scalar.scalar_value);
			this->array_position = 0;
		}
		
		sprintf(node_scalar_name, "%s-copy", src_node_scalar.node_scalar_name);
	}

private:
	char*** scalar_pointer_manager;
	char** scalar_pointer;
	char* scalar_value;
	unsigned int array_position;
	char node_scalar_name[64];
};

#endif // NODE_SCALAR_CLASS_H_INCLUDED
