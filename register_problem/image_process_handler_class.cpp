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

	XML_NODE_LEAF * root_node = xml_file_handler.getRoot();

	XML_NODE_LEAF * first_header = root_node->getFirstChild();
	while (first_header)
	{
		printf("Header identifier: %s\n", first_header->getIdentifier());

		XML_NODE_LEAF * first_child = first_header->getFirstChild();
		while (first_child)
		{
			printf("\tNode identifier: %s\n", first_child->getIdentifier());

			XML_NODE_LEAF *first_child_attribute = first_child->getFirstChild();
			printf("\tAttributes:\n");
			while (first_child_attribute)
			{
				printf("\t\t%s = %s\n", first_child_attribute->getIdentifier(), first_child_attribute->getValue());
				XML_NODE_LEAF * attribute_child = first_child_attribute->getFirstChild();
				while (attribute_child)
				{
					printf("\t\t\%s = %s\n", attribute_child->getIdentifier(), attribute_child->getValue());
					attribute_child = first_child_attribute->getNextChild();
				}

				first_child_attribute = first_child->getNextChild();
			}
			first_child = first_header->getNextChild();
		}
		first_header = root_node->getNextChild();
	}
}
