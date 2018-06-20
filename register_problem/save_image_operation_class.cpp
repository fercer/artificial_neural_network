#include "save_image_operation_class.h"

SAVE_IMAGE_OPERATION::SAVE_IMAGE_OPERATION()
{
	default_filename.setScalarValue("img.pgm");
	string_parameters_nodes_list.assignNodeValue(0, &default_filename);
}



SAVE_IMAGE_OPERATION::SAVE_IMAGE_OPERATION(const SAVE_IMAGE_OPERATION & src_save_image_operation)
{
	copyFromImageOperation(src_save_image_operation);

	/* Verify if the parameters are connected to an outer node pointer,
	or if them are connected to the default nodes of the source:
	*/
	this->string_parameters_nodes_list = src_save_image_operation.string_parameters_nodes_list;

	NODE_SCALAR<char *> * src_filename_pointer = this->string_parameters_nodes_list.getNodeValue(0);

	if (src_filename_pointer == &src_save_image_operation.default_filename)
	{
		this->default_filename.setScalarValue(src_filename_pointer->getScalarValue());
		this->string_parameters_nodes_list.assignNodeValue(0, &this->default_filename);
	}
}



SAVE_IMAGE_OPERATION SAVE_IMAGE_OPERATION::operator=(const SAVE_IMAGE_OPERATION & src_save_image_operation)
{
	if (this != &src_save_image_operation)
	{
		copyFromImageOperation(src_save_image_operation);

		/* Verify if the parameters are connected to an outer node pointer,
		or if them are connected to the default nodes of the source:
		*/
		this->string_parameters_nodes_list = src_save_image_operation.string_parameters_nodes_list;

		NODE_SCALAR<char *> * src_filename_pointer = this->string_parameters_nodes_list.getNodeValue(0);

		if (src_filename_pointer == &src_save_image_operation.default_filename)
		{
			this->default_filename.setScalarValue(src_filename_pointer->getScalarValue());
			this->string_parameters_nodes_list.assignNodeValue(0, &this->default_filename);
		}
	}

	return *this;
}



SAVE_IMAGE_OPERATION::~SAVE_IMAGE_OPERATION()
{
	// Nothing to deallocate
}

void SAVE_IMAGE_OPERATION::setFilename(const char * src_filename)
{
	default_filename.setScalarValue(src_filename);
	parameters_have_changed = true;
}


void SAVE_IMAGE_OPERATION::setFilename(NODE_SCALAR<char*>* src_node_filename)
{
	if (src_node_filename)
	{
		string_parameters_nodes_list.assignNodeValue(0, src_node_filename);
		parameters_have_changed = true;
	}
	else
	{
		default_filename.setScalarValue(string_parameters_nodes_list.getNodeValue(0)->getScalarValue());
		string_parameters_nodes_list.assignNodeValue(0, &default_filename);
	}
}


void SAVE_IMAGE_OPERATION::performOperation()
{
	if (!src_img_A)
	{
		printf("<<Error: The source image was not assigned>\n");
		return;
	}

	FILE * fp_img = fopen(string_parameters_nodes_list.getNodeValue(0)->getScalarValue(), "w");

	if (!fp_img)
	{
		printf("<Error: The file \"%s\" could not be created>\n", string_parameters_nodes_list.getNodeValue(0)->getScalarValue());
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