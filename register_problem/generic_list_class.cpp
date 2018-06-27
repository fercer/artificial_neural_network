#ifndef GENERIC_LIST_CLASS_CPP_INCLUDED
#define GENERIC_LIST_CLASS_CPP_INCLUDED

#include "generic_list_class.h"

template<class class_link_type>
void GENERIC_LIST<class_link_type>::dumpListToArray()
{
	if (last_dump_nodes_count == nodes_count)
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
}



template<class class_link_type>
class_link_type GENERIC_LIST<class_link_type>::getNodeValue_after_listdump(const unsigned int src_node_index)
{
	dumpListToArray();
	get_node_value_method = &GENERIC_LIST<class_link_type>::getNodeValue_without_listdump;
	return *(array_from_list + src_node_index);
}


template<class class_link_type>
class_link_type GENERIC_LIST<class_link_type>::getNodeValue_without_listdump(const unsigned int src_node_index)
{
	return *(array_from_list + src_node_index);
}



template<class class_link_type>
void GENERIC_LIST<class_link_type>::emptyList()
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
	
	get_node_value_method = &GENERIC_LIST::getNodeValue_after_listdump;
}
	


template<class class_link_type>
void GENERIC_LIST<class_link_type>::addNodeToList(const class_link_type src_node)
{
	tail_node->next_node = new LIST_NODE;
	tail_node->next_node->next_node = NULL;

	tail_node->next_node->node_value = src_node;

	tail_node = tail_node->next_node;
	nodes_count++;
	
	get_node_value_method = &GENERIC_LIST<class_link_type>::getNodeValue_after_listdump;
}



template<class class_link_type>
GENERIC_LIST<class_link_type>::GENERIC_LIST()
{
	tail_node = &head_node;
	head_node.next_node = NULL;
	nodes_count = 0;
	last_dump_nodes_count = 0;
	array_from_list = NULL;
}
	


template<class class_link_type>
GENERIC_LIST<class_link_type>::GENERIC_LIST(const GENERIC_LIST<class_link_type>& src_generic_class)
{
	this->tail_node = &head_node;
	this->head_node.next_node = NULL;
	this->nodes_count = 0;
	this->last_dump_nodes_count = 0;

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


template<class class_link_type>
GENERIC_LIST<class_link_type> GENERIC_LIST<class_link_type>::operator=(const GENERIC_LIST<class_link_type>& src_generic_class)
{
	if (this != &src_generic_class)
	{
		this->emptyList();

		this->tail_node = &head_node;
		this->head_node.next_node = NULL;
		this->nodes_count = 0;
		this->last_dump_nodes_count = 0;

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
	


template<class class_link_type>
GENERIC_LIST<class_link_type>::~GENERIC_LIST()
{
	emptyList();
}



template<class class_link_type>
unsigned int GENERIC_LIST<class_link_type>::assignNodeValue(const unsigned int src_index, const class_link_type& src_node)
{
	if (src_index >= nodes_count)
	{
		addNodeToList(src_node);
	}
	
	dumpListToArray();

	*(array_from_list + src_index) = src_node;

	return nodes_count;
}




template<class class_link_type>
class_link_type GENERIC_LIST<class_link_type>::getNodeValue(const unsigned int src_node_index)
{
	if (src_node_index >= nodes_count)
	{
		return (class_link_type)0;
	}

	return (this->*get_node_value_method)(src_node_index);
}




template<class class_link_type>
unsigned int GENERIC_LIST<class_link_type>::getNodesCount()
{
	return nodes_count;
}


/* ------------------------- GENERIC_ORDERED_LIST ---------------------------------------------------------------------*/
/*
template<class class_link_type>
void GENERIC_ORDERED_LIST<class_link_type>::dumpListToArray()
{
	if (last_dump_nodes_count == nodes_count)
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

	LIST_ORDERED_NODE * next_node_pointer = head_node.next_node;
	LIST_ORDERED_NODE * current_node_pointer;
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



template<class class_link_type>
class_link_type GENERIC_ORDERED_LIST<class_link_type>::getNodeValue_after_listdump(const unsigned int src_node_index)
{
	dumpListToArray();
	get_node_value_method = &GENERIC_ORDERED_LIST::getNodeValue_without_listdump;
	return *(array_from_list + src_node_index);
}


template<class class_link_type>
class_link_type GENERIC_ORDERED_LIST<class_link_type>::getNodeValue_without_listdump(const unsigned int src_node_index)
{
	return *(array_from_list + src_node_index);
}



template<class class_link_type>
void GENERIC_ORDERED_LIST<class_link_type>::emptyList()
{
	LIST_ORDERED_NODE * next_node_pointer = head_node.next_node;
	LIST_ORDERED_NODE * current_node_pointer;

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

	get_node_value_method = &GENERIC_ORDERED_LIST<class_link_type>::getNodeValue_after_listdump;
}
	


template<class class_link_type>
void GENERIC_ORDERED_LIST<class_link_type>::addNodeToList(const class_link_type src_node)
{
	tail_node->next_node = new LIST_ORDERED_NODE;
	tail_node->next_node->next_node = NULL;

	tail_node->next_node->node_value = src_node;

	tail_node = tail_node->next_node;
	nodes_count++;

	list_was_dumped_to_array = false;

	get_node_value_method = &GENERIC_ORDERED_LIST<class_link_type>::getNodeValue_after_listdump;
}

template<class class_link_type>
GENERIC_ORDERED_LIST<class_link_type>::GENERIC_ORDERED_LIST<class_link_type>()
{
	tail_node = &head_node;
	head_node.next_node = NULL;
	nodes_count = 0;
	last_dump_nodes_count = 0;
	array_from_list = NULL;
	list_was_dumped_to_array = false;
}
	


template<class class_link_type>
GENERIC_ORDERED_LIST<class_link_type>::GENERIC_ORDERED_LIST<class_link_type>(const GENERIC_ORDERED_LIST<class_link_type>& src_generic_class)
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


template<class class_link_type>
GENERIC_ORDERED_LIST<class_link_type> GENERIC_ORDERED_LIST<class_link_type>::operator=(const GENERIC_ORDERED_LIST<class_link_type>& src_generic_class)
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
	


template<class class_link_type>
GENERIC_ORDERED_LIST<class_link_type>::~GENERIC_ORDERED_LIST<class_link_type>()
{
	emptyList();
}



template<class class_link_type>
unsigned int GENERIC_ORDERED_LIST<class_link_type>::assignNodeValue(const unsigned int src_index, const class_link_type src_node)
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




template<class class_link_type>
class_link_type GENERIC_ORDERED_LIST<class_link_type>::getNodeValue(const unsigned int src_node_index)
{
	return (this->*get_node_value_method)(src_node_index);
}




template<class class_link_type>
unsigned int GENERIC_ORDERED_LIST<class_link_type>::getNodesCount()
{
	return nodes_count;
}

*/

#endif //GENERIC_LIST_CLASS_CPP_INCLUDED