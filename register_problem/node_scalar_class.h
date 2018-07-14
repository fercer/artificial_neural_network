#ifndef NODE_SCALAR_CLASS_H_INCLUDED
#define NODE_SCALAR_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "node_graphics/typography_class.h"
#include "node_graphics/link_node_class.h"
#include "node_graphics/node_figure_class.h"

template<class class_value_type>
class NODE_SCALAR
{
public:
	NODE_SCALAR()
	{
		scalar_value = 0.0;
		scalar_pointer_manager = &scalar_pointer;
		scalar_pointer = &scalar_value;
		array_position = 0;
		strcpy(node_scalar_name, "unnamed");

		node_name_textbox = NULL;
		node_value_textbox = NULL;
		node_bg = NULL;
		link_node_output = NULL;
		defineGraphicNode();
	}

	NODE_SCALAR(const class_value_type src_value)
	{
		scalar_pointer_manager = &scalar_pointer;
		scalar_pointer = &scalar_value;
		array_position = 0;
		strcpy(node_scalar_name, "unnamed");

		scalar_value = src_value;

		node_name_textbox = NULL;
		node_value_textbox = NULL;
		node_bg = NULL;
		link_node_output = NULL;
		defineGraphicNode();
	}

	NODE_SCALAR(const NODE_SCALAR& src_node_scalar)
	{
		this->scalar_pointer_manager = &scalar_pointer;
		this->scalar_pointer = &scalar_value;
		this->array_position = 0;
		this->copyFromNodeScalar(src_node_scalar);

		this->node_name_textbox = NULL;
		this->node_value_textbox = NULL;
		this->node_bg = NULL;
		this->link_node_output = NULL;
		this->defineGraphicNode();
	}


	NODE_SCALAR operator=(const NODE_SCALAR& src_node_scalar)
	{
		if (this != &src_node_scalar)
		{
			this->scalar_pointer_manager = &scalar_pointer;
			this->scalar_pointer = &scalar_value;
			this->array_position = 0;
			this->copyFromNodeScalar(src_node_scalar);
		}
		return *this;
	}

	virtual ~NODE_SCALAR() 
	{
		if (node_name_textbox)
		{
			delete node_name_textbox;
		}

		if (node_value_textbox)
		{
			delete node_value_textbox;
		}

		if (node_bg)
		{
			delete node_bg;
		}

		if (link_node_output)
		{
			delete link_node_output;
		}
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
		char n_to_str_buffer[16];
		snprintf(n_to_str_buffer, 16, "%f", *(*scalar_pointer_manager + array_position));
		node_value_textbox->setText(n_to_str_buffer);
		return *(*scalar_pointer_manager + array_position);
	}


	virtual void setScalarValue(const class_value_type src_scalar_value)
	{
		char n_to_str_buffer[32];
		snprintf(n_to_str_buffer, 32, "%f", src_scalar_value);
		node_value_textbox->setText(n_to_str_buffer);
		*(*scalar_pointer_manager + array_position) = src_scalar_value;
	}


	void setNodeScalarName(const char * src_name)
	{
		node_name_textbox->setText(src_name);
		strcpy(node_scalar_name, src_name);
	}
	
	std::vector<FIGURE_2D*>::iterator getGraphicNode()
	{
		return graphic_nodes.begin();
	}

	unsigned int getGraphicNodeCount()
	{
		return (unsigned int)graphic_nodes.size();
	}

protected:
	std::vector<FIGURE_2D*> graphic_nodes;

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

		sprintf(node_scalar_name, "%s-copy", src_node_scalar.node_scalar_name);
	}

private:
	class_value_type * scalar_pointer;
	class_value_type scalar_value;
	char node_scalar_name[64];

	TYPOGRAPHY_CLASS * node_name_textbox;
	TYPOGRAPHY_CLASS * node_value_textbox;
	NODE_FIGURE * node_bg;
	LINK_NODE * link_node_output;

	void defineGraphicNode()
	{
		node_name_textbox = new TYPOGRAPHY_CLASS;
		node_name_textbox->setCharactersMapFilename("letter_positions.dat");
		node_name_textbox->loadTexture("texture_typography_map.ppm");
		node_name_textbox->setBoundingBox(4.0, 1.0);
		node_name_textbox->setBackgroundColor(111, 111, 145);
		node_name_textbox->setFontColor(255, 255, 209);
		node_name_textbox->moveFigure(0.5, 1.5, 0.2);
		node_name_textbox->setText(node_scalar_name);

		graphic_nodes.push_back(node_name_textbox);

		node_value_textbox = new TYPOGRAPHY_CLASS;
		node_value_textbox->setCharactersMapFilename("letter_positions.dat");
		node_value_textbox->loadTexture("texture_typography_map.ppm");
		node_value_textbox->setBoundingBox(4.5, 1.0);
		node_value_textbox->setBackgroundColor(255, 255, 255);
		node_value_textbox->setFontColor(0, 0, 0);
		node_value_textbox->moveFigure(0.0, -1.0, 0.2);
		int decimal, sign;
		node_value_textbox->setText(fcvt(*(*scalar_pointer_manager + array_position), 6, &decimal, &sign));

		graphic_nodes.push_back(node_value_textbox);

		node_bg = new NODE_FIGURE;
		node_bg->loadTexture("link_nodes_nn.ppm");

		graphic_nodes.push_back(node_bg);

		link_node_output = new LINK_NODE;
		link_node_output->loadTexture("link_nodes_nn.ppm");
		link_node_output->moveFigure(2.5, 0.0, 0.0);

		graphic_nodes.push_back(link_node_output);
	}
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
		strcpy(node_scalar_name, "unnamed");

		node_name_textbox = NULL;
		node_value_textbox = NULL;
		node_bg = NULL;
		link_node_output = NULL;
		defineGraphicNode();
	}


	NODE_SCALAR(const char * src_string)
	{
		scalar_pointer_manager = &scalar_pointer;
		scalar_pointer = &scalar_value;
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

		node_name_textbox = NULL;
		node_value_textbox = NULL;
		node_bg = NULL;
		link_node_output = NULL;
		defineGraphicNode();
	}

	NODE_SCALAR(const NODE_SCALAR& src_node_scalar)
	{
		this->scalar_pointer_manager = &this->scalar_pointer;
		this->scalar_pointer = &this->scalar_value;
		this->scalar_value = NULL;
		this->array_position = 0;
		this->copyFromNodeScalar(src_node_scalar);

		this->node_name_textbox = NULL;
		this->node_value_textbox = NULL;
		this->node_bg = NULL;
		this->link_node_output = NULL;
		this->defineGraphicNode();
	}


	NODE_SCALAR operator=(const NODE_SCALAR& src_node_scalar)
	{
		if (this != &src_node_scalar)
		{
			this->scalar_pointer_manager = &this->scalar_pointer;
			this->scalar_pointer = &this->scalar_value;
			this->scalar_value = NULL;
			this->array_position = 0;
			this->copyFromNodeScalar(src_node_scalar);
		}

		return *this;
	}

	virtual ~NODE_SCALAR()
	{
		if (node_name_textbox)
		{
			delete node_name_textbox;
		}

		if (node_value_textbox)
		{
			delete node_value_textbox;
		}

		if (node_bg)
		{
			delete node_bg;
		}

		if (link_node_output)
		{
			delete link_node_output;
		}

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
		return *(*scalar_pointer_manager + array_position);
	}


	virtual void setScalarValue(const char * src_scalar_value)
	{
		if (*(*scalar_pointer_manager + array_position) && (strcmp(src_scalar_value, *(*scalar_pointer_manager + array_position)) == 0))
		{
			return;
		}

		const unsigned int string_length = (unsigned int)strlen(src_scalar_value) + 1;

		if (string_length == 1)
		{
			**(*scalar_pointer_manager + array_position) = (char)'/0';
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

		node_value_textbox->setText(src_scalar_value);
		strcpy(*(*scalar_pointer_manager + array_position), src_scalar_value);

	}

	void setNodeScalarName(const char * src_name)
	{
		node_name_textbox->setText(src_name);
		strcpy(node_scalar_name, src_name);
	}
	
	std::vector<FIGURE_2D*>::iterator getGraphicNode()
	{
		return graphic_nodes.begin();
	}

	unsigned int getGraphicNodeCount()
	{
		return (unsigned int)graphic_nodes.size();
	}

protected:
	std::vector<FIGURE_2D*> graphic_nodes;

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

		sprintf(node_scalar_name, "%s-copy", src_node_scalar.node_scalar_name);
	}

private:
	char*** scalar_pointer_manager;
	char** scalar_pointer;
	char* scalar_value;
	unsigned int array_position;
	char node_scalar_name[64];

	TYPOGRAPHY_CLASS * node_name_textbox;
	TYPOGRAPHY_CLASS * node_value_textbox;
	NODE_FIGURE * node_bg;
	LINK_NODE * link_node_output;

	void defineGraphicNode()
	{
		node_name_textbox = new TYPOGRAPHY_CLASS;
		node_name_textbox->setCharactersMapFilename("letter_positions.dat");
		node_name_textbox->loadTexture("texture_typography_map.ppm");
		node_name_textbox->setBoundingBox(4.0, 1.0);
		node_name_textbox->setBackgroundColor(111, 111, 145);
		node_name_textbox->setFontColor(255, 255, 209);
		node_name_textbox->moveFigure(0.5, 1.5, 0.2);
		node_name_textbox->setText(node_scalar_name);

		graphic_nodes.push_back(node_name_textbox);

		node_value_textbox = new TYPOGRAPHY_CLASS;
		node_value_textbox->setCharactersMapFilename("letter_positions.dat");
		node_value_textbox->loadTexture("texture_typography_map.ppm");
		node_value_textbox->setBoundingBox(4.5, 0.5);
		node_value_textbox->setBackgroundColor(255, 255, 255);
		node_value_textbox->setFontColor(0, 0, 0);
		node_value_textbox->moveFigure(0.0, -1.0, 0.2);
		node_value_textbox->setText(*(*scalar_pointer_manager + array_position));

		graphic_nodes.push_back(node_value_textbox);

		node_bg = new NODE_FIGURE;
		node_bg->loadTexture("link_nodes_nn.ppm");

		graphic_nodes.push_back(node_bg);

		link_node_output = new LINK_NODE;
		link_node_output->loadTexture("link_nodes_nn.ppm");
		link_node_output->moveFigure(2.5, 0.0, 0.0);

		graphic_nodes.push_back(link_node_output);
	}
};

#endif // NODE_SCALAR_CLASS_H_INCLUDED

