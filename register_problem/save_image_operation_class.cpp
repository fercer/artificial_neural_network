#include "save_image_operation_class.h"

SAVE_IMAGE_OPERATION::SAVE_IMAGE_OPERATION()
{
	input_string_nodes_required = 1;
	nodes_names_list.assignNodeValue(0, "filename");

	NODE_SCALAR<char*> * local_node_A = (NODE_SCALAR<char*>*)malloc(sizeof(NODE_SCALAR<char*>));
	local_node_A->setScalarValue("NOT-DEFINED");
	local_string_nodes_list.assignNodeValue(0, local_node_A);
	string_nodes_list.assignNodeValue(0, local_node_A);
	string_node_is_local_list.assignNodeValue(0, true);
}



SAVE_IMAGE_OPERATION::SAVE_IMAGE_OPERATION(const SAVE_IMAGE_OPERATION & src_save_image_operation)
{
	copyFromImageOperation(src_save_image_operation);
}



SAVE_IMAGE_OPERATION SAVE_IMAGE_OPERATION::operator=(const SAVE_IMAGE_OPERATION & src_save_image_operation)
{
	if (this != &src_save_image_operation)
	{
		copyFromImageOperation(src_save_image_operation);
	}

	return *this;
}



SAVE_IMAGE_OPERATION::~SAVE_IMAGE_OPERATION()
{
	// Nothing to deallocate
}


void SAVE_IMAGE_OPERATION::performOperation()
{
	if (!src_img_A)
	{
		printf("<<Error: The source image was not assigned>\n");
		return;
	}

	FILE * fp_img = fopen(string_nodes_list.getNodeValue(0)->getScalarValue(), "w");

	if (!fp_img)
	{
		printf("<Error: The file \"%s\" could not be created>\n", string_nodes_list.getNodeValue(0)->getScalarValue());
		return;
	}

	// Print magic number:
	fprintf(fp_img, "P2\n");

	// Print the commentary:
	fprintf(fp_img, "#Created by FerCer\n");

	// Print the width and height:
	fprintf(fp_img, "%i %i\n", width_A, height_A);
	fprintf(fp_img, "255\n");

	for (unsigned int pix_position = 0; pix_position < (width_A*height_A); pix_position++)
	{
		fprintf(fp_img, "%u\n", (unsigned int)floor(255.0 * *(src_img_A->image_data + pix_position)));
	}

	fclose(fp_img);
}