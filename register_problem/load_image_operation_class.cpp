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

	char read_line[512];

	// Read magic number:
	fscanf(fp_img, "%s\n", read_line);

	// Read the commentary
	fgets(read_line, 512, fp_img);

	int width, height;
	fscanf(fp_img, "%i %i", &width, &height);


	int max_intensity;
	fscanf(fp_img, "%i", &max_intensity);


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

	unsigned int pix_intensity;
	for (unsigned int pix_position = 0; pix_position < (unsigned int)(width*height); pix_position++)
	{
		fscanf(fp_img, "%u", &pix_intensity);
		*(dst_img->image_data + pix_position) = (double)(unsigned int)pix_intensity / (double)max_intensity;
	}

	fclose(fp_img);
}