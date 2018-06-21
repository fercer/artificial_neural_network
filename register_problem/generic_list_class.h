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

	class_link_type (GENERIC_LIST::*get_node_value_method)(const unsigned int src_node_index);

	unsigned int nodes_count;
	unsigned int last_dump_nodes_count;
	bool list_was_dumped_to_array;
	LIST_NODE head_node;
	LIST_NODE * tail_node;
	class_link_type * array_from_list;


	void dumpListToArray()
	{
		if (list_was_dumped_to_array)
		{
			return;
		}

		if (array_from_list)
		{
			class_link_type * swap_array = array_from_list;
			array_from_list = new class_link_type[nodes_count];
			memmove(array_from_list, swap_array, last_dump_nodes_count * sizeof(class_link_type));
			delete[] swap_array;
		}
		else
		{
			array_from_list = new class_link_type[nodes_count];
		}

		LIST_NODE * next_node_pointer = head_node.next_node;
		LIST_NODE * current_node_pointer;
		unsigned int current_node_position = 0;
		while (next_node_pointer)
		{
			current_node_pointer = next_node_pointer;
			next_node_pointer = current_node_pointer->next_node;

			*(array_from_list + current_node_position) = current_node_pointer->node_value;
			current_node_position++;
		}
		last_dump_nodes_count = nodes_count;
		list_was_dumped_to_array = true;
	}

	class_link_type getNodeValue_after_listdump(const unsigned int src_node_index)
	{
		dumpListToArray();
		get_node_value_method = &GENERIC_LIST::getNodeValue_without_listdump;
		return *(array_from_list + src_node_index);
	}

	class_link_type getNodeValue_without_listdump(const unsigned int src_node_index)
	{
		return *(array_from_list + src_node_index);
	}


	void emptyList()
	{
		LIST_NODE * next_node_pointer = head_node.next_node;
		LIST_NODE * current_node_pointer;

		while (next_node_pointer)
		{
			current_node_pointer = next_node_pointer;
			next_node_pointer = current_node_pointer->next_node;
			delete current_node_pointer;
		}

		tail_node = &head_node;
		head_node.next_node = NULL;

		if (array_from_list)
		{
			delete[] array_from_list;
		}

		array_from_list = NULL;
		nodes_count = 0;
		last_dump_nodes_count = 0;

		list_was_dumped_to_array = false;

		get_node_value_method = &GENERIC_LIST::getNodeValue_after_listdump;
	}
	


	void addNodeToList(const class_link_type src_node)
	{
		tail_node->next_node = new LIST_NODE;
		tail_node->next_node->next_node = NULL;

		tail_node->next_node->node_value = src_node;

		tail_node = tail_node->next_node;
		nodes_count++;

		list_was_dumped_to_array = false;

		get_node_value_method = &GENERIC_LIST::getNodeValue_after_listdump;
	}

public:
	GENERIC_LIST()
	{
		tail_node = &head_node;
		head_node.next_node = NULL;
		nodes_count = 0;
		last_dump_nodes_count = 0;
		array_from_list = NULL;
		list_was_dumped_to_array = false;
	}
	


	GENERIC_LIST(const GENERIC_LIST& src_generic_class)
	{
		this->tail_node = &head_node;
		this->head_node.next_node = NULL;
		this->nodes_count = 0;
		this->last_dump_nodes_count = 0;
		this->list_was_dumped_to_array = false;

		LIST_NODE * next_node_pointer = src_generic_class.head_node.next_node;
		LIST_NODE * current_node_pointer;

		while (next_node_pointer)
		{
			current_node_pointer = next_node_pointer;
			next_node_pointer = current_node_pointer->next_node;
			
			this->tail_node->next_node = new LIST_NODE;
			this->tail_node->next_node->next_node = NULL;

			this->tail_node->next_node->node_value = current_node_pointer->node_value;
			
			this->tail_node = tail_node->next_node;
			this->nodes_count++;
		}
	}

	
	GENERIC_LIST operator=(const GENERIC_LIST& src_generic_class)
	{
		if (this != &src_generic_class)
		{
			this->emptyList();

			this->tail_node = &head_node;
			this->head_node.next_node = NULL;
			this->nodes_count = 0;
			this->last_dump_nodes_count = 0;
			this->list_was_dumped_to_array = false;

			LIST_NODE * next_node_pointer = src_generic_class.head_node.next_node;
			LIST_NODE * current_node_pointer;

			while (next_node_pointer)
			{
				current_node_pointer = next_node_pointer;
				next_node_pointer = current_node_pointer->next_node;

				this->tail_node->next_node = new LIST_NODE;
				this->tail_node->next_node->next_node = NULL;

				this->tail_node->next_node->node_value = current_node_pointer->node_value;

				this->tail_node = tail_node->next_node;
				this->nodes_count++;
			}
		}
		return *this;
	}
	


	~GENERIC_LIST()
	{
		emptyList();
	}

	unsigned int assignNodeValue(const unsigned int src_index, const class_link_type src_node)
	{
		if (src_index >= nodes_count)
		{
			addNodeToList(src_node);
		}
		
		if(src_index >= last_dump_nodes_count)
		{
			dumpListToArray();
		}

		*(array_from_list + src_index) = src_node;

		return nodes_count;
	}


	class_link_type getNodeValue(const unsigned int src_node_index)
	{
		return (this->*get_node_value_method)(src_node_index);
	}


	unsigned int getNodesCount()
	{
		return nodes_count;
	}
};


#endif // GENERIC_LIST_CLASS_H_INCLUDED