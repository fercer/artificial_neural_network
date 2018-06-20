#ifndef IMAGE_PROCESS_HANDLER_CLASS_H_INCLUDED
#define IMAGE_PROCESS_HANDLER_CLASS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "generic_list_class.h"

#include "div_image_operation_class.h"
#include "mult_image_operation_class.h"
#include "sum_image_operation_class.h"
#include "load_image_operation_class.h"
#include "save_image_operation_class.h"
#include "filter_image_operation_class.h"
#include "rotate_image_operation_class.h"
#include "translate_image_operation_class.h"
#include "sqroot_image_operation_class.h"
#include "ones_image_operation_class.h"

#include "sum_image_scalar_operation_class.h"
#include "max_image_scalar_operation_class.h"
#include "min_image_scalar_operation_class.h"
#include "width_image_scalar_operation_class.h"
#include "height_image_scalar_operation_class.h"

#include "scalar_div_nodes_operation_class.h"
#include "scalar_mult_nodes_operation_class.h"
#include "scalar_sqrt_nodes_operation_class.h"
#include "scalar_sub_nodes_operation_class.h"
#include "scalar_sum_nodes_operation_class.h"

class IMAGE_PROCESS_HANDLER
{
public:
	IMAGE_PROCESS_HANDLER();
	IMAGE_PROCESS_HANDLER(const IMAGE_PROCESS_HANDLER& src_image_process_handler);
	IMAGE_PROCESS_HANDLER operator=(const IMAGE_PROCESS_HANDLER& src_image_process_handler);

	~IMAGE_PROCESS_HANDLER();

	void setFilename(const char * src_filename);

	void loadProcess();


private:
	typedef enum OPERATION_TYPE
	{
		/* Base oepration types: */
		OT_NODE_SCALAR = 0,
		OT_NODES_SCALAR_OPERATION = 1,
		OT_IMAGE_OPERATION = 2,
		OT_IMAGE_SCALAR_OPERATION = 3,

		/* Derived operation types: */
		/* Node Value */
		NV_NUMERIC = 4,
		NV_STRING = 5,

		/* Node Scalar Operation */
		NOV_SUM = 6,
		NOV_SUB = 7,
		NOV_MULT = 8,
		NOV_DIV = 9,
		NOV_SQRT = 10,

		/* Image Operation */
		IO_DIV = 11,
		IO_MULT = 12,
		IO_SUM = 13,
		IO_LOAD = 14,
		IO_SAVE = 15,
		IO_FILTER = 16,
		IO_ROTATE = 17,
		IO_TRANSLATE = 18,
		IO_SQRT = 19,
		IO_ONES = 20,

		/* Image Scalar Operation */
		ISO_SUM = 21,
		ISO_MAX = 22,
		ISO_MIN = 23,
		ISO_WIDTH = 24,
		ISO_HEIGHT = 25
	};

	typedef enum LINK_TYPE
	{
		NT_HEADER = 0,
		NT_VARIABLE = 1,
		NT_VALUE = 2,
		NT_EQUAL_SIGN = 3,
		NT_TAIL = 4
	};

	typedef struct CHAIN_LINK
	{
		char string_value[1024];
		unsigned int lenght;
		LINK_TYPE link_node_type;
		CHAIN_LINK * previous_link;
		CHAIN_LINK * next_link;
	};

	CHAIN_LINK chain_head;
	CHAIN_LINK * chain_tail;

	char filename[1024];

	GENERIC_LIST<NODE_SCALAR<char*>*> string_node_scalar_list;
	unsigned int string_node_scalar_count;

	GENERIC_LIST<NODE_SCALAR<double>*> numeric_node_scalar_list;
	unsigned int numeric_node_scalar_count;

	GENERIC_LIST<NODES_SCALAR_OPERATION*> nodes_operation_list;
	unsigned int nodes_operation_count;

	GENERIC_LIST<IMAGE_OPERATION*> image_operation_list;
	unsigned int image_operation_count;

	GENERIC_LIST<IMAGE_SCALAR_OPERATION*> image_scalar_operation_list;
	unsigned int image_scalar_operation_count;
	
	void loadProcessFile();
	CHAIN_LINK * dumpLinkToList(CHAIN_LINK * src_root);


	CHAIN_LINK * dumpLinkVariableToList(CHAIN_LINK * src_root, GENERIC_LIST<NODE_SCALAR<double>*>* src_input_nodes_list);

	CHAIN_LINK * dumpLinkVariableToList(CHAIN_LINK * src_root, GENERIC_LIST<NODE_SCALAR<char*>*>* src_input_nodes_list);

	CHAIN_LINK * dumpLinkVariableToList(CHAIN_LINK * src_root, GENERIC_LIST<IMAGE_OPERATION*>* src_input_operation_list);
};

#endif //IMAGE_PROCESS_HANDLER_CLASS_H_INCLUDED