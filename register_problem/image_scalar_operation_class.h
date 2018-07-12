#ifndef IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED
#define IMAGE_SCALAR_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_functions.h"
#include "image_operation_class.h"
#include "node_scalar_class.h"
#include "generic_list_class.cpp"

class IMAGE_SCALAR_OPERATION :
	public NODE_SCALAR<double>
{
public:
	IMAGE_SCALAR_OPERATION();

	virtual ~IMAGE_SCALAR_OPERATION();	

	double getScalarValue();

	void setInputOperation(IMAGE_OPERATION * src_operation);

	void assignNodeValue(const unsigned int src_node_position, const double src_node_value);

	void assignNodeValue(const unsigned int src_node_position, const char* src_node_value);

	void assignNodeValue(const unsigned int src_node_position, NODE_SCALAR<double> * src_node_pointer);

	void assignNodeValue(const unsigned int src_node_position, NODE_SCALAR<char*> * src_node_pointer);

	unsigned int getInputNumericNodesRequired();

	unsigned int getInputStringNodesRequired();

	char * getNumericInputName(const unsigned int src_variable_index);

	char * getStringInputName(const unsigned int src_variable_index);

protected:
	unsigned int width;
	unsigned int height;
	unsigned int operation_is_assigned;
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
	
	IMG_DATA * src_img;

	// Use this function when implementing the derived classes  destructor:
	void emptyImageScalarOperation();

	// Use this function when implementing the copy constructor, and assignation operator (operator =), in derived classes:
	void copyFromImageScalarOperation(const IMAGE_SCALAR_OPERATION& src_image_scalar_operation);

	// Each derived class must define this function:
	virtual double performScalarOperation() = 0;
	
private:
	bool input_has_changed;
	bool output_has_changed;

	IMAGE_OPERATION * operation;

	void performImageScalarOperation();
};

#endif // IMAGE_OPERATION_CLASS_H_INCLUDED
