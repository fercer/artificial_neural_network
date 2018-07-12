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

	FILE * fp_img = fopen(string_nodes_list.getNodeValue(0)->getScalarValue(), "rb");
	if (!fp_img)
	{
		printf("<<Error: The file \'%s\' could not be opened>>\n", string_nodes_list.getNodeValue(0)->getScalarValue());
		return;
	}

	char magic_character;
	char magic_number;
	magic_character = fgetc(fp_img);
	magic_number = fgetc(fp_img);

	fclose(fp_img);

	// By now, only PGM files are supported:
	if (magic_character != 'P')
	{
		printf("<<Error: The file \'%s\' is not a PGM file>>\n", string_nodes_list.getNodeValue(0)->getScalarValue());
		return;
	}

	switch((int)magic_number)
	{
	case (int)'5':
		loadPGM_raw(string_nodes_list.getNodeValue(0)->getScalarValue());
		break;

	case (int)'2':
		loadPGM_ascii(string_nodes_list.getNodeValue(0)->getScalarValue());
		break;

	default:
		printf("<<Error: The PGM file has an unknown codification format>>\n");
		break;
	}
}



void LOAD_IMAGE_OPERATION::loadPGM_ascii(const char * src_filename)
{
	FILE * fp_img = fopen(src_filename, "r");

	char magic_number[3];

	// Read magic number:
	fgets(magic_number, 3, fp_img);

	// Read the new line character after the magic number:
	char temporal_characters = fgetc(fp_img);

	// Verify if the file contains a commentary:
	fpos_t image_body_file_position;
	fgetpos(fp_img, &image_body_file_position);

	temporal_characters = fgetc(fp_img);
	if (temporal_characters == '#')
	{
		// The file contains a commentary:
		while (temporal_characters != '\n')
		{
			temporal_characters = fgetc(fp_img);
		}
	}
	else
	{
		fsetpos(fp_img, &image_body_file_position);
	}

	// Read the width, height and maximum intensity of the image:
	int width, height;
	fscanf(fp_img, "%i %i\n", &width, &height);

	if (!dst_img)
	{
		dst_img = createVoidImage(width, height);
	}

	if ((dst_img->width != width) || (dst_img->height != height))
	{
		if (dst_img->image_data_type == IMG_UNSET)
		{
			dst_img->image_data_type = IMG_DOUBLE;
		}
		else if (dst_img->image_data.double_image_data)
		{
			free(dst_img->image_data.double_image_data);
		}
		dst_img->width = width;
		dst_img->height = height;
		dst_img->image_data.double_image_data = (double*)calloc(width * height, sizeof(double));
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

	double max_intensity;
	fscanf(fp_img, "%lf\n", &max_intensity);

	double pix_intensity;
	for (unsigned int pix_position = 0; pix_position < (unsigned int)(width*height); pix_position++)
	{
		fscanf(fp_img, "%lf\n", &pix_intensity);
		*(dst_img->image_data.double_image_data + pix_position) = pix_intensity / max_intensity;
	}

	fclose(fp_img);
}



void LOAD_IMAGE_OPERATION::loadPGM_raw(const char * src_filename)
{
	FILE * fp_img = fopen(src_filename, "rb");
	
	char temporal_characters;
	char magic_number;

	// Read magic number:
	fread(&temporal_characters, sizeof(char), 1, fp_img);
	fread(&magic_number, sizeof(char), 1, fp_img);

	// Read the new line character:
	fread(&temporal_characters, sizeof(char), 1, fp_img);

	// Verify if the file contains a commentary:
	fpos_t image_body_file_position;
	fgetpos(fp_img, &image_body_file_position);

	fread(&temporal_characters, sizeof(char), 1, fp_img);
	if (temporal_characters == '#')
	{
		// The file contains a commentary:
		while (temporal_characters != '\n')
		{
			fread(&temporal_characters, sizeof(char), 1, fp_img);
		}
	}
	else
	{
		fsetpos(fp_img, &image_body_file_position);
	}

	// Read the width, height and maximum intensity of the image:
	char width_string[] = "\0\0\0\0\0\0\0";
	unsigned int string_position = 0;
	do
	{
		fread(width_string + string_position, sizeof(char), 1, fp_img);
	} while (*(width_string + string_position++) != ' ');
	int width = atoi(width_string);

	char height_string[] = "\0\0\0\0\0\0\0";
	string_position = 0;
	do {
		fread(height_string + string_position, sizeof(char), 1, fp_img);
	} while (*(height_string + string_position++) != '\n');
	int height = atoi(height_string);

	if (!dst_img)
	{
		dst_img = createVoidImage(width, height);
	}

	if ((dst_img->width != width) || (dst_img->height != height))
	{
		if (dst_img->image_data_type == IMG_UNSET)
		{
			dst_img->image_data_type = IMG_DOUBLE;
		}
		else if (dst_img->image_data.double_image_data)
		{
			free(dst_img->image_data.double_image_data);
		}
		dst_img->width = width;
		dst_img->height = height;
		dst_img->image_data.double_image_data = (double*)calloc(width * height, sizeof(double));
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


	char max_intensity_string[] = "\0\0\0\0\0\0\0";
	string_position = 0;
	do {
		fread(max_intensity_string + string_position, sizeof(char), 1, fp_img);
	} while (*(max_intensity_string + string_position++) != '\n');
	double max_intensity = atof(max_intensity_string);

	unsigned char * pix_intensities = (unsigned char*)malloc(height * width * sizeof(unsigned char));

	fread(pix_intensities, sizeof(unsigned char), width * height, fp_img);
	fclose(fp_img);


	for (unsigned int pix_position = 0; pix_position < (unsigned int)(width*height); pix_position++)
	{
		*(dst_img->image_data.double_image_data + pix_position) = (double)*(pix_intensities + pix_position) / max_intensity;
	}
	free(pix_intensities);
}
