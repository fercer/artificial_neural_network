#include "xml_handler_class.h"



XML_HANDLER::XML_HANDLER()
{
	filename = NULL;
	filename_has_changed = false;
	tree_root = NULL;
}



XML_HANDLER::XML_HANDLER(const XML_HANDLER & src_xml_handler)
{
}



XML_HANDLER XML_HANDLER::operator=(const XML_HANDLER & src_xml_handler)
{
	return XML_HANDLER();
}



XML_HANDLER::~XML_HANDLER()
{
	free(filename);

	deallocateTree(tree_root);
}



void XML_HANDLER::setFilename(const char * src_filename)
{
	if (filename)
	{
		free(filename);
	}

	size_t filename_length = strlen(src_filename) + 1;
	filename = (char*)malloc(filename_length);
	strcpy(filename, src_filename);

	filename_has_changed = true;
}



void XML_HANDLER::loadXMLFile()
{
	if (!filename_has_changed)
	{
		return;
	}

	if (!filename)
	{
		printf("<<Error: The filename has not been defined yet>>\n");
		return;
	}

	FILE * fp_xml_file = fopen(filename, "r");

	if (!fp_xml_file)
	{
		printf("<<Error: The filename \'%s\' could not be opened>>\n", filename);
		return;
	}

	const unsigned int max_read_characters = 1023;
	char read_line[max_read_characters + 1];
	/* Read the first line that should define the xml file: */
	fgets(read_line, max_read_characters, fp_xml_file);
	printf("XML file definition: \'%s\'\n", read_line);

	/* Read the first line that should define the xml file: */	
	deallocateTree(tree_root);
	tree_root = new XML_NODE_LEAF;
	
	dumpLeafNode(fp_xml_file, tree_root, read_line, max_read_characters);

	fclose(fp_xml_file);

	filename_has_changed = false;
}

XML_NODE_LEAF * XML_HANDLER::getRoot()
{
	return tree_root->getFirstChild();
}



bool XML_HANDLER::dumpLeafNode(FILE * src_fp_xml_file, XML_NODE_LEAF * src_parent_leaf, char * src_read_line, const unsigned int src_max_read_characters)
{
	// Look for the following identifier:
	const XML_NODE_LEAF::NODE_TYPE following_identifier_type = getFollowingIdentifier(src_fp_xml_file, src_read_line, src_max_read_characters);

	switch (following_identifier_type)
	{
	case XML_NODE_LEAF::NT_HEADER:
	{
		XML_NODE_LEAF * new_header_node = new XML_NODE_LEAF;
		new_header_node->assignType(XML_NODE_LEAF::NT_HEADER);
		new_header_node->assignIdentifier(src_read_line);
		src_parent_leaf->assignChild(new_header_node);

		while (dumpLeafNode(src_fp_xml_file, new_header_node, src_read_line, src_max_read_characters))
		{
			// Loop until the corresponding tail is found ...
		}

		return true;
		break;
	}
	case XML_NODE_LEAF::NT_ATTRIBUTE:
	{
		XML_NODE_LEAF * new_attribute_node = new XML_NODE_LEAF;
		new_attribute_node->assignType(XML_NODE_LEAF::NT_ATTRIBUTE);
		new_attribute_node->assignIdentifier(src_read_line);
		src_parent_leaf->assignChild(new_attribute_node);

		/* Look for the current attribute's value */
		dumpLeafNode(src_fp_xml_file, new_attribute_node, src_read_line, src_max_read_characters);

		/* Keep the search for attributes and children nodes using the current parent */
		return true;
		break;
	}
	case XML_NODE_LEAF::NT_VALUE:
		src_parent_leaf->assignValue(src_read_line);

		if (src_parent_leaf->getType() == XML_NODE_LEAF::NT_HEADER)
		{
			/* Continue the search using the current parent */
			return true;
		}
		else
		{
			/* Stop the search and return to the current parent's parent */
			return false;
		}

		break;

	case XML_NODE_LEAF::NT_CLOSER:
		/* Let the search continue using the current node as parent */
		return true;
		break;

	case XML_NODE_LEAF::NT_TAIL:
		/* Stop the search and return to the current parent's parent */
		return false;
		break;
	case XML_NODE_LEAF::NT_UNDEFINED:
		break;
	default:
		printf("<<Error: An undefined identifier was found>>\n");
		break;
	}

	return false;
}



XML_NODE_LEAF::NODE_TYPE XML_HANDLER::getFollowingIdentifier(FILE * src_fp_xml_file, char * src_read_line, const unsigned int src_max_read_characters)
{
	char first_character = fgetc(src_fp_xml_file);
	/* Look for the following blank space, or new line, until a different character is found.
	If the found character is an '=', look again for another character:
	*/
	while (1)
	{
		// If the end of the file is found before the searched node is found, end the recursivity.
		if (first_character == EOF)
		{
			return XML_NODE_LEAF::NT_UNDEFINED;
		}
		else if (first_character != ' ' && first_character != '\n')
		{
			break;
		}

		first_character = fgetc(src_fp_xml_file);
	}
	*src_read_line = first_character;

	if (first_character == '>')
	{
		*(src_read_line) = '\0';
		return XML_NODE_LEAF::NT_CLOSER;
	}

	const char second_delimiter = fgetc(src_fp_xml_file);
	*(src_read_line + 1) = second_delimiter;

	unsigned int read_characters_count = 2;
	char identifier_delimiter = second_delimiter;

	// Look for the ending of the current identifier:
	if (first_character == '\"')
	{
		bool continue_reading = true;
		while (continue_reading && read_characters_count < src_max_read_characters)
		{
			// If the end of the file is found before the searched node is found, end the recursivity.
			switch((int)identifier_delimiter)
			{
			case (int)'\"':
				continue_reading = false;
				break;
			case EOF:
				return XML_NODE_LEAF::NT_UNDEFINED;
				break;
			default:
				identifier_delimiter = fgetc(src_fp_xml_file);
				*(src_read_line + read_characters_count) = identifier_delimiter;
				read_characters_count++;
				break;
			}
		}
	}
	else
	{
		bool continue_reading = true;
		while (continue_reading && read_characters_count < src_max_read_characters)
		{
			// If the end of the file is found before the searched node is found, end the recursivity.
			switch ((int)identifier_delimiter)
			{
			case (int)' ':
			case (int)'>':
			case (int)'=':
				continue_reading = false;
				break;
			case EOF:
				return XML_NODE_LEAF::NT_UNDEFINED;
				break;
			default:
				identifier_delimiter = fgetc(src_fp_xml_file);
				*(src_read_line + read_characters_count) = identifier_delimiter;
				read_characters_count++;
				break;
			}
		}
	}

	*(src_read_line + read_characters_count) = '\0';

	/* Define the type of the read identifier */
	if (first_character == '<' && second_delimiter == '/')
	{
		memmove(src_read_line, src_read_line + 2, read_characters_count - 3);
		*(src_read_line + read_characters_count - 3) = '\0';
		return XML_NODE_LEAF::NT_TAIL;
	}
	else if (first_character == '<' && second_delimiter != '/')
	{
		memmove(src_read_line, src_read_line + 1, read_characters_count - 2);
		*(src_read_line + read_characters_count - 2) = '\0';
		return XML_NODE_LEAF::NT_HEADER;
	}
	else if (first_character == '\"')
	{
		memmove(src_read_line, src_read_line + 1, read_characters_count - 2);
		*(src_read_line + read_characters_count - 2) = '\0';
		return XML_NODE_LEAF::NT_VALUE;
	}
	else
	{
		memmove(src_read_line, src_read_line, read_characters_count - 1);
		*(src_read_line + read_characters_count - 1) = '\0';
		return XML_NODE_LEAF::NT_ATTRIBUTE;
	}

	return XML_NODE_LEAF::NT_UNDEFINED;
}



XML_NODE_LEAF::XML_NODE_LEAF()
{
	link_type = NT_UNDEFINED;
	left_branch = NULL;
	right_branch = NULL;
	node_children = NULL;
	link_name = NULL;
	link_value = NULL;

	node_children_array = NULL;
	children_count = 0;
	last_children_count = 0;
	previous_sibling_nodes_count = 0;
	sibling_nodes_count = 0;
}



XML_NODE_LEAF::XML_NODE_LEAF(const char * src_identifier, NODE_TYPE src_type)
{
	link_type = src_type;
	left_branch = NULL;
	right_branch = NULL;
	node_children = NULL;
	link_name = NULL;
	link_value = NULL;

	node_children_array = NULL;
	children_count = 0;
	last_children_count = 0;
	previous_sibling_nodes_count = 0;
	sibling_nodes_count = 0;

	assignIdentifier(src_identifier);
}



XML_NODE_LEAF::XML_NODE_LEAF(const char * src_identifier, const char * src_value, NODE_TYPE src_type)
{
	link_type = src_type;
	left_branch = NULL;
	right_branch = NULL;
	node_children = NULL;
	link_name = NULL;
	link_value = NULL;

	node_children_array = NULL;
	children_count = 0;
	last_children_count = 0;
	previous_sibling_nodes_count = 0;
	sibling_nodes_count = 0;

	assignIdentifier(src_identifier);
	assignValue(src_value);
}



XML_NODE_LEAF::XML_NODE_LEAF(const XML_NODE_LEAF & src_node_leaf)
{
	this->link_type = src_node_leaf.link_type;
	this->left_branch = src_node_leaf.left_branch;
	this->right_branch = src_node_leaf.right_branch;
	this->node_children = src_node_leaf.node_children;

	this->assignIdentifier(src_node_leaf.link_name);
	this->assignValue(src_node_leaf.link_value);


	this->node_children_array = NULL;
	this->children_count = src_node_leaf.children_count;
	this->last_children_count = 0;
	this->previous_sibling_nodes_count = src_node_leaf.previous_sibling_nodes_count;
	this->sibling_nodes_count = src_node_leaf.sibling_nodes_count;
}



XML_NODE_LEAF XML_NODE_LEAF::operator=(const XML_NODE_LEAF & src_node_leaf)
{
	if (this != &src_node_leaf)
	{
		this->link_type = src_node_leaf.link_type;
		this->left_branch = src_node_leaf.left_branch;
		this->right_branch = src_node_leaf.right_branch;
		this->node_children = src_node_leaf.node_children;

		this->assignIdentifier(src_node_leaf.link_name);
		this->assignValue(src_node_leaf.link_value);

		this->node_children_array = NULL;
		this->children_count = src_node_leaf.children_count;
		this->last_children_count = 0;
		this->previous_sibling_nodes_count = src_node_leaf.previous_sibling_nodes_count;
		this->sibling_nodes_count = src_node_leaf.sibling_nodes_count;
	}

	return *this;
}



XML_NODE_LEAF::~XML_NODE_LEAF()
{
	if (link_name)
	{
		free(link_name);
	}

	if (link_value)
	{
		free(link_value);
	}
}



void deallocateTree(XML_NODE_LEAF * src_tree_root)
{
	if (!src_tree_root)
	{
		return;
	}

	deallocateTree(src_tree_root->node_children);
	deallocateTree(src_tree_root->left_branch);
	deallocateTree(src_tree_root->right_branch);

	delete src_tree_root;
}



void XML_NODE_LEAF::assignChild(XML_NODE_LEAF * src_new_child)
{
	children_count++;
	if (node_children)
	{
		node_children->assignSibling(src_new_child);
	}
	else
	{
		node_children = src_new_child;
	}
}



void XML_NODE_LEAF::assignSibling(XML_NODE_LEAF * src_new_sibling)
{
	sibling_nodes_count++;
	if (strcmp(link_name, src_new_sibling->link_name) <= 0)
	{
		previous_sibling_nodes_count++;
		if (left_branch)
		{
			left_branch->assignSibling(src_new_sibling);
		}
		else
		{
			left_branch = src_new_sibling;
		}
	}
	else
	{
		if (right_branch)
		{
			right_branch->assignSibling(src_new_sibling);
		}
		else
		{
			right_branch = src_new_sibling;
		}
	}
}



void XML_NODE_LEAF::assignSiblingToArray(XML_NODE_LEAF ** src_nodes_array, const unsigned int src_previous_sibling_nodes_count)
{
	*(src_nodes_array + src_previous_sibling_nodes_count + previous_sibling_nodes_count) = this;

	if (left_branch)
	{
		left_branch->assignSiblingToArray(src_nodes_array, src_previous_sibling_nodes_count);
	}
	
	if (right_branch)
	{
		right_branch->assignSiblingToArray(src_nodes_array, src_previous_sibling_nodes_count + previous_sibling_nodes_count + 1);
	}
}



void XML_NODE_LEAF::dumpChildrenListToArray()
{
	if (last_children_count == children_count)
	{
		return;
	}

	if (node_children_array)
	{
		free(node_children_array);
	}

	last_children_count = children_count;
	node_children_array = (XML_NODE_LEAF**)malloc(children_count * sizeof(XML_NODE_LEAF*));
	node_children->assignSiblingToArray(node_children_array, 0);
}



void XML_NODE_LEAF::assignIdentifier(const char * src_identifier)
{
	if (link_name) 
	{
		free(link_name);
	}

	link_name = (char*)calloc(strlen(src_identifier) + 1, sizeof(char));
	strcpy(link_name, src_identifier);
}



void XML_NODE_LEAF::assignValue(const char * src_value)
{
	if (link_value)
	{
		free(link_value);
	}

	link_value = (char*)calloc(strlen(src_value) + 1, sizeof(char));
	strcpy(link_value, src_value);
}

void XML_NODE_LEAF::assignType(const NODE_TYPE src_type)
{
	link_type = src_type;
}


XML_NODE_LEAF::NODE_TYPE XML_NODE_LEAF::getType()
{
	return link_type;
}

XML_NODE_LEAF * XML_NODE_LEAF::getFirstChild()
{
	if (children_count == 0)
	{
		return NULL;
	}

	dumpChildrenListToArray();
	children_position = 0;
	return *(node_children_array);
}

XML_NODE_LEAF * XML_NODE_LEAF::getNextChild()
{
	if (children_count == 0)
	{
		return NULL;
	}

	children_position++;
	if (children_position >= children_count)
	{
		return NULL;
	}

	return *(node_children_array + children_position);
}


XML_NODE_LEAF * XML_NODE_LEAF::matchSearchingCriteria(const char * src_attribute, unsigned int * src_children_position_pointer)
{
	int comparison_result = strcmp(link_name, src_attribute);
	if (comparison_result == 0)
	{
		return this;
	}
	else if (comparison_result < 0 && left_branch)
	{
		return left_branch->matchSearchingCriteria(src_attribute, src_children_position_pointer);
	}
	else if (right_branch)
	{
		*src_children_position_pointer = *src_children_position_pointer + 1 + previous_sibling_nodes_count;
		return right_branch->matchSearchingCriteria(src_attribute, src_children_position_pointer);
	}

	return NULL;
}


XML_NODE_LEAF * XML_NODE_LEAF::findChild(const char * src_attribute)
{
	if (children_count == 0)
	{
		return NULL;
	}

	children_position = 0;
	return node_children->matchSearchingCriteria(src_attribute, &children_position);
}


char * XML_NODE_LEAF::getValue()
{
	return link_value;
}


char * XML_NODE_LEAF::getIdentifier()
{
	return link_name;
}

unsigned int XML_NODE_LEAF::getChildPosition()
{
	return previous_sibling_nodes_count;
}
