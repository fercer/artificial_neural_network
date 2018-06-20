#include "image_process_handler_class.h"

IMAGE_PROCESS_HANDLER::IMAGE_PROCESS_HANDLER()
{
	sprintf(filename, "NOT-DEFINED");

	chain_head.lenght = 0;
	sprintf(chain_head.string_value, "HEAD NODE");
	chain_head.previous_link = NULL;
	chain_head.link_node_type = NT_HEADER;
	chain_head.next_link = NULL;

	string_node_scalar_count = 0;
	numeric_node_scalar_count = 0;
	nodes_operation_count = 0;
	image_operation_count = 0;
	image_scalar_operation_count = 0;

	chain_tail = &chain_head;
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
	CHAIN_LINK * next_chain_link = chain_head.next_link;
	CHAIN_LINK * current_chain_link;

	while (next_chain_link)
	{
		current_chain_link = next_chain_link;
		next_chain_link = current_chain_link->next_link;

		free(current_chain_link);
	}
}


void IMAGE_PROCESS_HANDLER::setFilename(const char * src_filename)
{
	strcpy(filename, src_filename);
}


void IMAGE_PROCESS_HANDLER::loadProcess()
{
	loadProcessFile();
}


void IMAGE_PROCESS_HANDLER::loadProcessFile()
{
	if (strcmp(filename, "NOT-DEFINED") == 0)
	{
		printf("<<Error: The filename has not been defined yet>>\n");
		return;
	}

	FILE * fp_process = fopen(filename, "r");

	if (!fp_process)
	{
		printf("<<Error: The filename \'%s\' could not be opened>>\n", filename);
		return;
	}

	char read_line[512];
	/* Read the first line that should define the xml file: */
	fgets(read_line, 512, fp_process);

	/* Start reading the file: */
	unsigned int starting_position;
	char * starting_character;
	char * ending_character;
	char * current_character_pointer;
	char * last_character_pointer;
	LINK_TYPE current_type;

	fgets(read_line, 512, fp_process);

	while (1)
	{
		last_character_pointer = strpbrk(read_line, "<\"");
		while (last_character_pointer && (*last_character_pointer != '>') && (*last_character_pointer != '\n') && (*last_character_pointer != '\0'))
		{
			starting_position = 1;

			switch ((int)*last_character_pointer)
			{
			case (int)'\"':
				current_character_pointer = strpbrk(last_character_pointer + 1, "\"");
				starting_character = last_character_pointer + 1;
				ending_character = current_character_pointer - 1;
				current_type = NT_VALUE;
				break;

			case (int)'<':
				if (*(last_character_pointer + 1) == '/')
				{
					current_character_pointer = strpbrk(last_character_pointer + 1, ">");
					starting_character = last_character_pointer + 2;
					ending_character = current_character_pointer - 1;
					current_type = NT_TAIL;
				}
				else
				{
					current_character_pointer = strpbrk(last_character_pointer + 1, "> ");
					starting_character = last_character_pointer + 1;
					ending_character = current_character_pointer - 1;
					current_type = NT_HEADER;
				}
				break;

			case (int)'=':
				current_character_pointer = strpbrk(last_character_pointer, " \"");
				starting_character = last_character_pointer;
				ending_character = starting_character;
				starting_position = 0;
				current_type = NT_EQUAL_SIGN;
				break;

			case (int)' ':
				current_character_pointer = strpbrk(last_character_pointer + 1, " =");
				starting_character = last_character_pointer + 1;
				ending_character = current_character_pointer - 1;
				starting_position = 0;
				current_type = NT_VARIABLE;
				break;

			default:
				current_character_pointer = strpbrk(last_character_pointer, " =");
				starting_character = last_character_pointer;
				ending_character = current_character_pointer - 1;
				starting_position = 0;
				current_type = NT_VARIABLE;
				break;
			}
			
			
			/* Add a new link to the chain: */
			chain_tail->next_link = (CHAIN_LINK*)malloc(sizeof(CHAIN_LINK));
			chain_tail->next_link->previous_link = chain_tail;
			chain_tail = chain_tail->next_link;
			chain_tail->next_link = NULL;
			chain_tail->lenght = ending_character - starting_character + 1;
			chain_tail->link_node_type = current_type;

			memcpy(chain_tail->string_value, starting_character, chain_tail->lenght * sizeof(char));
			*(chain_tail->string_value + chain_tail->lenght) = '\0';
			last_character_pointer = current_character_pointer + starting_position;
		}

		fgets(read_line, 512, fp_process);

		if (feof(fp_process))
		{
			break;
		}
	}

	fclose(fp_process);

	/* Dump the read nodes into an array keeping the variable-value relation */
	CHAIN_LINK * next_chain_link = chain_head.next_link;
	CHAIN_LINK * current_chain_link;
	
	if (!next_chain_link)
	{
		printf("<<Error: The chain is empty>>\n");
		return;
	}

	/* Look for the Image Processing nodes: */
	while (next_chain_link)
	{
		current_chain_link = next_chain_link;
		next_chain_link = current_chain_link->next_link;

		if (strcmp(current_chain_link->string_value, "ImageProcessing") && (current_chain_link->link_node_type != NT_HEADER))
		{
			break;
		}
	}

	/* Read all the Node values */
	while (next_chain_link)
	{
		current_chain_link = next_chain_link;
		next_chain_link = current_chain_link->next_link;

		/* Verify the type of node: */
		if (current_chain_link->link_node_type != NT_HEADER)
		{
			continue;
		}

		char current_header[64];
		strcpy(current_header, current_chain_link->string_value);

		GENERIC_LIST<NODE_SCALAR<char*>*> string_node_scalar_list;
		GENERIC_LIST<NODE_SCALAR<double>*> numeric_node_scalar_list;


		if (strcmp(current_chain_link->string_value, "ImageOperation") == 0)
		{
			GENERIC_LIST<IMAGE_OPERATION*> image_operation_list;
		}

		if (strcmp(current_chain_link->string_value, "ImageScalarOperation") == 0)
		{
			GENERIC_LIST<IMAGE_SCALAR_OPERATION*> image_scalar_operation_list;
		}

		if (strcmp(current_chain_link->string_value, "NodeOperationValue") == 0)
		{
			GENERIC_LIST<NODES_SCALAR_OPERATION*> nodes_operation_list;
		}
	}
}


IMAGE_PROCESS_HANDLER::CHAIN_LINK * IMAGE_PROCESS_HANDLER::dumpLinkToList(CHAIN_LINK * src_root)
{
	CHAIN_LINK * next_link = src_root;
	CHAIN_LINK * current_link;
	char current_header[128];
	strcpy(current_header, src_root->string_value);

	unsigned int current_link_position;
	char current_link_name[128];
	
	/* All the possible operations */
	OPERATION_TYPE current_link_operation;
	
	while (next_link && !(strcmp(next_link->string_value, current_header) == 0 && next_link->link_node_type == NT_TAIL))
	{
		current_link = next_link;
		if (current_link->link_node_type == NT_HEADER)
		{
			/* At this point, all the varaibles on the header should have been read, 
			so them are dump into the corresponding operation.
			*/
			// Using temporal lists of nodes, and then adding them to the newlly generated objects.
			switch (current_link_operation)
			{
			case NV_NUMERIC:
			{
				NODE_SCALAR<double> * new_node_scalar = (NODE_SCALAR<double>*) malloc(sizeof(NODE_SCALAR<double>));
				new_node_scalar->setNodeScalarName(current_link_name);
				numeric_node_scalar_count = numeric_node_scalar_list.assignNodeValue(numeric_node_scalar_count, new_node_scalar);
				break;
			}
			case NV_STRING:
			{
				NODE_SCALAR<char*> * new_node_scalar = (NODE_SCALAR<char*>*) malloc(sizeof(NODE_SCALAR<char*>));
				new_node_scalar->setNodeScalarName(current_link_name);
				string_node_scalar_count = string_node_scalar_list.assignNodeValue(string_node_scalar_count, new_node_scalar);
				break;
			}
			case NOV_DIV:
			{
				SCALAR_DIV_NODES_OPERATION * new_nodes_operation = (SCALAR_DIV_NODES_OPERATION*)malloc(sizeof(SCALAR_DIV_NODES_OPERATION));
				new_nodes_operation->setNodeScalarName(current_link_name);
				nodes_operation_count = nodes_operation_list.assignNodeValue(nodes_operation_count, new_nodes_operation);
				break;
			}
			case NOV_MULT:
			{
				SCALAR_MULT_NODES_OPERATION * new_nodes_operation = (SCALAR_MULT_NODES_OPERATION*)malloc(sizeof(SCALAR_MULT_NODES_OPERATION));
				new_nodes_operation->setNodeScalarName(current_link_name);
				nodes_operation_count = nodes_operation_list.assignNodeValue(nodes_operation_count, new_nodes_operation);
				break;
			}
			case NOV_SQRT:
			{
				SCALAR_SQRT_NODES_OPERATION * new_nodes_operation = (SCALAR_SQRT_NODES_OPERATION*)malloc(sizeof(SCALAR_SQRT_NODES_OPERATION));
				new_nodes_operation->setNodeScalarName(current_link_name);
				nodes_operation_count = nodes_operation_list.assignNodeValue(nodes_operation_count, new_nodes_operation);
				break;
			}
			case NOV_SUM:
			{
				SCALAR_SUM_NODES_OPERATION * new_nodes_operation = (SCALAR_SUM_NODES_OPERATION*)malloc(sizeof(SCALAR_SUM_NODES_OPERATION));
				new_nodes_operation->setNodeScalarName(current_link_name);
				nodes_operation_count = nodes_operation_list.assignNodeValue(nodes_operation_count, new_nodes_operation);
				break;
			}
			case NOV_SUB:
			{
				SCALAR_SUB_NODES_OPERATION * new_nodes_operation = (SCALAR_SUB_NODES_OPERATION*)malloc(sizeof(SCALAR_SUB_NODES_OPERATION));
				new_nodes_operation->setNodeScalarName(current_link_name);
				nodes_operation_count = nodes_operation_list.assignNodeValue(nodes_operation_count, new_nodes_operation);
				break;
			}
			case IO_DIV:
			{
				DIV_IMAGE_OPERATION * new_image_operation = (DIV_IMAGE_OPERATION*)malloc(sizeof(DIV_IMAGE_OPERATION));
				new_image_operation->setImageName(current_link_name);
				image_operation_count = image_operation_list.assignNodeValue(image_operation_count, new_image_operation);
				break;
			}
			case IO_FILTER:
			{
				FILTER_IMAGE_OPERATION * new_image_operation = (FILTER_IMAGE_OPERATION*)malloc(sizeof(FILTER_IMAGE_OPERATION));
				new_image_operation->setImageName(current_link_name);
				image_operation_count = image_operation_list.assignNodeValue(image_operation_count, new_image_operation);
				break;
			}
			case IO_LOAD:
			{
				LOAD_IMAGE_OPERATION * new_image_operation = (LOAD_IMAGE_OPERATION*)malloc(sizeof(LOAD_IMAGE_OPERATION));
				new_image_operation->setImageName(current_link_name);
				image_operation_count = image_operation_list.assignNodeValue(image_operation_count, new_image_operation);
				break;
			}
			case IO_MULT:
			{
				MULT_IMAGE_OPERATION * new_image_operation = (MULT_IMAGE_OPERATION*)malloc(sizeof(MULT_IMAGE_OPERATION));
				new_image_operation->setImageName(current_link_name);
				image_operation_count = image_operation_list.assignNodeValue(image_operation_count, new_image_operation);
				break;
			}
			case IO_ONES:
			{
				ONES_IMAGE_OPERATION * new_image_operation = (ONES_IMAGE_OPERATION*)malloc(sizeof(ONES_IMAGE_OPERATION));
				new_image_operation->setImageName(current_link_name);
				image_operation_count = image_operation_list.assignNodeValue(image_operation_count, new_image_operation);
				break;
			}
			case IO_ROTATE:
			{
				ROTATE_IMAGE_OPERATION * new_image_operation = (ROTATE_IMAGE_OPERATION*)malloc(sizeof(ROTATE_IMAGE_OPERATION));
				new_image_operation->setImageName(current_link_name);
				image_operation_count = image_operation_list.assignNodeValue(image_operation_count, new_image_operation);
				break;
			}
			case IO_SAVE:
			{
				SAVE_IMAGE_OPERATION * new_image_operation = (SAVE_IMAGE_OPERATION*)malloc(sizeof(SAVE_IMAGE_OPERATION));
				new_image_operation->setImageName(current_link_name);
				image_operation_count = image_operation_list.assignNodeValue(image_operation_count, new_image_operation);
				break;
			}
			case IO_SQRT:
			{
				SQROOT_IMAGE_OPERATION * new_image_operation = (SQROOT_IMAGE_OPERATION*)malloc(sizeof(SQROOT_IMAGE_OPERATION));
				new_image_operation->setImageName(current_link_name);
				image_operation_count = image_operation_list.assignNodeValue(image_operation_count, new_image_operation);
				break;
			}
			case IO_SUM:
			{
				SUM_IMAGE_OPERATION * new_image_operation = (SUM_IMAGE_OPERATION*)malloc(sizeof(SUM_IMAGE_OPERATION));
				new_image_operation->setImageName(current_link_name);
				image_operation_count = image_operation_list.assignNodeValue(image_operation_count, new_image_operation);
				break;
			}
			case IO_TRANSLATE:
			{
				TRANSLATE_IMAGE_OPERATION * new_image_operation = (TRANSLATE_IMAGE_OPERATION*)malloc(sizeof(TRANSLATE_IMAGE_OPERATION));
				new_image_operation->setImageName(current_link_name);
				image_operation_count = image_operation_list.assignNodeValue(image_operation_count, new_image_operation);
				break;
			}
			case ISO_HEIGHT:
			{
				HEIGHT_IMAGE_SCALAR_OPERATION * new_image_scalar_operation = (HEIGHT_IMAGE_SCALAR_OPERATION*)malloc(sizeof(HEIGHT_IMAGE_SCALAR_OPERATION));
				new_image_scalar_operation->setNodeScalarName(current_link_name);
				image_scalar_operation_count = image_scalar_operation_list.assignNodeValue(image_scalar_operation_count, new_image_scalar_operation);
				break;
			}
			case ISO_MAX:
			{
				MAX_IMAGE_SCALAR_OPERATION * new_image_scalar_operation = (MAX_IMAGE_SCALAR_OPERATION*)malloc(sizeof(MAX_IMAGE_SCALAR_OPERATION));
				new_image_scalar_operation->setNodeScalarName(current_link_name);
				image_scalar_operation_count = image_scalar_operation_list.assignNodeValue(image_scalar_operation_count, new_image_scalar_operation);
				break;
			}
			case ISO_MIN:
			{
				MIN_IMAGE_SCALAR_OPERATION * new_image_scalar_operation = (MIN_IMAGE_SCALAR_OPERATION*)malloc(sizeof(MIN_IMAGE_SCALAR_OPERATION));
				new_image_scalar_operation->setNodeScalarName(current_link_name);
				image_scalar_operation_count = image_scalar_operation_list.assignNodeValue(image_scalar_operation_count, new_image_scalar_operation);
				break;
			}
			case ISO_SUM:
			{
				SUM_IMAGE_SCALAR_OPERATION * new_image_scalar_operation = (SUM_IMAGE_SCALAR_OPERATION*)malloc(sizeof(SUM_IMAGE_SCALAR_OPERATION));
				new_image_scalar_operation->setNodeScalarName(current_link_name);
				image_scalar_operation_count = image_scalar_operation_list.assignNodeValue(image_scalar_operation_count, new_image_scalar_operation);
				break;
			}
			case ISO_WIDTH:
			{
				WIDTH_IMAGE_SCALAR_OPERATION * new_image_scalar_operation = (WIDTH_IMAGE_SCALAR_OPERATION*)malloc(sizeof(WIDTH_IMAGE_SCALAR_OPERATION));
				new_image_scalar_operation->setNodeScalarName(current_link_name);
				image_scalar_operation_count = image_scalar_operation_list.assignNodeValue(image_scalar_operation_count, new_image_scalar_operation);
				break;
			}
			default:
				printf("<<Error: The operation type is not deffined \'%s\'>>\n", current_header);
				break;
			}
		}
		else
		{
			if (strcmp(current_link->string_value, "position") == 0)
			{
				current_link = next_link;
				next_link = current_link->next_link;
				
				/* Look for the corresponding value: */
				while (next_link)
				{
					current_link = next_link;
					next_link = current_link->next_link;

					if (strcmp(current_link->string_value, "=") != 0)
					{
						break;
					}
				}

				current_link_position = atoi(current_link->string_value);
			}

			if (strcmp(current_link->string_value, "type") == 0)
			{
				current_link = next_link;
				next_link = current_link->next_link;

				/* Look for the corresponding type: */
				while (next_link)
				{
					current_link = next_link;
					next_link = current_link->next_link;

					if (strcmp(current_link->string_value, "=") != 0)
					{
						break;
					}
				}

				if (strcmp(current_link->string_value, "NV_NUMERIC") == 0)
				{
					current_link_operation = NV_NUMERIC;
				}
				else if (strcmp(current_link->string_value, "NV_STRING") == 0)
				{
					current_link_operation = NV_STRING;
				}
				else if (strcmp(current_link->string_value, "NOV_SUM") == 0)
				{
					current_link_operation = NOV_SUM;
				}
				else if (strcmp(current_link->string_value, "NOV_SUB") == 0)
				{
					current_link_operation = NOV_SUB;
				}
				else if (strcmp(current_link->string_value, "NOV_MULT") == 0)
				{
					current_link_operation = NOV_MULT;
				}
				else if (strcmp(current_link->string_value, "NOV_DIV") == 0)
				{
					current_link_operation = NOV_DIV;
				}
				else if (strcmp(current_link->string_value, "NOV_SQRT") == 0)
				{
					current_link_operation = NOV_SQRT;
				}
				else if (strcmp(current_link->string_value, "IO_DIV") == 0)
				{
					current_link_operation = IO_DIV;
				}
				else if (strcmp(current_link->string_value, "IO_MULT") == 0)
				{
					current_link_operation = IO_MULT;
				}
				else if (strcmp(current_link->string_value, "IO_SUM") == 0)
				{
					current_link_operation = IO_SUM;
				}
				else if (strcmp(current_link->string_value, "IO_LOAD") == 0)
				{
					current_link_operation = IO_LOAD;
				}
				else if (strcmp(current_link->string_value, "IO_SAVE") == 0)
				{
					current_link_operation = IO_SAVE;
				}
				else if (strcmp(current_link->string_value, "IO_FILTER") == 0)
				{
					current_link_operation = IO_FILTER;
				}
				else if (strcmp(current_link->string_value, "IO_ROTATE") == 0)
				{
					current_link_operation = IO_ROTATE;
				}
				else if (strcmp(current_link->string_value, "IO_TRANSLATE") == 0)
				{
					current_link_operation = IO_TRANSLATE;
				}
				else if (strcmp(current_link->string_value, "IO_SQRT") == 0)
				{
					current_link_operation = IO_SQRT;
				}
				else if (strcmp(current_link->string_value, "IO_ONES") == 0)
				{
					current_link_operation = IO_ONES;
				}
				else if (strcmp(current_link->string_value, "ISO_SUM") == 0)
				{
					current_link_operation = ISO_SUM;
				}
				else if (strcmp(current_link->string_value, "ISO_MAX") == 0)
				{
					current_link_operation = ISO_MAX;
				}
				else if (strcmp(current_link->string_value, "ISO_MIN") == 0)
				{
					current_link_operation = ISO_MIN;
				}
				else if (strcmp(current_link->string_value, "ISO_WIDTH") == 0)
				{
					current_link_operation = ISO_WIDTH;
				}
				else if (strcmp(current_link->string_value, "ISO_HEIGHT") == 0)
				{
					current_link_operation = ISO_HEIGHT;
				}
				else
				{
					printf("<<Error: Oepration type not defined \'%s\'>>\n", current_link->string_value);
				}
			}

			if (strcmp(current_link->string_value, "name") == 0)
			{
				current_link = next_link;
				next_link = current_link->next_link;

				/* Look for the corresponding value: */
				while (next_link)
				{
					current_link = next_link;
					next_link = current_link->next_link;

					if (strcmp(current_link->string_value, "=") != 0)
					{
						break;
					}
				}

				strcpy(current_link_name, current_link->string_value);
			}
		}
	}

	return NULL;
}



IMAGE_PROCESS_HANDLER::CHAIN_LINK * IMAGE_PROCESS_HANDLER::dumpLinkVariableToList(CHAIN_LINK * src_root, GENERIC_LIST<NODE_SCALAR<double>*> * src_input_nodes_list)
{
}



IMAGE_PROCESS_HANDLER::CHAIN_LINK * IMAGE_PROCESS_HANDLER::dumpLinkVariableToList(CHAIN_LINK * src_root, GENERIC_LIST<NODE_SCALAR<char*>*> * src_input_nodes_list)
{
}



IMAGE_PROCESS_HANDLER::CHAIN_LINK * IMAGE_PROCESS_HANDLER::dumpLinkVariableToList(CHAIN_LINK * src_root, GENERIC_LIST<IMAGE_OPERATION*> * src_input_operation_list)
{
}