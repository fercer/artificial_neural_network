#include "load_image_operation_class.h"

LOAD_IMAGE_OPERATION::LOAD_IMAGE_OPERATION()
{
	default_filename.setScalarValue("NOT-DEFINED");
	string_parameters_nodes_list.assignNodeValue(0, &default_filename);
}



LOAD_IMAGE_OPERATION::LOAD_IMAGE_OPERATION(const LOAD_IMAGE_OPERATION & src_load_image_operation)
{
	copyFromImageOperation(src_load_image_operation);

	/* Verify if the parameters are connected to an outer node pointer,
	or if them are connected to the default nodes of the source:
	*/
	this->string_parameters_nodes_list = src_load_image_operation.string_parameters_nodes_list;

	NODE_SCALAR<char *> * src_filename_pointer = this->string_parameters_nodes_list.getNodeValue(0);

	if (src_filename_pointer == &src_load_image_operation.default_filename)
	{
		this->default_filename.setScalarValue(src_filename_pointer->getScalarValue());
		this->string_parameters_nodes_list.assignNodeValue(0, &this->default_filename);
	}
}



LOAD_IMAGE_OPERATION LOAD_IMAGE_OPERATION::operator=(const LOAD_IMAGE_OPERATION & src_load_image_operation)
{
	if (this != &src_load_image_operation)
	{
		copyFromImageOperation(src_load_image_operation);

		/* Verify if the parameters are connected to an outer node pointer,
		or if them are connected to the default nodes of the source:
		*/
		this->string_parameters_nodes_list = src_load_image_operation.string_parameters_nodes_list;

		NODE_SCALAR<char *> * src_filename_pointer = this->string_parameters_nodes_list.getNodeValue(0);

		if (src_filename_pointer == &src_load_image_operation.default_filename)
		{
			this->default_filename.setScalarValue(src_filename_pointer->getScalarValue());
			this->string_parameters_nodes_list.assignNodeValue(0, &this->default_filename);
		}
	}

	return *this;
}



LOAD_IMAGE_OPERATION::~LOAD_IMAGE_OPERATION()
{
	// Nothing to deallocate
}


void LOAD_IMAGE_OPERATION::setFilename(const char * src_filename)
{
	default_filename.setScalarValue(src_filename);
	parameters_have_changed = true;
}

void LOAD_IMAGE_OPERATION::setFilename(NODE_SCALAR<char*>* src_node_filename)
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


void LOAD_IMAGE_OPERATION::performOperation()
{
	if (strcmp(string_parameters_nodes_list.getNodeValue(0)->getScalarValue(), "NOT-DEFINED") == 0)
	{
		printf("<<Error: The filename has not been defined yet>>\n");
		return;
	}

	FILE * fp_img = fopen(string_parameters_nodes_list.getNodeValue(0)->getScalarValue(), "r");
	if (!fp_img)
	{
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
	for (unsigned int pix_position = 0; pix_position < (width*height); pix_position++)
	{
		fscanf(fp_img, "%u", &pix_intensity);
		*(dst_img->image_data + pix_position) = (double)(unsigned int)pix_intensity / (double)max_intensity;
	}

	fclose(fp_img);
}