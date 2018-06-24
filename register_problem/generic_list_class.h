#ifndef GENERIC_LIST_CLASS_H_INCLUDED
#define GENERIC_LIST_CLASS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

template<class class_link_type>
class GENERIC_LIST
{
public:
	GENERIC_LIST();
	GENERIC_LIST(const GENERIC_LIST& src_generic_class);
	GENERIC_LIST operator=(const GENERIC_LIST& src_generic_class);
	~GENERIC_LIST();


	unsigned int assignNodeValue(const unsigned int src_index, const class_link_type src_node);
	class_link_type getNodeValue(const unsigned int src_node_index);
	unsigned int getNodesCount();

private:
	typedef struct LIST_NODE
	{
		class_link_type node_value;
		LIST_NODE * next_node;
	} LIST_NODE;

	class_link_type(GENERIC_LIST<class_link_type>::*get_node_value_method)(const unsigned int src_node_index);

	unsigned int nodes_count;
	unsigned int last_dump_nodes_count;
	LIST_NODE head_node;
	LIST_NODE * tail_node;
	class_link_type * array_from_list;


	void dumpListToArray();

	class_link_type getNodeValue_after_listdump(const unsigned int src_node_index);
	class_link_type getNodeValue_without_listdump(const unsigned int src_node_index);

	void emptyList();
	void addNodeToList(const class_link_type src_node);

};



/*
template<class class_link_type>
class GENERIC_ORDERED_LIST
{
public:
	GENERIC_ORDERED_LIST();
	GENERIC_ORDERED_LIST(const GENERIC_ORDERED_LIST& src_generic_class);
	GENERIC_ORDERED_LIST operator=(const GENERIC_ORDERED_LIST& src_generic_class);
	~GENERIC_ORDERED_LIST();


	unsigned int assignNodeValue(const unsigned int src_index, const class_link_type src_node);
	class_link_type getNodeValue(const unsigned int src_node_index);
	unsigned int getNodesCount();

private:
	typedef struct LIST_ORDERED_NODE
	{
		class_link_type node_value;
		LIST_ORDERED_NODE * left_node;
		LIST_ORDERED_NODE * right_node;
		unsigned int previous_nodes_count;
	};

	class_link_type(GENERIC_ORDERED_LIST::*get_node_value_method)(const unsigned int src_node_index);

	unsigned int nodes_count;
	unsigned int last_dump_nodes_count;
	bool list_was_dumped_to_array;
	LIST_ORDERED_NODE head_node;
	LIST_ORDERED_NODE * tail_node;
	class_link_type * array_from_list;


	void dumpListToArray();

	class_link_type getNodeValue_after_listdump(const unsigned int src_node_index);
	class_link_type getNodeValue_without_listdump(const unsigned int src_node_index);

	void emptyList();
	void addNodeToList(const class_link_type src_node);
};
*/
#endif // GENERIC_LIST_CLASS_H_INCLUDED