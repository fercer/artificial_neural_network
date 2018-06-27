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
	for (unsigned int operation_index = 0; operation_index < numeric_node_scalar_count; operation_index++)
	{
		delete numeric_node_scalar_list.getNodeValue(operation_index);
	}

	for (unsigned int operation_index = 0; operation_index < string_node_scalar_count; operation_index++)
	{
		delete string_node_scalar_list.getNodeValue(operation_index);
	}

	for (unsigned int operation_index = 0; operation_index < nodes_operation_count; operation_index++)
	{
		delete nodes_operation_list.getNodeValue(operation_index);
	}

	for (unsigned int operation_index = 0; operation_index < image_operation_count; operation_index++)
	{
		delete image_operation_list.getNodeValue(operation_index);
	}

	for (unsigned int operation_index = 0; operation_index < image_scalar_operation_count; operation_index++)
	{
		delete image_scalar_operation_list.getNodeValue(operation_index);
	}
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
		unsigned int current_node_identifier_length = (unsigned int)strlen(current_node_identifier);
		for (unsigned int character_index = 0; character_index < current_node_identifier_length; character_index++)
		{
			current_operation_node_code += (int)*(current_node_identifier + character_index);
		}

		// Cast the identifier string to the corresponding operation base type:
		const int current_operation_node_base_type = current_operation_node_code;

		// Find the operation type:
		char * current_node_operation_type_identifier = current_node->findChild("type")->getValue();
		current_node_identifier_length = (unsigned int)strlen(current_node_operation_type_identifier);
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

		printf("\'%s\'; base = %i, derived = %i\n", current_node_operation_type_identifier, current_operation_node_base_type, current_operation_node_derived_type);

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

		case NOV_ATANGENT:
			new_node_operation = new SCALAR_ATAN_NODES_OPERATION;
			break;

		case NOV_TANGENT:
			new_node_operation = new SCALAR_TAN_NODES_OPERATION;
			break;

		case NOV_SINUS:
			new_node_operation = new SCALAR_SIN_NODES_OPERATION;
			break;

		case NOV_COSINUS:
			new_node_operation = new SCALAR_COS_NODES_OPERATION;
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
			numeric_node_scalar_position_list.assignNodeValue(numeric_node_scalar_count, current_operation_position);
			numeric_node_scalar_count = numeric_node_scalar_list.assignNodeValue(numeric_node_scalar_count, new_numeric_node);
		}
		else if (new_string_node)
		{
			new_string_node->setNodeScalarName(current_node_operation_name);
			new_string_node->setScalarValue(current_node->findChild("Value")->getValue());
			string_node_scalar_position_list.assignNodeValue(string_node_scalar_count, current_operation_position);
			string_node_scalar_count = string_node_scalar_list.assignNodeValue(string_node_scalar_count, new_string_node);
		}
		else if (new_node_operation)
		{
			new_node_operation->setNodeScalarName(current_node_operation_name);

			unsigned int numeric_nodes_required = new_node_operation->getInputNumericNodesRequired();
			for (unsigned int node_index = 0; node_index < numeric_nodes_required; node_index++)
			{
				XML_NODE_LEAF * current_node_child = current_node->getFirstChild();
				char * current_required_node_identifier = new_node_operation->getNumericInputName(node_index);
				while (current_node_child)
				{
					if (!strcmp(current_node_child->getIdentifier(), "Node") && 
						current_node_child->findChild("id") &&
						!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
						)
					{
						break;
					}

					current_node_child = current_node->getNextChild();
				}

				/* If the children actually exists, and the type is SELF:
				take the value as locally defined, else, the value will be assigned in the following assignation cycle:
				*/
				if (current_node_child && current_node->findChild("type") && 
					current_node->findChild("type") &&
					!strcmp(current_node_child->findChild("type")->getValue(), "SELF"))
				{
					new_node_operation->assignNodeValue(node_index, atof(current_node_child->getValue()));
				}
			}

			unsigned int string_nodes_required = new_node_operation->getInputStringNodesRequired();
			for (unsigned int node_index = 0; node_index < string_nodes_required; node_index++)
			{
				XML_NODE_LEAF * current_node_child = current_node->getFirstChild();
				char * current_required_node_identifier = new_node_operation->getStringInputName(node_index);
				while (current_node_child &&
					!strcmp(current_node_child->getIdentifier(), "Node") &&
					!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
					)
				{
					current_node_child = current_node->getNextChild();
				}

				/* If the children actually exists, and the type is SELF:
				take the value as locally defined, else, the value will be assigned in the following assignation cycle:
				*/
				if (current_node_child && current_node_child->findChild("type") &&
					!strcmp(current_node_child->findChild("type")->getValue(), "SELF"))
				{
					new_node_operation->assignNodeValue(node_index, current_node_child->getValue());
				}
			}

			nodes_operation_position_list.assignNodeValue(nodes_operation_count, current_operation_position);
			nodes_operation_count = nodes_operation_list.assignNodeValue(nodes_operation_count, new_node_operation);
		}
		else if (new_image_operation)
		{
			new_image_operation->setImageName(current_node_operation_name);

			unsigned int numeric_nodes_required = new_image_operation->getInputNumericNodesRequired();
			for (unsigned int node_index = 0; node_index < numeric_nodes_required; node_index++)
			{
				XML_NODE_LEAF * current_node_child = current_node->getFirstChild();
				char * current_required_node_identifier = new_image_operation->getNumericInputName(node_index);
				while (current_node_child)
				{
					if (!strcmp(current_node_child->getIdentifier(), "Node") &&
						current_node_child->findChild("id") &&
						!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
						)
					{
						break;
					}

					current_node_child = current_node->getNextChild();
				}

				/* If the children actually exists, and the type is SELF:
				take the value as locally defined, else, the value will be assigned in the following assignation cycle:
				*/
				if (current_node_child && current_node->findChild("type") &&
					current_node->findChild("type") &&
					!strcmp(current_node_child->findChild("type")->getValue(), "SELF"))
				{
					new_image_operation->assignNodeValue(node_index, atof(current_node_child->getValue()));
				}
			}

			unsigned int string_nodes_required = new_image_operation->getInputStringNodesRequired();
			for (unsigned int node_index = 0; node_index < string_nodes_required; node_index++)
			{
				XML_NODE_LEAF * current_node_child = current_node->getFirstChild();
				char * current_required_node_identifier = new_image_operation->getStringInputName(node_index);
				while (current_node_child)
				{
					if (!strcmp(current_node_child->getIdentifier(), "Node") &&
						current_node_child->findChild("id") &&
						!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
						)
					{
						break;
					}

					current_node_child = current_node->getNextChild();
				}

				/* If the children actually exists, and the type is SELF:
				take the value as locally defined, else, the value will be assigned in the following assignation cycle:
				*/
				if (current_node_child && current_node_child->findChild("type") && 
					!strcmp(current_node_child->findChild("type")->getValue(), "SELF"))
				{
					new_image_operation->assignNodeValue(node_index, current_node_child->getValue());
				}
			}

			image_operation_position_list.assignNodeValue(image_operation_count, current_operation_position);
			image_operation_count = image_operation_list.assignNodeValue(image_operation_count, new_image_operation);
		}
		else if (new_image_scalar_operation)
		{
			new_image_scalar_operation->setNodeScalarName(current_node_operation_name);

			unsigned int numeric_nodes_required = new_image_scalar_operation->getInputNumericNodesRequired();
			for (unsigned int node_index = 0; node_index < numeric_nodes_required; node_index++)
			{
				XML_NODE_LEAF * current_node_child = current_node->getFirstChild();
				char * current_required_node_identifier = new_image_scalar_operation->getNumericInputName(node_index);
				while (current_node_child)
				{
					if (!strcmp(current_node_child->getIdentifier(), "Node") &&
						current_node_child->findChild("id") &&
						!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
						)
					{
						break;
					}

					current_node_child = current_node->getNextChild();
				}

				/* If the children actually exists, and the type is SELF:
				take the value as locally defined, else, the value will be assigned in the following assignation cycle:
				*/
				if (current_node_child && current_node->findChild("type") &&
					current_node->findChild("type") &&
					!strcmp(current_node_child->findChild("type")->getValue(), "SELF"))
				{
					new_image_scalar_operation->assignNodeValue(node_index, atof(current_node_child->getValue()));
				}
			}

			unsigned int string_nodes_required = new_image_scalar_operation->getInputStringNodesRequired();
			for (unsigned int node_index = 0; node_index < string_nodes_required; node_index++)
			{
				XML_NODE_LEAF * current_node_child = current_node->getFirstChild();
				char * current_required_node_identifier = new_image_scalar_operation->getStringInputName(node_index);
				while (current_node_child)
				{
					if (!strcmp(current_node_child->getIdentifier(), "Node") &&
						current_node_child->findChild("id") &&
						!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
						)
					{
						break;
					}

					current_node_child = current_node->getNextChild();
				}

				/* If the children actually exists, and the type is SELF:
				take the value as locally defined, else, the value will be assigned in the following assignation cycle:
				*/
				if (current_node_child && current_node_child->findChild("type") &&
					!strcmp(current_node_child->findChild("type")->getValue(), "SELF"))
				{
					new_image_scalar_operation->assignNodeValue(node_index, current_node_child->getValue());
				}
			}

			image_scalar_operation_position_list.assignNodeValue(image_scalar_operation_count, current_operation_position);
			image_scalar_operation_count = image_scalar_operation_list.assignNodeValue(image_scalar_operation_count, new_image_scalar_operation);
		}
		
		// Continue loading the following nodes:
		current_node = file_root->getNextChild();
	}


	current_node = file_root->getFirstChild();

	while (current_node)
	{
		char * current_node_identifier = current_node->getIdentifier();

		int current_operation_node_code = 0;
		unsigned int current_node_identifier_length = (unsigned int)strlen(current_node_identifier);
		for (unsigned int character_index = 0; character_index < current_node_identifier_length; character_index++)
		{
			current_operation_node_code += (int)*(current_node_identifier + character_index);
		}

		// Cast the identifier string to the corresponding operation base type:
		const int current_operation_node_base_type = current_operation_node_code;

		// Find the operation type:
		char * current_node_operation_type_identifier = current_node->findChild("type")->getValue();
		current_node_identifier_length = (unsigned int)strlen(current_node_operation_type_identifier);
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
		unsigned int current_operation_list_index = 0;

		NODE_SCALAR<double> * new_numeric_node = NULL;
		NODE_SCALAR<char*> * new_string_node = NULL;
		NODES_SCALAR_OPERATION * new_node_operation = NULL;
		IMAGE_OPERATION * new_image_operation = NULL;
		IMAGE_SCALAR_OPERATION * new_image_scalar_operation = NULL;

		printf("\'%s\'; base = %i, derived = %i\n", current_node_operation_type_identifier, current_operation_node_base_type, current_operation_node_derived_type);

		// Create the corresponding operation node:
		switch (current_operation_node_derived_type)
		{
		case NV_NUMERIC:
		case NV_STRING:
			break;
		default:
		{
			switch (current_operation_node_base_type)
			{
			case OT_NODES_SCALAR_OPERATION:
			{
				while (nodes_operation_position_list.getNodeValue(current_operation_list_index) != current_operation_position)
				{
					current_operation_list_index++;
				}

				new_node_operation = nodes_operation_list.getNodeValue(current_operation_list_index);
				break;
			}

			case OT_IMAGE_OPERATION:
			{
				while (image_operation_position_list.getNodeValue(current_operation_list_index) != current_operation_position)
				{
					current_operation_list_index++;
				}

				new_image_operation = image_operation_list.getNodeValue(current_operation_list_index);
				break;
			}

			case OT_IMAGE_SCALAR_OPERATION:
			{
				while (image_scalar_operation_position_list.getNodeValue(current_operation_list_index) != current_operation_position)
				{
					current_operation_list_index++;
				}

				new_image_scalar_operation = image_scalar_operation_list.getNodeValue(current_operation_list_index);
				break;
			}

			default:
				printf("<<Error: The operation type \'%i\' is not defined>>\n", current_operation_node_base_type);
				break;
			}
		}
		}

		// Perform the linkage with the nodes of the network:
		if (new_node_operation)
		{
			unsigned int numeric_nodes_required = new_node_operation->getInputNumericNodesRequired();
			for (unsigned int node_index = 0; node_index < numeric_nodes_required; node_index++)
			{
				XML_NODE_LEAF * current_node_child = current_node->getFirstChild();
				char * current_required_node_identifier = new_node_operation->getNumericInputName(node_index);
				while (current_node_child)
				{
					if (!strcmp(current_node_child->getIdentifier(), "Node") &&
						current_node_child->findChild("id") &&
						!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
						)
					{
						break;
					}

					current_node_child = current_node->getNextChild();
				}

				/* If the children actually exists, and the type is defined:
				Assign the pointer of the defined position:
				*/
				if (current_node_child && current_node->findChild("type") &&
					current_node->findChild("type")->getValue())
				{
					const int linking_operation_position = atof(current_node_child->getValue());
					unsigned int linking_operation_list_index = 0;

					if (!strcmp(current_node_child->findChild("type")->getValue(), "NV"))
					{
						while (numeric_node_scalar_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
						{
							linking_operation_list_index++;
						}

						new_node_operation->assignNodeValue(node_index,
							numeric_node_scalar_list.getNodeValue(linking_operation_list_index));
					}
					else if (!strcmp(current_node_child->findChild("type")->getValue(), "NOV"))
					{
						while (nodes_operation_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
						{
							linking_operation_list_index++;
						}

						new_node_operation->assignNodeValue(node_index,
							nodes_operation_list.getNodeValue(linking_operation_list_index));

					}
					else if (!strcmp(current_node_child->findChild("type")->getValue(), "ISO"))
					{
						while (image_scalar_operation_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
						{
							linking_operation_list_index++;
						}

						new_node_operation->assignNodeValue(node_index,
							image_scalar_operation_list.getNodeValue(linking_operation_list_index));
					}
				}
			}

			unsigned int string_nodes_required = new_node_operation->getInputStringNodesRequired();
			for (unsigned int node_index = 0; node_index < string_nodes_required; node_index++)
			{
				XML_NODE_LEAF * current_node_child = current_node->getFirstChild();
				char * current_required_node_identifier = new_node_operation->getStringInputName(node_index);
				while (current_node_child &&
					!strcmp(current_node_child->getIdentifier(), "Node") &&
					!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
					)
				{
					current_node_child = current_node->getNextChild();
				}

				/* If the children actually exists, and the type is defined:
				Assign the pointer of the defined position:
				*/
				if (current_node_child && current_node->findChild("type") &&
					current_node->findChild("type")->getValue())
				{
					const int linking_operation_position = atof(current_node_child->getValue());
					unsigned int linking_operation_list_index = 0;

					if (!strcmp(current_node_child->findChild("type")->getValue(), "NV"))
					{
						while (string_node_scalar_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
						{
							linking_operation_list_index++;
						}

						new_node_operation->assignNodeValue(node_index,
							string_node_scalar_list.getNodeValue(linking_operation_list_index));
					}
				}
			}				
		}
		else if (new_image_operation)
		{
			unsigned int numeric_nodes_required = new_image_operation->getInputNumericNodesRequired();
			for (unsigned int node_index = 0; node_index < numeric_nodes_required; node_index++)
			{
				XML_NODE_LEAF * current_node_child = current_node->getFirstChild();
				char * current_required_node_identifier = new_image_operation->getNumericInputName(node_index);
				while (current_node_child)
				{
					if (!strcmp(current_node_child->getIdentifier(), "Node") &&
						current_node_child->findChild("id") &&
						!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
						)
					{
						break;
					}

					current_node_child = current_node->getNextChild();
				}

				/* If the children actually exists, and the type is defined:
				Assign the pointer of the defined position:
				*/
				if (current_node_child && current_node->findChild("type") &&
					current_node->findChild("type")->getValue())
				{
					const int linking_operation_position = atof(current_node_child->getValue());
					unsigned int linking_operation_list_index = 0;

					if (!strcmp(current_node_child->findChild("type")->getValue(), "NV"))
					{
						while (numeric_node_scalar_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
						{
							linking_operation_list_index++;
						}

						new_image_operation->assignNodeValue(node_index,
							numeric_node_scalar_list.getNodeValue(linking_operation_list_index));
					}
					else if (!strcmp(current_node_child->findChild("type")->getValue(), "NOV"))
					{
						while (nodes_operation_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
						{
							linking_operation_list_index++;
						}

						new_image_operation->assignNodeValue(node_index,
							nodes_operation_list.getNodeValue(linking_operation_list_index));

					}
					else if (!strcmp(current_node_child->findChild("type")->getValue(), "ISO"))
					{
						while (image_scalar_operation_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
						{
							linking_operation_list_index++;
						}

						new_image_operation->assignNodeValue(node_index,
							image_scalar_operation_list.getNodeValue(linking_operation_list_index));
					}
				}
			}

			unsigned int string_nodes_required = new_image_operation->getInputStringNodesRequired();
			for (unsigned int node_index = 0; node_index < string_nodes_required; node_index++)
			{
				XML_NODE_LEAF * current_node_child = current_node->getFirstChild();
				char * current_required_node_identifier = new_image_operation->getStringInputName(node_index);
				while (current_node_child)
				{
					if (!strcmp(current_node_child->getIdentifier(), "Node") &&
						current_node_child->findChild("id") &&
						!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
						)
					{
						break;
					}

					current_node_child = current_node->getNextChild();
				}

				/* If the children actually exists, and the type is defined:
				Assign the pointer of the defined position:
				*/
				if (current_node_child && current_node->findChild("type") &&
					current_node->findChild("type")->getValue())
				{
					const int linking_operation_position = atof(current_node_child->getValue());
					unsigned int linking_operation_list_index = 0;

					if (!strcmp(current_node_child->findChild("type")->getValue(), "NV"))
					{
						while (string_node_scalar_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
						{
							linking_operation_list_index++;
						}

						new_image_operation->assignNodeValue(node_index,
							string_node_scalar_list.getNodeValue(linking_operation_list_index));
					}
				}
			}

			// Look for the operation nodes:
			XML_NODE_LEAF * current_node_child = current_node->getFirstChild();
			char current_required_node_identifier[] = "operation_A";
			while (current_node_child)
			{
				if (!strcmp(current_node_child->getIdentifier(), "Operation") &&
					current_node_child->findChild("id") &&
					!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
					)
				{
					break;
				}

				current_node_child = current_node->getNextChild();
			}

			/* If the children actually exists, and the type is defined:
			Assign the pointer of the defined position:
			*/
			if (current_node_child)
			{
				const int linking_operation_position = atof(current_node_child->getValue());
				unsigned int linking_operation_list_index = 0;

				while (image_operation_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
				{
					linking_operation_list_index++;
				}

				new_image_operation->setInputOperationA(image_operation_list.getNodeValue(linking_operation_list_index));
			}


			// Look for the operation nodes:
			current_node_child = current_node->getFirstChild();
			strcpy(current_required_node_identifier, "operation_B");
			while (current_node_child)
			{
				if (!strcmp(current_node_child->getIdentifier(), "Operation") &&
					current_node_child->findChild("id") &&
					!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
					)
				{
					break;
				}

				current_node_child = current_node->getNextChild();
			}

			/* If the children actually exists, and the type is defined:
			Assign the pointer of the defined position:
			*/
			if (current_node_child)
			{
				const int linking_operation_position = atof(current_node_child->getValue());
				unsigned int linking_operation_list_index = 0;

				while (image_operation_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
				{
					linking_operation_list_index++;
				}

				new_image_operation->setInputOperationB(image_operation_list.getNodeValue(linking_operation_list_index));
			}
		}
		else if (new_image_scalar_operation)
		{
			unsigned int numeric_nodes_required = new_image_scalar_operation->getInputNumericNodesRequired();
			for (unsigned int node_index = 0; node_index < numeric_nodes_required; node_index++)
			{
				XML_NODE_LEAF * current_node_child = current_node->getFirstChild();
				char * current_required_node_identifier = new_image_scalar_operation->getNumericInputName(node_index);
				while (current_node_child)
				{
					if (!strcmp(current_node_child->getIdentifier(), "Node") &&
						current_node_child->findChild("id") &&
						!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
						)
					{
						break;
					}

					current_node_child = current_node->getNextChild();
				}

				/* If the children actually exists, and the type is defined:
				Assign the pointer of the defined position:
				*/
				if (current_node_child && current_node->findChild("type") &&
					current_node->findChild("type")->getValue())
				{
					const int linking_operation_position = atof(current_node_child->getValue());
					unsigned int linking_operation_list_index = 0;

					if (!strcmp(current_node_child->findChild("type")->getValue(), "NV"))
					{
						while (numeric_node_scalar_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
						{
							linking_operation_list_index++;
						}

						new_image_scalar_operation->assignNodeValue(node_index,
							numeric_node_scalar_list.getNodeValue(linking_operation_list_index));
					}
					else if (!strcmp(current_node_child->findChild("type")->getValue(), "NOV"))
					{
						while (nodes_operation_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
						{
							linking_operation_list_index++;
						}

						new_image_scalar_operation->assignNodeValue(node_index,
							nodes_operation_list.getNodeValue(linking_operation_list_index));

					}
					else if (!strcmp(current_node_child->findChild("type")->getValue(), "ISO"))
					{
						while (image_scalar_operation_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
						{
							linking_operation_list_index++;
						}

						new_image_scalar_operation->assignNodeValue(node_index,
							image_scalar_operation_list.getNodeValue(linking_operation_list_index));
					}
				}
			}

			unsigned int string_nodes_required = new_image_scalar_operation->getInputStringNodesRequired();
			for (unsigned int node_index = 0; node_index < string_nodes_required; node_index++)
			{
				XML_NODE_LEAF * current_node_child = current_node->getFirstChild();
				char * current_required_node_identifier = new_image_scalar_operation->getStringInputName(node_index);
				while (current_node_child)
				{
					if (!strcmp(current_node_child->getIdentifier(), "Node") &&
						current_node_child->findChild("id") &&
						!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
						)
					{
						break;
					}

					current_node_child = current_node->getNextChild();
				}

				/* If the children actually exists, and the type is defined:
				Assign the pointer of the defined position:
				*/
				if (current_node_child && current_node->findChild("type") &&
					current_node->findChild("type")->getValue())
				{
					const int linking_operation_position = atof(current_node_child->getValue());
					unsigned int linking_operation_list_index = 0;

					if (!strcmp(current_node_child->findChild("type")->getValue(), "NV"))
					{
						while (string_node_scalar_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
						{
							linking_operation_list_index++;
						}

						new_image_scalar_operation->assignNodeValue(node_index,
							string_node_scalar_list.getNodeValue(linking_operation_list_index));
					}
				}
			}

			// Look for the operation nodes:
			XML_NODE_LEAF * current_node_child = current_node->getFirstChild();
			char current_required_node_identifier[] = "operation_A";
			while (current_node_child)
			{
				if (!strcmp(current_node_child->getIdentifier(), "Operation") &&
					current_node_child->findChild("id") &&
					!strcmp(current_node_child->findChild("id")->getValue(), current_required_node_identifier)
					)
				{
					break;
				}

				current_node_child = current_node->getNextChild();
			}

			/* If the children actually exists, and the type is defined:
			Assign the pointer of the defined position:
			*/
			if (current_node_child)
			{
				const int linking_operation_position = atof(current_node_child->getValue());
				unsigned int linking_operation_list_index = 0;

				while (image_operation_position_list.getNodeValue(linking_operation_list_index) != linking_operation_position)
				{
					linking_operation_list_index++;
				}

				new_image_scalar_operation->setInputOperation(image_operation_list.getNodeValue(linking_operation_list_index));
			}
		}

		// Continue loading the following nodes:
		current_node = file_root->getNextChild();
	}
}



void IMAGE_PROCESS_HANDLER::runProcess()
{
	for (unsigned int node_index = 0; node_index < nodes_operation_count; node_index++)
	{
		printf("Numeric node: %i\n", node_index);
		nodes_operation_list.getNodeValue(node_index)->getScalarValue();
	}

	for (unsigned int node_index = 0; node_index < image_operation_count; node_index++)
	{
		printf("Image operation: %i\n", node_index);
		image_operation_list.getNodeValue(node_index)->getImageData();
	}

	for (unsigned int node_index = 0; node_index < image_scalar_operation_count; node_index++)
	{
		printf("Image scalar operation: %i\n", node_index);
		image_scalar_operation_list.getNodeValue(node_index)->getScalarValue();
	}
}



void IMAGE_PROCESS_HANDLER::runNodeOperation(const unsigned int src_node_index)
{
	if (src_node_index >= nodes_operation_count)
	{
		return;
	}

	unsigned int position_node_index = 0;
	while (nodes_operation_position_list.getNodeValue(position_node_index) != src_node_index)
	{
		position_node_index++;
	}

	nodes_operation_list.getNodeValue(position_node_index)->getScalarValue();
}



void IMAGE_PROCESS_HANDLER::runImageOperation(const unsigned int src_node_index)
{
	if (src_node_index >= image_operation_count)
	{
		return;
	}

	unsigned int position_node_index = 0;
	while (image_operation_position_list.getNodeValue(position_node_index) != src_node_index)
	{
		position_node_index++;
	}

	image_operation_list.getNodeValue(position_node_index)->getImageData();
}



void IMAGE_PROCESS_HANDLER::runImageScalarOperation(const unsigned int src_node_index)
{
	if (src_node_index >= image_scalar_operation_count)
	{
		return;
	}

	unsigned int position_node_index = 0;
	while (image_scalar_operation_position_list.getNodeValue(position_node_index) != src_node_index)
	{
		position_node_index++;
	}

	image_scalar_operation_list.getNodeValue(position_node_index)->getScalarValue();
}



void IMAGE_PROCESS_HANDLER::setInputNodeValue(const unsigned int src_node_index, const double src_numeric_value)
{
	if (src_node_index >= numeric_node_scalar_count)
	{
		return;
	}

	unsigned int position_node_index = 0;
	while (numeric_node_scalar_position_list.getNodeValue(position_node_index) != src_node_index)
	{
		position_node_index++;
	}

	numeric_node_scalar_list.getNodeValue(position_node_index)->setScalarValue(src_numeric_value);
}



void IMAGE_PROCESS_HANDLER::setInputNodeValue(const unsigned int src_node_index, const char * src_string_value)
{
	if (src_node_index >= string_node_scalar_count)
	{
		return;
	}

	unsigned int position_node_index = 0;
	while (string_node_scalar_position_list.getNodeValue(position_node_index) != src_node_index)
	{
		position_node_index++;
	}

	string_node_scalar_list.getNodeValue(position_node_index)->setScalarValue(src_string_value);
}



void IMAGE_PROCESS_HANDLER::setInputNodeValue(const unsigned int src_node_index, const unsigned int src_operation_index, NODE_SCALAR<double>* src_new_link)
{
	if (src_node_index >= nodes_operation_count)
	{
		return;
	}
	
	unsigned int position_node_index = 0;
	while (nodes_operation_position_list.getNodeValue(position_node_index) != src_node_index)
	{
		position_node_index++;
	}

	if (src_operation_index >= nodes_operation_list.getNodeValue(position_node_index)->getInputNumericNodesRequired())
	{
		return;
	}

	nodes_operation_list.getNodeValue(position_node_index)->assignNodeValue(src_operation_index, src_new_link);
}



void IMAGE_PROCESS_HANDLER::setInputNodeValue(const unsigned int src_node_index, const unsigned int src_operation_index, NODE_SCALAR<char*>* src_new_link)
{
	if (src_node_index >= nodes_operation_count)
	{
		return;
	}

	unsigned int position_node_index = 0;
	while (nodes_operation_position_list.getNodeValue(position_node_index) != src_node_index)
	{
		position_node_index++;
	}

	if (src_operation_index >= nodes_operation_list.getNodeValue(position_node_index)->getInputStringNodesRequired())
	{
		return;
	}

	nodes_operation_list.getNodeValue(position_node_index)->assignNodeValue(src_operation_index, src_new_link);
}



void IMAGE_PROCESS_HANDLER::setInputNodeToImageOperation(const unsigned int src_node_index, const unsigned int src_operation_index, NODE_SCALAR<double>* src_new_link)
{
	if (src_node_index >= image_operation_count)
	{
		return;
	}

	unsigned int position_node_index = 0;
	while (image_operation_position_list.getNodeValue(position_node_index) != src_node_index)
	{
		position_node_index++;
	}

	if (src_operation_index >= image_operation_list.getNodeValue(position_node_index)->getInputNumericNodesRequired())
	{
		return;
	}

	image_operation_list.getNodeValue(position_node_index)->assignNodeValue(src_operation_index, src_new_link);
}



void IMAGE_PROCESS_HANDLER::setInputNodeToImageOperation(const unsigned int src_node_index, const unsigned int src_operation_index, NODE_SCALAR<char*>* src_new_link)
{
	if (src_node_index >= image_operation_count)
	{
		return;
	}

	unsigned int position_node_index = 0;
	while (image_operation_position_list.getNodeValue(position_node_index) != src_node_index)
	{
		position_node_index++;
	}

	if (src_operation_index >= image_operation_list.getNodeValue(position_node_index)->getInputStringNodesRequired())
	{
		return;
	}

	image_operation_list.getNodeValue(position_node_index)->assignNodeValue(src_operation_index, src_new_link);
}


void IMAGE_PROCESS_HANDLER::setInputNodeToImageOperation(const unsigned int src_node_index, const unsigned int src_operation_index, IMAGE_OPERATION * src_new_link)
{
	if (src_node_index >= image_operation_count)
	{
		return;
	}

	unsigned int position_node_index = 0;
	while (image_operation_position_list.getNodeValue(position_node_index) != src_node_index)
	{
		position_node_index++;
	}

	switch (src_operation_index)
	{
	case 0:
		image_operation_list.getNodeValue(position_node_index)->setInputOperationA(src_new_link);
		break;

	case 1:
		image_operation_list.getNodeValue(position_node_index)->setInputOperationB(src_new_link);
		break;

	default:
		break;
	}
}


void IMAGE_PROCESS_HANDLER::setInputNodeToImageScalarOperation(const unsigned int src_node_index, const unsigned int src_operation_index, NODE_SCALAR<double>* src_new_link)
{
	if (src_node_index >= image_scalar_operation_count)
	{
		return;
	}

	unsigned int position_node_index = 0;
	while (image_scalar_operation_position_list.getNodeValue(position_node_index) != src_node_index)
	{
		position_node_index++;
	}

	if (src_operation_index >= image_scalar_operation_list.getNodeValue(position_node_index)->getInputNumericNodesRequired())
	{
		return;
	}

	image_scalar_operation_list.getNodeValue(position_node_index)->assignNodeValue(src_operation_index, src_new_link);
}



void IMAGE_PROCESS_HANDLER::setInputNodeToImageScalarOperation(const unsigned int src_node_index, const unsigned int src_operation_index, NODE_SCALAR<char*>* src_new_link)
{
	if (src_node_index >= image_scalar_operation_count)
	{
		return;
	}

	unsigned int position_node_index = 0;
	while (image_scalar_operation_position_list.getNodeValue(position_node_index) != src_node_index)
	{
		position_node_index++;
	}

	if (src_operation_index >= image_scalar_operation_list.getNodeValue(position_node_index)->getInputStringNodesRequired())
	{
		return;
	}

	image_scalar_operation_list.getNodeValue(position_node_index)->assignNodeValue(src_operation_index, src_new_link);
}



void IMAGE_PROCESS_HANDLER::setInputNodeToImageScalarOperation(const unsigned int src_node_index, const unsigned int src_operation_index, IMAGE_OPERATION* src_new_link)
{
	if (src_node_index >= image_scalar_operation_count)
	{
		return;
	}

	unsigned int position_node_index = 0;
	while (image_scalar_operation_position_list.getNodeValue(position_node_index) != src_node_index)
	{
		position_node_index++;
	}

	if (src_operation_index > 0)
	{
		return;
	}

	image_scalar_operation_list.getNodeValue(position_node_index)->setInputOperation(src_new_link);
}
