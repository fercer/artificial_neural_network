#ifndef IMAGE_PROCESS_HANDLER_CLASS_H_INCLUDED
#define IMAGE_PROCESS_HANDLER_CLASS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "generic_list_class.cpp"
#include "xml_handler_class.h"
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
#include "scalar_atan_nodes_operation_class.h"
#include "scalar_tan_nodes_operation_class.h"
#include "scalar_sin_nodes_operation_class.h"
#include "scalar_cos_nodes_operation_class.h"

class IMAGE_PROCESS_HANDLER
{
public:
	IMAGE_PROCESS_HANDLER();
	IMAGE_PROCESS_HANDLER(const IMAGE_PROCESS_HANDLER& src_image_process_handler);
	IMAGE_PROCESS_HANDLER operator=(const IMAGE_PROCESS_HANDLER& src_image_process_handler);

	~IMAGE_PROCESS_HANDLER();

	void setFilename(const char * src_filename);

	void loadProcess();

	void runProcess();

	void runNodeOperation(const unsigned int src_node_index);

	void runImageOperation(const unsigned int src_node_index);

	void runImageScalarOperation(const unsigned int src_node_index);
	


	void setInputNodeValue(const unsigned int src_node_index, const double src_numeric_value);
	void setInputNodeValue(const unsigned int src_node_index, const unsigned int src_operation_index, NODE_SCALAR<double> * src_new_link);
	void setInputNodeValue(const unsigned int src_node_index, const char * src_string_value);
	void setInputNodeValue(const unsigned int src_node_index, const unsigned int src_operation_index, NODE_SCALAR<char*> * src_new_link);
	
	void setInputNodeToImageOperation(const unsigned int src_node_index, const unsigned int src_operation_index, NODE_SCALAR<double> * src_new_link);
	void setInputNodeToImageOperation(const unsigned int src_node_index, const unsigned int src_operation_index, NODE_SCALAR<char*> * src_new_link);
	void setInputNodeToImageOperation(const unsigned int src_node_index, const unsigned int src_operation_index, IMAGE_OPERATION * src_new_link);
	
	void setInputNodeToImageScalarOperation(const unsigned int src_node_index, const unsigned int src_operation_index, NODE_SCALAR<double> * src_new_link);
	void setInputNodeToImageScalarOperation(const unsigned int src_node_index, const unsigned int src_operation_index, NODE_SCALAR<char*> * src_new_link);
	void setInputNodeToImageScalarOperation(const unsigned int src_node_index, const unsigned int src_operation_index, IMAGE_OPERATION * src_new_link);

private:
	typedef enum _OPERATION_TYPE
	{
		/* Base oepration types: */
		OT_NODE_SCALAR = 899,
		OT_NODES_SCALAR_OPERATION = 1844,
		OT_IMAGE_OPERATION = 1428,
		OT_IMAGE_SCALAR_OPERATION = 2026,

		/* Derived operation types: */
		/* Node Value */
		NV_NUMERIC = 790,
		NV_STRING = 730,

		/* Node Scalar Operation */
		NOV_SUM = 583,
		NOV_SUB = 572,
		NOV_MULT = 660,
		NOV_DIV = 565,
		NOV_SQRT = 668,
		NOV_ATANGENT = 932,
		NOV_TANGENT = 867,
		NOV_SINUS = 740,
		NOV_COSINUS = 886,

		/* Image Operation */
		IO_DIV = 474,
		IO_MULT = 569,
		IO_SUM = 492,
		IO_LOAD = 535,
		IO_SAVE = 550,
		IO_FILTER = 701,
		IO_ROTATE = 710,
		IO_TRANSLATE = 933,
		IO_SQRT = 577,
		IO_ONES = 556,

		/* Image Scalar Operation */
		ISO_SUM = 575,
		ISO_MAX = 560,
		ISO_MIN = 558,
		ISO_WIDTH = 714,
		ISO_HEIGHT = 771
	} OPERATION_TYPE;

	char filename[1024];

	GENERIC_LIST<NODE_SCALAR<char*>*> string_node_scalar_list;
	GENERIC_LIST<int> string_node_scalar_position_list;
	unsigned int string_node_scalar_count;

	GENERIC_LIST<NODE_SCALAR<double>*> numeric_node_scalar_list;
	GENERIC_LIST<int> numeric_node_scalar_position_list;
	unsigned int numeric_node_scalar_count;

	GENERIC_LIST<NODES_SCALAR_OPERATION*> nodes_operation_list;
	GENERIC_LIST<int> nodes_operation_position_list;
	unsigned int nodes_operation_count;

	GENERIC_LIST<IMAGE_OPERATION*> image_operation_list;
	GENERIC_LIST<int> image_operation_position_list;
	unsigned int image_operation_count;

	GENERIC_LIST<IMAGE_SCALAR_OPERATION*> image_scalar_operation_list;
	GENERIC_LIST<int> image_scalar_operation_position_list;
	unsigned int image_scalar_operation_count;

	XML_HANDLER xml_file_handler;
};

#endif //IMAGE_PROCESS_HANDLER_CLASS_H_INCLUDED