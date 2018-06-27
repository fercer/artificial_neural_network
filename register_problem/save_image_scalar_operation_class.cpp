#include "save_image_scalar_operation_class.h"

SAVE_IMAGE_SCALAR_OPERATION::SAVE_IMAGE_SCALAR_OPERATION()
{
	input_string_nodes_required = 1;
	NODE_SCALAR<char*> * node_filename_identifier = new NODE_SCALAR<char*>("filename");
	string_nodes_names_list.assignNodeValue(0, node_filename_identifier);

	NODE_SCALAR<char*> * local_node_A = new NODE_SCALAR<char*>("NOT-DEFINED");
	local_string_nodes_list.assignNodeValue(0, local_node_A);
	string_nodes_list.assignNodeValue(0, local_string_nodes_list.getNodeValue(0));
	string_node_is_local_list.assignNodeValue(0, true);
}



SAVE_IMAGE_SCALAR_OPERATION::SAVE_IMAGE_SCALAR_OPERATION(const SAVE_IMAGE_SCALAR_OPERATION & src_save_image_scalar_operation)
{
	input_string_nodes_required = 1;
	NODE_SCALAR<char*> * node_filename_identifier = new NODE_SCALAR<char*>("filename");
	string_nodes_names_list.assignNodeValue(0, node_filename_identifier);

	NODE_SCALAR<char*> * local_node_A = new NODE_SCALAR<char*>("NOT-DEFINED");
	local_string_nodes_list.assignNodeValue(0, local_node_A);
	string_nodes_list.assignNodeValue(0, local_string_nodes_list.getNodeValue(0));
	string_node_is_local_list.assignNodeValue(0, true);

	copyFromImageScalarOperation(src_save_image_scalar_operation);
}



SAVE_IMAGE_SCALAR_OPERATION SAVE_IMAGE_SCALAR_OPERATION::operator=(const SAVE_IMAGE_SCALAR_OPERATION & src_save_image_scalar_operation)
{
	if (this != &src_save_image_scalar_operation)
	{
		copyFromImageScalarOperation(src_save_image_scalar_operation);
	}

	return *this;
}



SAVE_IMAGE_SCALAR_OPERATION::~SAVE_IMAGE_SCALAR_OPERATION()
{

}


double SAVE_IMAGE_SCALAR_OPERATION::performScalarOperation()
{
	if (!src_img)
	{
		printf("<<Error: The source image was not assigned>\n");
		return 0.0;
	}

	FILE * fp_img = fopen(string_nodes_list.getNodeValue(0)->getScalarValue(), "w");

	if (!fp_img)
	{
		printf("<Error: The file \"%s\" could not be created>\n", string_nodes_list.getNodeValue(0)->getScalarValue());
		return 0.0;
	}

	// Print magic number:
	fprintf(fp_img, "P2\n");

	// Print the commentary:
	fprintf(fp_img, "#Created by FerCer\n");

	// Print the width and height:
	fprintf(fp_img, "%i %i\n", width, height);
	fprintf(fp_img, "255\n");

	for (unsigned int pix_position = 0; pix_position < (width*height); pix_position++)
	{
		fprintf(fp_img, "%u\n", (unsigned int)floor(255.0 * *(src_img->image_data + pix_position)));
	}

	fclose(fp_img);

	return 1.0;
}