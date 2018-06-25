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
		strcpy(node_scalar_name, "unnamed");
		scalar_value_has_changed = true;
	}


	NODE_SCALAR(const class_value_type src_value)
	{
		scalar_pointer_manager = &scalar_pointer;
		scalar_pointer = &scalar_value;
		array_position = 0;
		strcpy(node_scalar_name, "unnamed");
		scalar_value_has_changed = true;

		scalar_value = src_value;
	}

	NODE_SCALAR(const NODE_SCALAR& src_node_scalar)
	{
		this->scalar_pointer_manager = &scalar_pointer;
		this->scalar_pointer = &scalar_value;
		this->array_position = 0;
		strcpy(this->node_scalar_name, "unnamed");

		this->copyFromNodeScalar(src_node_scalar);
	}

	NODE_SCALAR operator=(const NODE_SCALAR& src_node_scalar)
	{
		if (this != &src_node_scalar)
		{
			this->scalar_pointer_manager = &scalar_pointer;
			this->scalar_pointer = &scalar_value;
			this->array_position = 0;
			strcpy(this->node_scalar_name, "unnamed");

			this->copyFromNodeScalar(src_node_scalar);
		}
		return *this;
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
		scalar_value_has_changed = false;
		return *(*scalar_pointer_manager + array_position);
	}

	virtual void setScalarValue(const class_value_type src_scalar_value)
	{
		*(*scalar_pointer_manager + array_position) = src_scalar_value;
		scalar_value_has_changed = true;
	}

	void setNodeScalarName(const char * src_name)
	{
		strcpy(node_scalar_name, src_name);
	}

	bool getValueHasChanged()
	{
		return scalar_value_has_changed;
	}

protected:
	class_value_type ** scalar_pointer_manager;
	unsigned int array_position;

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

		this->scalar_value_has_changed = src_node_scalar.scalar_value_has_changed;
		sprintf(node_scalar_name, "%s-copy", src_node_scalar.node_scalar_name);
	}

private:
	class_value_type * scalar_pointer;
	class_value_type scalar_value;
	bool scalar_value_has_changed;
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
		scalar_value = NULL;
		array_position = 0;
		scalar_value_has_changed = true;

		strcpy(node_scalar_name, "unnamed");
	}

	NODE_SCALAR(const char * src_string)
	{
		scalar_pointer_manager = &scalar_pointer;
		scalar_pointer = &scalar_value;
		scalar_value_has_changed = true;

		strcpy(node_scalar_name, "unnamed");

		const unsigned int string_length = (unsigned int)strlen(src_string) + 1;
		if (string_length > 1)
		{
			scalar_value = (char*)calloc(string_length, sizeof(char));
			strcpy(scalar_value, src_string);
		}
		else
		{
			scalar_value = NULL;
		}

		array_position = 0;

	}

	NODE_SCALAR(const NODE_SCALAR& src_node_scalar)
	{
		this->scalar_pointer_manager = &this->scalar_pointer;
		this->scalar_pointer = &this->scalar_value;
		this->scalar_value = NULL;
		this->array_position = 0;
		strcpy(this->node_scalar_name, "unnamed");

		this->copyFromNodeScalar(src_node_scalar);
	}


	NODE_SCALAR operator=(const NODE_SCALAR& src_node_scalar)
	{
		if (this != &src_node_scalar)
		{
			this->scalar_pointer_manager = &this->scalar_pointer;
			this->scalar_pointer = &this->scalar_value;
			this->scalar_value = NULL;
			this->array_position = 0;
			strcpy(this->node_scalar_name, "unnamed");

			this->copyFromNodeScalar(src_node_scalar);
		}

		return *this;
	}


	virtual ~NODE_SCALAR()
	{
		if (scalar_value)
		{
			free(scalar_value);
		}
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
		scalar_value_has_changed = false;
		return *(*scalar_pointer_manager + array_position);
	}


	virtual void setScalarValue(const char * src_scalar_value)
	{
		const unsigned int string_length = strlen(src_scalar_value) + 1;

		if (string_length == 1)
		{
			**(*scalar_pointer_manager + array_position) = '/0';
			return;
		}

		if (!*(*scalar_pointer_manager + array_position))
		{
			*(*scalar_pointer_manager + array_position) = (char*)calloc(string_length, sizeof(char));
		}
		else if (string_length > (unsigned int)(strlen(*(*scalar_pointer_manager + array_position)) + 1))
		{
			free(*(*scalar_pointer_manager + array_position));
			*(*scalar_pointer_manager + array_position) = (char*)calloc(string_length, sizeof(char));
		}

		strcpy(*(*scalar_pointer_manager + array_position), src_scalar_value);
		scalar_value_has_changed = true;
	}


	void setNodeScalarName(const char * src_name)
	{
		strcpy(node_scalar_name, src_name);
	}

	bool getValueHasChanged()
	{
		return scalar_value_has_changed;
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
			this->setScalarValue(*(*(src_node_scalar.scalar_pointer_manager) + src_node_scalar.array_position));
			this->array_position = 0;
		}

		this->scalar_value_has_changed = src_node_scalar.scalar_value_has_changed;
		sprintf(node_scalar_name, "%s-copy", src_node_scalar.node_scalar_name);
	}

private:
	char*** scalar_pointer_manager;
	char** scalar_pointer;
	char* scalar_value;
	bool scalar_value_has_changed;
	unsigned int array_position;
	char node_scalar_name[64];
};

#endif // NODE_SCALAR_CLASS_H_INCLUDED
