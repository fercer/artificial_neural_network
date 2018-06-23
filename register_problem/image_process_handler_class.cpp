#include "image_process_handler_class.h"

IMAGE_PROCESS_HANDLER::IMAGE_PROCESS_HANDLER()
{
	sprintf(filename, "NOT-DEFINED");
	
	string_node_scalar_count = 0;
	numeric_node_scalar_count = 0;
	nodes_operation_count = 0;
	image_operation_count = 0;
	image_scalar_operation_count = 0;
}

IMAGE_PROCESS_HANDLER::IMAGE_PROCESS_HANDLER(const IMAGE_PROCESS_HANDLER & src_image_process_handler)
{

}

IMAGE_PROCESS_HANDLER IMAGE_PROCESS_HANDLER::operator=(const IMAGE_PROCESS_HANDLER & src_image_process_handler)
{
	return IMAGE_PROCESS_HANDLER();
}

IMAGE_PROCESS_HANDLER::~IMAGE_PROCESS_HANDLER()
{

}


void IMAGE_PROCESS_HANDLER::setFilename(const char * src_filename)
{
	strcpy(filename, src_filename);
}


void IMAGE_PROCESS_HANDLER::loadProcess()
{
	xml_file_handler.setFilename(filename);
	xml_file_handler.loadXMLFile();

	/* Read the root node, it must be an Image Process */
	XML_NODE_LEAF * file_root = xml_file_handler.getRoot();

	if (strcmp(file_root->getIdentifier(), "ImageProcessing") != 0)
	{
		return;
	}

	/* Read each node of the process and create the nodes network, without ralations: */
	XML_NODE_LEAF * current_node = file_root->getFirstChild();

	while (current_node)
	{
		char * current_node_identifier = current_node->getIdentifier();

		int current_operation_node_code = 0;
		unsigned int current_node_identifier_length = strlen(current_node_identifier);
		for (unsigned int character_index = 0; character_index < current_node_identifier_length; character_index++)
		{
			current_operation_node_code += (int)*(current_node_identifier + character_index);
		}

		// Cast the identifier string to the corresponding operation base type:
		const int current_operation_node_base_type = current_operation_node_code;

		// Find the operation type:
		char * current_node_operation_type_identifier = current_node->findChild("type")->getValue();
		current_node_identifier_length = strlen(current_node_identifier);
		current_operation_node_code = 0;
		for (unsigned int character_index = 0; character_index < current_node_identifier_length; character_index++)
		{
			current_operation_node_code += (int)*(current_node_operation_type_identifier + character_index);
		}

		// Cast the identifier string to the corresponding operation base type:
		const int current_operation_node_derived_type = current_operation_node_code;

		// Get the operation name:
		char * current_node_operation_name = current_node->findChild("name")->getValue();

		// Get the operation position:
		const int current_operation_position = atoi(current_node->findChild("position")->getValue());

		NODE_SCALAR<double> * new_numeric_node = NULL;
		NODE_SCALAR<char*> * new_string_node = NULL;
		NODES_SCALAR_OPERATION * new_node_operation = NULL;
		IMAGE_OPERATION * new_image_operation = NULL;
		IMAGE_SCALAR_OPERATION * new_image_scalar_operation = NULL;

		// Create the corresponding operation node:
		switch (current_operation_node_derived_type)
		{
		case NV_NUMERIC:
			new_numeric_node = new NODE_SCALAR<double>;
			break;

		case NV_STRING:
			new_string_node = new NODE_SCALAR<char*>;
			break;

		case NOV_SUM:
			new_node_operation = new SCALAR_SUM_NODES_OPERATION;
			break;

		case NOV_SUB:
			new_node_operation = new SCALAR_SUB_NODES_OPERATION;
			break;

		case NOV_MULT:
			new_node_operation = new SCALAR_MULT_NODES_OPERATION;
			break;

		case NOV_DIV:
			new_node_operation = new SCALAR_DIV_NODES_OPERATION;
			break;

		case NOV_SQRT:
			new_node_operation = new SCALAR_SQRT_NODES_OPERATION;
			break;

		case IO_DIV:
			new_image_operation = new DIV_IMAGE_OPERATION;
			break;

		case IO_MULT:
			new_image_operation = new MULT_IMAGE_OPERATION;
			break;

		case IO_SUM:
			new_image_operation = new SUM_IMAGE_OPERATION;
			break;

		case IO_LOAD:
			new_image_operation = new LOAD_IMAGE_OPERATION;
			break;

		case IO_SAVE:
			new_image_operation = new SAVE_IMAGE_OPERATION;
			break;

		case IO_FILTER:
			new_image_operation = new FILTER_IMAGE_OPERATION;
			break;

		case IO_ROTATE:
			new_image_operation = new ROTATE_IMAGE_OPERATION;
			break;

		case IO_TRANSLATE:
			new_image_operation = new TRANSLATE_IMAGE_OPERATION;
			break;

		case IO_SQRT:
			new_image_operation = new SQROOT_IMAGE_OPERATION;
			break;

		case IO_ONES:
			new_image_operation = new ONES_IMAGE_OPERATION;
			break;

		case ISO_SUM:
			new_image_scalar_operation = new SUM_IMAGE_SCALAR_OPERATION;
			break;

		case ISO_MAX:
			new_image_scalar_operation = new MAX_IMAGE_SCALAR_OPERATION;
			break;

		case ISO_MIN:
			new_image_scalar_operation = new MIN_IMAGE_SCALAR_OPERATION;
			break;

		case ISO_WIDTH:
			new_image_scalar_operation = new WIDTH_IMAGE_SCALAR_OPERATION;
			break;

		case ISO_HEIGHT:
			new_image_scalar_operation = new HEIGHT_IMAGE_SCALAR_OPERATION;
			break;

		default:
			printf("<<Error: The operation type \'%i\' is not defined>>\n", current_operation_node_code);
			break;
		}




		// Assign the attributes to the new operation node:
		if (new_numeric_node)
		{
			new_numeric_node->setNodeScalarName(current_node_operation_name);
			new_numeric_node->setScalarValue(atof(current_node->findChild("Value")->getValue()));
			// Add the original position to a list ...
			numeric_node_scalar_count = numeric_node_scalar_list.assignNodeValue(numeric_node_scalar_count, new_numeric_node);
		}
		else if (new_string_node)
		{
			new_string_node->setNodeScalarName(current_node_operation_name);
			new_string_node->setScalarValue(current_node->findChild("Value")->getValue());
			// Add the original position to a list ...
			string_node_scalar_count = string_node_scalar_list.assignNodeValue(string_node_scalar_count, new_string_node);
		}
		else if (new_node_operation)
		{
			new_node_operation->setNodeScalarName(current_node_operation_name);
			// Add the original position to a list ...
			nodes_operation_count = nodes_operation_list.assignNodeValue(nodes_operation_count);
		}

		// Continue loading the following nodes:
		current_node = file_root->getNextChild();
	}
}
