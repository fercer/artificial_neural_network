#ifndef XML_HANDLER_CLASS_H_INCLUDED
#define XML_HANDLER_CLASS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "generic_list_class.h"

class XML_NODE_LEAF
{
public:
	typedef enum NODE_TYPE
	{
		NT_UNDEFINED = 0,
		NT_HEADER = 1,			/* < */
		NT_CLOSER = 2,			/* > */
		NT_ATTRIBUTE = 3,		/* any character */
		NT_VALUE = 4,			/* " */
		NT_TAIL = 5				/* </ */
	};

	XML_NODE_LEAF();
	XML_NODE_LEAF(const char * src_identifier, NODE_TYPE src_type = NT_UNDEFINED);
	XML_NODE_LEAF(const char * src_identifier, const char * src_value, NODE_TYPE src_type = NT_UNDEFINED);
	XML_NODE_LEAF(const XML_NODE_LEAF& src_node_leaf);
	XML_NODE_LEAF operator=(const XML_NODE_LEAF& src_node_leaf);
	~XML_NODE_LEAF();

	void assignChild(XML_NODE_LEAF * src_new_child);
	void assignIdentifier(const char * src_identifier);
	void assignValue(const char * src_value);
	void assignType(const NODE_TYPE src_type);
	
	friend void deallocateTree(XML_NODE_LEAF * src_tree_root);
	
	NODE_TYPE getType();

	XML_NODE_LEAF * getFirstChild();
	XML_NODE_LEAF * getNextChild();
	XML_NODE_LEAF * findChild(const char * src_attribute);

	char * getValue();
	char * getIdentifier();

private:
	char * link_name;
	char * link_value;
	NODE_TYPE link_type;
	XML_NODE_LEAF* node_children;
	XML_NODE_LEAF* left_branch;
	XML_NODE_LEAF* right_branch;
	XML_NODE_LEAF** node_children_array;
	unsigned int children_position;
	unsigned int children_count;
	unsigned int last_children_count;
	unsigned int previous_sibling_nodes_count;
	unsigned int sibling_nodes_count;

	void assignSibling(XML_NODE_LEAF * src_new_sibling);
	void assignSiblingToArray(XML_NODE_LEAF ** src_nodes_array, const unsigned int src_previous_sibling_nodes_count);
	void dumpChildrenListToArray();
	XML_NODE_LEAF * matchSearchingCriteria(const char * src_attribute);
};



class XML_HANDLER
{
public:
	XML_HANDLER();
	XML_HANDLER(const XML_HANDLER& src_xml_handler);
	XML_HANDLER operator=(const XML_HANDLER& src_xml_handler);
	~XML_HANDLER();

	void setFilename(const char * src_filename);
	void loadXMLFile();

	XML_NODE_LEAF * getRoot();

private:
	
	XML_NODE_LEAF * tree_root;

	char * filename;
	bool filename_has_changed;

	bool dumpLeafNode(FILE * src_fp_xml_file, XML_NODE_LEAF * src_parent_leaf, char * src_read_line, const unsigned int src_max_read_characters);

	XML_NODE_LEAF::NODE_TYPE getFollowingIdentifier(FILE * src_fp_xml_file, char * src_read_line, const unsigned int src_max_read_characters);
};

#endif // XML_HANDLER_CLASS_H_INCLUDED
