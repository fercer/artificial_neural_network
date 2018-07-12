#ifndef IMAGE_OPERATION_CLASS_H_INCLUDED
#define IMAGE_OPERATION_CLASS_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node_scalar_class.h"
#include "generic_list_class.cpp"
#include "image_functions.h"


class IMAGE_OPERATION 
{
public:
	IMAGE_OPERATION();

	virtual ~IMAGE_OPERATION();
	
	IMG_DATA * getImageData();

	void setInputOperationA(IMAGE_OPERATION * src_operation_A);

	void setInputOperationB(IMAGE_OPERATION * src_operation_B);
	
	void setImageName(const char * src_image_name);
	
	void assignNodeValue(const unsigned int src_node_position, const double src_node_value);

	void assignNodeValue(const unsigned int src_node_position, const char* src_node_value);

	void assignNodeValue(const unsigned int src_node_position, NODE_SCALAR<double> * src_node_pointer);

	void assignNodeValue(const unsigned int src_node_position, NODE_SCALAR<char*> * src_node_pointer);

	unsigned int getInputNumericNodesRequired();
	
	unsigned int getInputStringNodesRequired();
	
	char * getNumericInputName(const unsigned int src_variable_index);

	char * getStringInputName(const unsigned int src_variable_index);

	bool getOutputHasChanged();

protected:
	unsigned int minimum_input_operations_required; 
	unsigned int input_operations_required;
	unsigned int input_numeric_nodes_required;
	unsigned int input_string_nodes_required;

	GENERIC_LIST<NODE_SCALAR<char*>*> numeric_nodes_names_list;
	GENERIC_LIST<bool> numeric_node_is_local_list;
	GENERIC_LIST<NODE_SCALAR<double>*> local_numeric_nodes_list;
	GENERIC_LIST<NODE_SCALAR<double>*> local_previous_numeric_nodes_values_list;

	GENERIC_LIST<NODE_SCALAR<char*>*> string_nodes_names_list;
	GENERIC_LIST<bool> string_node_is_local_list;
	GENERIC_LIST<NODE_SCALAR<char*>*> local_string_nodes_list;
	GENERIC_LIST<NODE_SCALAR<char*>*> local_previous_string_nodes_values_list;

	GENERIC_LIST<NODE_SCALAR<double>*> numeric_nodes_list;
	GENERIC_LIST<NODE_SCALAR<char*>*> string_nodes_list;
	
	IMG_DATA * dst_img;
	IMG_DATA * src_img_A;
	IMG_DATA * src_img_B;
	
	unsigned int width_A;
	unsigned int height_A;
	unsigned int width_B;
	unsigned int height_B;

	unsigned int computable_width;
	unsigned int computable_height;

	int ULa_x;
	int ULa_y;
	int LRa_x;
	int LRa_y;

	int ULb_x;
	int ULb_y;
	int LRb_x;
	int LRb_y;

	int ULi_x;
	int ULi_y;

	int ULg_x;
	int ULg_y;

	char image_operation_name[64];

	// Use this function when implementing the derived classes  destructor:
	void emptyImageOperation();

	// Use this function when implementing the copy constructor, and assignation operator (operator =), in derived classes:
	void copyFromImageOperation(const IMAGE_OPERATION& src_image_operation);

	// Each derived class must define this function:
	virtual void performOperation() = 0;
	
	void computeActiveROI();

private:
	unsigned int operations_loaded;
	bool output_has_changed;

	bool src_A_has_changed;
	bool src_B_has_changed;

	IMAGE_OPERATION * operation_A;
	IMAGE_OPERATION * operation_B;

	void performImageOperation();
};

#endif // IMAGE_OPERATION_CLASS_H_INCLUDED
