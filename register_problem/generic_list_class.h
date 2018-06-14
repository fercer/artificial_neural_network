#ifndef GENERIC_LIST_CLASS_H_INCLUDED
#define GENERIC_LIST_CLASS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

template<class class_link_type>
class GENERIC_LIST
{
private:
	typedef struct LIST_NODE
	{
		class_link_type node_value;
		LIST_NODE * next_node;
	};

	unsigned int nodes_count;
	bool list_was_dumped_to_array;
	LIST_NODE head_node;
	LIST_NODE * tail_node;

public:
	GENERIC_LIST()
	{
		tail_node = &head_node;
		head_node.next_node = NULL;
		nodes_count = 0;
		list_was_dumped_to_array = false;
	}
	
	GENERIC_LIST(const GENERIC_LIST& src_generic_class)
	{
		tail_node = &head_node;
		head_node.next_node = NULL;
		nodes_count = 0;
		list_was_dumped_to_array = false;

		LIST_NODE * next_node_pointer = src_generic_class.head_node.next_node;
		LIST_NODE * current_node_pointer;

		while (next_node_pointer)
		{
			current_node_pointer = next_node_pointer;
			next_node_pointer = current_node_pointer->next_node;
			
			tail_node->next_node = new LIST_NODE;
			tail_node->next_node->next_node = NULL;

			tail_node->next_node->node_value = current_node_pointer->node_value;
			
			tail_node = tail_node->next_node;
			nodes_count++;
		}
	}

	~GENERIC_LIST()
	{
		LIST_NODE * next_node_pointer = head_node.next_node;
		LIST_NODE * current_node_pointer;

		while (next_node_pointer)
		{
			current_node_pointer = next_node_pointer;
			next_node_pointer = current_node_pointer->next_node;
			delete current_node_pointer;
		}
	}


	unsigned int addNodeToList(const class_link_type& src_node)
	{
		tail_node->next_node = new LIST_NODE;
		tail_node->next_node->next_node = NULL;

		tail_node->next_node->node_value = new class_link_type(src_node);

		tail_node = tail_node->next_node;
		nodes_count++;

		list_was_dumped_to_array = false;

		return nodes_count;
	}


	class_link_type * dumpListToArray()
	{
		if ((nodes_count < 1) || (list_was_dumped_to_array))
		{
			return NULL;
		}

		class_link_type * array_from_list = new class_link_type[nodes_count];
		unsigned int current_node_position = 0;
		while (next_node_pointer)
		{
			current_node_pointer = next_node_pointer;
			next_node_pointer = current_node_pointer->next_node;

			*(array_from_list + current_node_position) = current_node_position->node_value;
			current_node_position++;
		}

		list_was_dumped_to_array = true;

		return array_from_list;
	}
};


#endif // GENERIC_LIST_CLASS_H_INCLUDED