#include "load_image_operation_class.h"

LOAD_IMAGE_OPERATION::LOAD_IMAGE_OPERATION()
{
	minimum_input_operations_required = 0;
	input_operations_required = 0;

	input_string_nodes_required = 1;
	NODE_SCALAR<char*> * node_filename_identifier = new NODE_SCALAR<char*> ("filename");
	string_nodes_names_list.assignNodeValue(0, node_filename_identifier);

	NODE_SCALAR<char*> * local_node_A = new NODE_SCALAR<char*>("NOT-DEFINED");
	local_string_nodes_list.assignNodeValue(0, local_node_A);
	string_nodes_list.assignNodeValue(0, local_string_nodes_list.getNodeValue(0));
	string_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<char*> * local_previous_node_A = new NODE_SCALAR<char*>("NOT-DEFINED");
	local_previous_string_nodes_values_list.assignNodeValue(0, local_previous_node_A);
}



LOAD_IMAGE_OPERATION::LOAD_IMAGE_OPERATION(const LOAD_IMAGE_OPERATION & src_load_image_operation)
{
	minimum_input_operations_required = 0;
	input_operations_required = 0;

	input_string_nodes_required = 1;
	NODE_SCALAR<char*> * node_filename_identifier = new NODE_SCALAR<char*>("filename");
	string_nodes_names_list.assignNodeValue(0, node_filename_identifier);

	NODE_SCALAR<char*> * local_node_A = new NODE_SCALAR<char*>("NOT-DEFINED");
	local_string_nodes_list.assignNodeValue(0, local_node_A);
	string_nodes_list.assignNodeValue(0, local_string_nodes_list.getNodeValue(0));
	string_node_is_local_list.assignNodeValue(0, true);
	
	NODE_SCALAR<char*> * local_previous_node_A = new NODE_SCALAR<char*>("NOT-DEFINED");
	local_previous_string_nodes_values_list.assignNodeValue(0, local_previous_node_A);

	copyFromImageOperation(src_load_image_operation);
}



LOAD_IMAGE_OPERATION LOAD_IMAGE_OPERATION::operator=(const LOAD_IMAGE_OPERATION & src_load_image_operation)
{
	if (this != &src_load_image_operation)
	{
		copyFromImageOperation(src_load_image_operation);
	}

	return *this;
}



LOAD_IMAGE_OPERATION::~LOAD_IMAGE_OPERATION()
{

}


void LOAD_IMAGE_OPERATION::performOperation()
{
	if (strcmp(string_nodes_list.getNodeValue(0)->getScalarValue(), "NOT-DEFINED") == 0)
	{
		printf("<<Error: The filename has not been defined yet>>\n");
		return;
	}

	FILE * fp_img = fopen(string_nodes_list.getNodeValue(0)->getScalarValue(), "r");
	if (!fp_img)
	{
		printf("<<Error: The file \'%s\' could not be opened>>\n", string_nodes_list.getNodeValue(0)->getScalarValue());
		return;
	}

	char magic_number[4];

	// Read magic number:
	magic_number[0] = fgetc(fp_img);
	magic_number[1] = fgetc(fp_img);

	fpos_t image_body_starting;
	fgetpos(fp_img, &image_body_starting);

	magic_number[2] = fgetc(fp_img);
	magic_number[3] = fgetc(fp_img);


	if (magic_number[2] == '#' || magic_number[3] == '#')
	{
		// The file contains a commentary without space between the magic number:
		char commentary[512];
		fscanf(fp_img, "%s", commentary);
		printf("Commentary: \'%s\'", commentary);
	}
	else if(magic_number[2] == '\n')
	{
		fsetpos(fp_img, &image_body_starting);
	}

	int width, height;
	fscanf(fp_img, "%i", &width);
	fscanf(fp_img, "%i", &height);

	int max_intensity;
	fscanf(fp_img, "%i\n", &max_intensity);

	if (!dst_img)
	{
		dst_img = createVoidImage(width, height);
	}


	if ((dst_img->width != width) || (dst_img->height != height))
	{
		if (dst_img->image_data)
		{
			free(dst_img->image_data);
		}
		dst_img->width = width;
		dst_img->height = height;
		dst_img->image_data = (double*)calloc(width * height, sizeof(double));
	}


	if (dst_img->head_roi.next_roi)
	{
		ROI_BBOX * next_roi_node = dst_img->head_roi.next_roi;
		ROI_BBOX * current_roi_node;

		while (next_roi_node)
		{
			current_roi_node = next_roi_node;
			next_roi_node = current_roi_node->next_roi;

			free(current_roi_node);
		}

		dst_img->head_roi.next_roi = NULL;
		dst_img->tail_roi = &dst_img->head_roi;
	}


	if (magic_number[1] == '5')
	{
		int pix_intensity_character;
		for (unsigned int pix_position = 0; pix_position < (unsigned int)(width*height); pix_position++)
		{
			pix_intensity_character = fgetc(fp_img);
			printf("%i\n", feof(fp_img));
			*(dst_img->image_data + pix_position) = (double)(unsigned int)pix_intensity_character / (double)max_intensity;
		}
	}
	else
	{
		unsigned int pix_intensity_integer;
		for (unsigned int pix_position = 0; pix_position < (unsigned int)(width*height); pix_position++)
		{
			fscanf(fp_img, "%u", &pix_intensity_integer);
			*(dst_img->image_data + pix_position) = (double)pix_intensity_integer / (double)max_intensity;
		}
	}

	fclose(fp_img);
}