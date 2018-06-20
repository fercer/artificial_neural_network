#ifndef IMAGE_PROCESS_HANDLER_CLASS_H_INCLUDED
#define IMAGE_PROCESS_HANDLER_CLASS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "generic_list_class.h"

#include "div_image_operation_class.h"
#include "mult_image_operation_class.h"
#include "sum_image_operation_class.h"
#include "scalar_sum_image_operation_class.h"
#include "load_image_operation_class.h"
#include "save_image_operation_class.h"
#include "filter_image_operation_class.h"
#include "rotate_image_operation_class.h"
#include "translate_image_operation_class.h"
#include "sqroot_image_operation_class.h"
#include "max_image_scalar_operation_class.h"
#include "min_image_scalar_operation_class.h"
#include "sum_image_scalar_operation_class.h"
#include "ones_image_operation_class.h"
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
	GENERIC_LIST<NODE_SCALAR<double>*> numeric_node_scalar_list;
	GENERIC_LIST<NODES_SCALAR_OPERATION*> nodes_operation_list;
	GENERIC_LIST<IMAGE_OPERATION*> image_operation_list;
	GENERIC_LIST<IMAGE_SCALAR_OPERATION*> image_scalar_operation_list;
	
	void loadProcessFile();
	void dumpLinkToList(CHAIN_LINK * src_root);
};

#endif //IMAGE_PROCESS_HANDLER_CLASS_H_INCLUDED