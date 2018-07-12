#include "text_image_operation_class.h"

TEXT_IMAGE_OPERATION::TEXT_IMAGE_OPERATION()
{
	minimum_input_operations_required = 0;
	input_operations_required = 0;

	input_numeric_nodes_required = 2;
	input_string_nodes_required = 3;

	NODE_SCALAR<char*> * node_filename_identifier = new NODE_SCALAR<char*> ("directory");
	string_nodes_names_list.assignNodeValue(0, node_filename_identifier);

	NODE_SCALAR<char*> * local_node_A = new NODE_SCALAR<char*>("NOT-DEFINED");
	local_string_nodes_list.assignNodeValue(0, local_node_A);
	string_nodes_list.assignNodeValue(0, local_string_nodes_list.getNodeValue(0));
	string_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<char*> * node_text_identifier = new NODE_SCALAR<char*>("text");
	string_nodes_names_list.assignNodeValue(1, node_text_identifier);

	NODE_SCALAR<char*> * local_node_B = new NODE_SCALAR<char*>(" ");
	local_string_nodes_list.assignNodeValue(1, local_node_B);
	string_nodes_list.assignNodeValue(1, local_string_nodes_list.getNodeValue(1));
	string_node_is_local_list.assignNodeValue(1, true);
	
	NODE_SCALAR<char*> * node_width_identifier = new NODE_SCALAR<char*>("characters_per_line");
	numeric_nodes_names_list.assignNodeValue(0, node_width_identifier);

	NODE_SCALAR<double> * local_node_C = new NODE_SCALAR<double>(64);
	local_numeric_nodes_list.assignNodeValue(0, local_node_C);
	numeric_nodes_list.assignNodeValue(0, local_numeric_nodes_list.getNodeValue(0));
	numeric_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<char*> * node_character_spacing_identifier = new NODE_SCALAR<char*>("character_spacing");
	numeric_nodes_names_list.assignNodeValue(1, node_character_spacing_identifier);

	NODE_SCALAR<double> * local_node_D = new NODE_SCALAR<double>(5);
	local_numeric_nodes_list.assignNodeValue(1, local_node_D);
	numeric_nodes_list.assignNodeValue(1, local_numeric_nodes_list.getNodeValue(1));
	numeric_node_is_local_list.assignNodeValue(1, true);

	NODE_SCALAR<char*> * node_character_dimension_data_identifier =
		new NODE_SCALAR<char*>("character_dimension_data");
	string_nodes_names_list.assignNodeValue(2, node_character_dimension_data_identifier);

	NODE_SCALAR<char*> * local_node_E = new NODE_SCALAR<char*>("NOT-DEFINED");
	local_string_nodes_list.assignNodeValue(2, local_node_E);
	string_nodes_list.assignNodeValue(2, local_string_nodes_list.getNodeValue(2));
	string_node_is_local_list.assignNodeValue(2, true);


	NODE_SCALAR<char*> * local_previous_node_A = new NODE_SCALAR<char*>("NOT-DEFINED");
	local_previous_string_nodes_values_list.assignNodeValue(0, local_previous_node_A);

	NODE_SCALAR<char*> * local_previous_node_B = new NODE_SCALAR<char*>(" ");
	local_previous_string_nodes_values_list.assignNodeValue(1, local_previous_node_B);

	NODE_SCALAR<double> * local_previous_node_C = new NODE_SCALAR<double>(64);
	local_previous_numeric_nodes_values_list.assignNodeValue(0, local_previous_node_C);

	NODE_SCALAR<double> * local_previous_node_D = new NODE_SCALAR<double>(5);
	local_previous_numeric_nodes_values_list.assignNodeValue(1, local_previous_node_D);

	NODE_SCALAR<char*> * local_previous_node_E = new NODE_SCALAR<char*>("NOT-DEFINED");
	local_previous_string_nodes_values_list.assignNodeValue(2, local_previous_node_E);

	characters_set_was_loaded = false;
	character_set = NULL;
}



TEXT_IMAGE_OPERATION::TEXT_IMAGE_OPERATION(const TEXT_IMAGE_OPERATION & src_text_image_operation)
{
	this->minimum_input_operations_required = 0;
	this->input_operations_required = 0;

	input_numeric_nodes_required = 2;
	input_string_nodes_required = 3;

	NODE_SCALAR<char*> * node_filename_identifier = new NODE_SCALAR<char*>("directory");
	this->string_nodes_names_list.assignNodeValue(0, node_filename_identifier);

	NODE_SCALAR<char*> * local_node_A = new NODE_SCALAR<char*>("NOT-DEFINED");
	this->local_string_nodes_list.assignNodeValue(0, local_node_A);
	this->string_nodes_list.assignNodeValue(0, this->local_string_nodes_list.getNodeValue(0));
	this->string_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<char*> * node_text_identifier = new NODE_SCALAR<char*>("text");
	this->string_nodes_names_list.assignNodeValue(1, node_text_identifier);

	NODE_SCALAR<char*> * local_node_B = new NODE_SCALAR<char*>(" ");
	this->local_string_nodes_list.assignNodeValue(1, local_node_B);
	this->string_nodes_list.assignNodeValue(1, this->local_string_nodes_list.getNodeValue(1));
	this->string_node_is_local_list.assignNodeValue(1, true);

	NODE_SCALAR<char*> * node_width_identifier = new NODE_SCALAR<char*>("characters_per_line");
	this->numeric_nodes_names_list.assignNodeValue(0, node_width_identifier);

	NODE_SCALAR<double> * local_node_C = new NODE_SCALAR<double>(64);
	this->local_numeric_nodes_list.assignNodeValue(0, local_node_C);
	this->numeric_nodes_list.assignNodeValue(0, this->local_numeric_nodes_list.getNodeValue(0));
	this->numeric_node_is_local_list.assignNodeValue(0, true);

	NODE_SCALAR<char*> * node_character_spacing_identifier = new NODE_SCALAR<char*>("character_spacing");
	this->numeric_nodes_names_list.assignNodeValue(1, node_character_spacing_identifier);

	NODE_SCALAR<double> * local_node_D = new NODE_SCALAR<double>(5);
	this->local_numeric_nodes_list.assignNodeValue(1, local_node_D);
	this->numeric_nodes_list.assignNodeValue(1, this->local_numeric_nodes_list.getNodeValue(1));
	this->numeric_node_is_local_list.assignNodeValue(1, true);

	NODE_SCALAR<char*> * node_character_dimension_data_identifier =
		new NODE_SCALAR<char*>("character_dimension_data");
	this->string_nodes_names_list.assignNodeValue(2, node_character_dimension_data_identifier);

	NODE_SCALAR<char*> * local_node_E = new NODE_SCALAR<char*>("NOT-DEFINED");
	this->local_string_nodes_list.assignNodeValue(2, local_node_E);
	this->string_nodes_list.assignNodeValue(2, this->local_string_nodes_list.getNodeValue(2));
	this->string_node_is_local_list.assignNodeValue(2, true);


	NODE_SCALAR<char*> * local_previous_node_A = new NODE_SCALAR<char*>("NOT-DEFINED");
	this->local_previous_string_nodes_values_list.assignNodeValue(0, local_previous_node_A);

	NODE_SCALAR<char*> * local_previous_node_B = new NODE_SCALAR<char*>(" ");
	this->local_previous_string_nodes_values_list.assignNodeValue(1, local_previous_node_B);

	NODE_SCALAR<double> * local_previous_node_C = new NODE_SCALAR<double>(64);
	this->local_previous_numeric_nodes_values_list.assignNodeValue(0, local_previous_node_C);

	NODE_SCALAR<double> * local_previous_node_D = new NODE_SCALAR<double>(5);
	this->local_previous_numeric_nodes_values_list.assignNodeValue(1, local_previous_node_D);

	NODE_SCALAR<char*> * local_previous_node_E = new NODE_SCALAR<char*>("NOT-DEFINED");
	this->local_previous_string_nodes_values_list.assignNodeValue(2, local_previous_node_E);

	this->characters_set_was_loaded = src_text_image_operation.characters_set_was_loaded;
	if (this->characters_set_was_loaded)
	{
		this->character_set = (IMG_DATA**)malloc(224 * sizeof(IMG_DATA*));

		for (unsigned int char_id = 0; char_id < 224; char_id++)
		{
			*(this->character_set + char_id) = createFromImageData(*(src_text_image_operation.character_set + char_id));
		}

		this->max_height = src_text_image_operation.max_height;
	}
	else
	{
		this->character_set = NULL;
	}

	this->copyFromImageOperation(src_text_image_operation);
}



TEXT_IMAGE_OPERATION TEXT_IMAGE_OPERATION::operator=(const TEXT_IMAGE_OPERATION & src_text_image_operation)
{
	if (this != &src_text_image_operation)
	{
		this->characters_set_was_loaded = src_text_image_operation.characters_set_was_loaded;
		if (this->characters_set_was_loaded)
		{
			this->character_set = (IMG_DATA**)malloc(224 * sizeof(IMG_DATA*));

			for (unsigned int char_id = 0; char_id < 224; char_id++)
			{
				*(this->character_set + char_id) = createFromImageData(*(src_text_image_operation.character_set + char_id));
			}
		}
		else
		{
			this->character_set = NULL;
		}

		this->copyFromImageOperation(src_text_image_operation);
	}

	return *this;
}



TEXT_IMAGE_OPERATION::~TEXT_IMAGE_OPERATION()
{
	if (characters_set_was_loaded)
	{
		for (unsigned int char_id = 0; char_id < 224; char_id++)
		{
			freeImageData(*(character_set + char_id));
		}
		free(character_set);
		free(character_dimensions);
	}
}



void TEXT_IMAGE_OPERATION::performOperation()
{
	char * text_pointer = string_nodes_list.getNodeValue(1)->getScalarValue();
	const unsigned int characters_count = (unsigned int)strlen(text_pointer);
	
	if (characters_count < 1)
	{
		return;
	}

	loadCharacters();
	computeCanvas();

	if (dst_img)
	{
		freeImageData(dst_img);
	}

	dst_img = createVoidImage(computable_width, computable_height);

	unsigned int current_position_x = 0;
	unsigned int current_position_y = 0;
	unsigned int character_position = 0;

	const unsigned int character_spacing = (unsigned int)numeric_nodes_list.getNodeValue(1)->getScalarValue();
	const unsigned int max_characters_per_line = (unsigned int)floor(numeric_nodes_list.getNodeValue(0)->getScalarValue());

	do 
	{
		if (((character_position + 1) % (max_characters_per_line+1)) == 0)
		{
			current_position_y += max_height;
			current_position_x = 0;
		}

		const int character_code = ((unsigned int)(unsigned char)*(text_pointer + character_position) - 32);
		addCharacter(current_position_x, current_position_y, character_code);
		const unsigned int min_x = (character_dimensions + character_code)->UL_x;
		const unsigned int max_x = (character_dimensions + character_code)->LR_x;
		const unsigned int current_character_width = max_x - min_x + 1;
		current_position_x += current_character_width + character_spacing;
	} while (++character_position < characters_count);

}



void TEXT_IMAGE_OPERATION::loadCharacters()
{
	if (characters_set_was_loaded)
	{
		return;
	}
	
	/* If the characters' dimensions were passed as an argument, load the information for each image:
	otherwise, use the actual dimension of the images.
	*/
	character_dimensions = (ROI_BBOX *)malloc(224 * sizeof(ROI_BBOX));

	FILE * fp_character_dimensions = NULL;
	if (strcmp(string_nodes_list.getNodeValue(2)->getScalarValue(), "NOT-DEFINED") != 0)
	{
		fp_character_dimensions = fopen(string_nodes_list.getNodeValue(2)->getScalarValue(), "r");

		if (fp_character_dimensions)
		{
			char headers[32];
			// Read the code header:
			fscanf(fp_character_dimensions, "%s", headers);

			// Read the min X header:
			fscanf(fp_character_dimensions, "%s", headers);

			// Read the max X header:
			fscanf(fp_character_dimensions, "%s", headers);

			// Read the min Y header:
			fscanf(fp_character_dimensions, "%s", headers);

			// Read the max Y header:
			fscanf(fp_character_dimensions, "%s", headers);
		}
	}
	unsigned int path_length = (unsigned int) strlen(string_nodes_list.getNodeValue(0)->getScalarValue()) + 13 + 1;
	char * char_image_filename = (char*)calloc(path_length, sizeof(char));
	character_set = (IMG_DATA**)malloc(224 * sizeof(IMG_DATA*));

	max_height = 0;
	for (unsigned int char_id = 32; char_id < 256; char_id++)
	{
		sprintf(char_image_filename, "%s/char_%i.pgm", string_nodes_list.getNodeValue(0)->getScalarValue(), char_id);
		character_loader.assignNodeValue(0, char_image_filename);
		*(character_set + char_id - 32) = createFromImageData(character_loader.getImageData());

		if (fp_character_dimensions)
		{
			int ascii_code, min_x, max_x, min_y, max_y;

			fscanf(fp_character_dimensions, "%i %i %i %i %i\n", &ascii_code, &min_x, &max_x, &min_y, &max_y);

			(character_dimensions + char_id - 32)->UL_x = min_x;
			(character_dimensions + char_id - 32)->UL_y = min_y;

			(character_dimensions + char_id - 32)->UR_x = max_x;
			(character_dimensions + char_id - 32)->UR_y = min_y;

			(character_dimensions + char_id - 32)->LR_x = max_x;
			(character_dimensions + char_id - 32)->LR_y = max_y;

			(character_dimensions + char_id - 32)->LL_x = min_x;
			(character_dimensions + char_id - 32)->LL_y = max_y;
		}
		else
		{
			(character_dimensions + char_id - 32)->UL_x = 0;
			(character_dimensions + char_id - 32)->UL_y = 0;

			(character_dimensions + char_id - 32)->UR_x = (*(character_set + char_id - 32))->width - 1;
			(character_dimensions + char_id - 32)->UR_y = 0;

			(character_dimensions + char_id - 32)->LR_x = (*(character_set + char_id - 32))->width - 1;
			(character_dimensions + char_id - 32)->LR_y = (*(character_set + char_id - 32))->height - 1;

			(character_dimensions + char_id - 32)->LL_x = 0;
			(character_dimensions + char_id - 32)->LL_y = (*(character_set + char_id - 32))->height - 1;
		}

		const unsigned int max_y = (character_dimensions + char_id - 32)->LL_y;

		if (max_height < max_y)
		{
			max_height = max_y;
		}
	}

	free(char_image_filename);

	if (fp_character_dimensions)
	{
		fclose(fp_character_dimensions);
	}

	characters_set_was_loaded = true;
}



void TEXT_IMAGE_OPERATION::addCharacter(const unsigned int src_position_x, const unsigned int src_position_y, const unsigned int src_character_code)
{
	const unsigned int min_x = (character_dimensions + src_character_code)->UL_x;
	const unsigned int max_x = (character_dimensions + src_character_code)->LR_x;
	const unsigned int min_y = (character_dimensions + src_character_code)->UL_y;
	const unsigned int max_y = (character_dimensions + src_character_code)->LR_y;
	const unsigned int character_width = (*(character_set + src_character_code))->width;

	for (unsigned int y = min_y; y <= max_y; y++)
	{
		for (unsigned int x = min_x; x <= max_x; x++)
		{
			*(dst_img->image_data.double_image_data + (src_position_y + y) * computable_width + src_position_x + x - min_x) =
				*((*(character_set + src_character_code))->image_data.double_image_data + y * character_width + x);
		}
	}
}



void TEXT_IMAGE_OPERATION::computeCanvas()
{
	unsigned int current_line_width = 0;
	unsigned int canvas_max_width = 0;
	unsigned int canvas_height = max_height;
	unsigned int character_position = 0;

	const unsigned int character_spacing = (unsigned int)numeric_nodes_list.getNodeValue(1)->getScalarValue();
	const unsigned int max_characters_per_line = (unsigned int)floor(numeric_nodes_list.getNodeValue(0)->getScalarValue());
	char * text_pointer = string_nodes_list.getNodeValue(1)->getScalarValue();
	const unsigned int characters_count = (unsigned int)strlen(text_pointer);

	do
	{
		const unsigned int character_code = ((unsigned int)(unsigned char)*(text_pointer + character_position) - 32);
		if (((character_position + 1) % (max_characters_per_line + 1)) == 0)
		{
			canvas_height += max_height;

			if (canvas_max_width < current_line_width)
			{
				canvas_max_width = current_line_width;
			}

			current_line_width = 0;
		}

		const unsigned int min_x = (character_dimensions + character_code)->UL_x;
		const unsigned int max_x = (character_dimensions + character_code)->LR_x;
		const unsigned int current_character_width = max_x - min_x + 1;

		current_line_width += current_character_width + character_spacing;
	} while (++character_position < characters_count);

	if (canvas_max_width < current_line_width)
	{
		canvas_max_width = current_line_width;
	}

	computable_width = canvas_max_width;
	computable_height = canvas_height;
}