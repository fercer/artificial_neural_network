#include "text_image_operation_class.h"

TEXT_IMAGE_OPERATION::TEXT_IMAGE_OPERATION()
{
	minimum_input_operations_required = 0;
	input_operations_required = 0;

	input_numeric_nodes_required = 1;
	input_string_nodes_required = 2;

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

	NODE_SCALAR<char*> * local_previous_node_A = new NODE_SCALAR<char*>("NOT-DEFINED");
	local_previous_string_nodes_values_list.assignNodeValue(0, local_previous_node_A);

	NODE_SCALAR<char*> * local_previous_node_B = new NODE_SCALAR<char*>(" ");
	local_previous_string_nodes_values_list.assignNodeValue(1, local_previous_node_B);

	NODE_SCALAR<double> * local_previous_node_C = new NODE_SCALAR<double>(64);
	local_previous_numeric_nodes_values_list.assignNodeValue(0, local_previous_node_C);

	characters_set_was_loaded = false;
	character_set = NULL;
}



TEXT_IMAGE_OPERATION::TEXT_IMAGE_OPERATION(const TEXT_IMAGE_OPERATION & src_text_image_operation)
{
	this->minimum_input_operations_required = 0;
	this->input_operations_required = 0;

	this->input_numeric_nodes_required = 1;
	this->input_string_nodes_required = 2;

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

	NODE_SCALAR<char*> * local_previous_node_A = new NODE_SCALAR<char*>("NOT-DEFINED");
	this->local_previous_string_nodes_values_list.assignNodeValue(0, local_previous_node_A);

	NODE_SCALAR<char*> * local_previous_node_B = new NODE_SCALAR<char*>(" ");
	this->local_previous_string_nodes_values_list.assignNodeValue(1, local_previous_node_B);

	NODE_SCALAR<double> * local_previous_node_C = new NODE_SCALAR<double>(64);
	this->local_previous_numeric_nodes_values_list.assignNodeValue(0, local_previous_node_C);

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
	const unsigned int max_characters_per_line = (unsigned int)floor(numeric_nodes_list.getNodeValue(0)->getScalarValue());

	do 
	{
		if (character_position >= max_characters_per_line)
		{
			current_position_y += max_height;
			current_position_x = 0;
		}

		const int character_code = ((unsigned int)*(text_pointer + character_position) - 32);
		current_position_x += (*(character_set + character_code))->width;
		addCharacter(current_position_x, current_position_y, character_code);
	} while (++character_position < characters_count);

}



void TEXT_IMAGE_OPERATION::loadCharacters()
{
	if (characters_set_was_loaded)
	{
		return;
	}


	SAVE_IMAGE_SCALAR_OPERATION save_img;
	save_img.assignNodeValue(0, "test.pgm");

	unsigned int path_length = strlen(string_nodes_list.getNodeValue(0)->getScalarValue()) + 13 + 1;
	char * char_image_filename = (char*)calloc(path_length, sizeof(char));
	character_set = (IMG_DATA**)malloc(224 * sizeof(IMG_DATA*));

	max_height = 0;
	for (unsigned int char_id = 32; char_id < 256; char_id++)
	{
		sprintf(char_image_filename, "%s/char_%i.pgm", string_nodes_list.getNodeValue(0)->getScalarValue(), char_id);
		character_loader.assignNodeValue(0, char_image_filename);
		*(character_set + char_id - 32) = createFromImageData(character_loader.getImageData());
		if (max_height < (*(character_set + char_id - 32))->height)
		{
			max_height = (*(character_set + char_id - 32))->height;
		}
		if (char_id == 57)
		{
			save_img.setInputOperation(&character_loader);
			save_img.getScalarValue();
		}
	}

	free(char_image_filename);

	characters_set_was_loaded = true;
}



void TEXT_IMAGE_OPERATION::addCharacter(const unsigned int src_position_x, const unsigned int src_position_y, const unsigned int src_character_code)
{
	const unsigned int character_width = (*(character_set + src_character_code))->width;
	const unsigned int character_height = (*(character_set + src_character_code))->height;
	for (unsigned int y = 0; y < character_height; y++)
	{
		for (unsigned int x = 0; x < character_width; x++)
		{
			*(dst_img->image_data + (src_position_y + y) * computable_width + src_position_x + x) =
				*((*(character_set + src_character_code))->image_data + y * character_width + x);
		}
	}
}



void TEXT_IMAGE_OPERATION::computeCanvas()
{
	unsigned int current_line_width = 0;
	unsigned int canvas_max_width = 0;
	unsigned int canvas_height = max_height;
	unsigned int character_position = 0;
	const unsigned int max_characters_per_line = (unsigned int)floor(numeric_nodes_list.getNodeValue(0)->getScalarValue());
	char * text_pointer = string_nodes_list.getNodeValue(1)->getScalarValue();
	const unsigned int characters_count = (unsigned int)strlen(text_pointer);

	do
	{
		if (character_position >= max_characters_per_line)
		{
			canvas_height += max_height;

			if (canvas_max_width < current_line_width)
			{
				canvas_max_width = current_line_width;
			}

			current_line_width = 0;
		}

		current_line_width += (*(character_set + ((int)*(text_pointer + character_position) - 32)))->width;
	} while (++character_position < characters_count);

	if (canvas_max_width < current_line_width)
	{
		canvas_max_width = current_line_width;
	}

	computable_width = canvas_max_width;
	computable_height = canvas_height;
}