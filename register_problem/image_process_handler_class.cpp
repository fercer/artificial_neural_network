#include "image_process_handler_class.h"

IMAGE_PROCESS_HANDLER::IMAGE_PROCESS_HANDLER()
{
	sprintf(filename, "NOT-DEFINED");

	chain_head.lenght = 0;
	sprintf(chain_head.string_value, "HEAD NODE");
	chain_head.previous_link = NULL;
	chain_head.link_node_type = NT_HEADER;
	chain_head.next_link = NULL;

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


void IMAGE_PROCESS_HANDLER::dumpLinkToList(CHAIN_LINK * src_root)
{

}