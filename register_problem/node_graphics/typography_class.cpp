#include "typography_class.h"

TYPOGRAPHY_CLASS::TYPOGRAPHY_CLASS()
{
	triangles_count = 0;
	vertices_positions = NULL;
	color_values = NULL;
	uv_values = NULL;
	normal_vectors = NULL;

	text_box = NULL;

	characters_map = NULL;
	character_map_was_loaded = false;

	characters_boundaries_map_filename = NULL;
	uv_character_values = (GLfloat*)malloc(224 * 12 * sizeof(GLfloat));
	
	character_boundaries_L = (GLfloat*)malloc(224 * sizeof(GLfloat));
	character_boundaries_R = (GLfloat*)malloc(224 * sizeof(GLfloat));
	character_boundaries_T = (GLfloat*)malloc(224 * sizeof(GLfloat));
	character_boundaries_B = (GLfloat*)malloc(224 * sizeof(GLfloat));

	character_offset_y = (GLfloat*)malloc(224 * sizeof(GLfloat));

	horizontal_spacing = 0.01f;
	vertical_spacing = 0.01f;
	max_text_width = 2.0f;
	max_text_height = 0.15f;


	r_channel_bg = 255;
	g_channel_bg = 255;
	b_channel_bg = 255;

	r_channel_font = 0;
	g_channel_font = 0;
	b_channel_font = 0;

	initializeVerticesPositions();
	initializeColorValues();
	initializeUVValues();
}



TYPOGRAPHY_CLASS::TYPOGRAPHY_CLASS(const TYPOGRAPHY_CLASS& src_node_figure)
{
	this->triangles_count = 0;
	this->vertices_positions = NULL;
	this->color_values = NULL;
	this->uv_values = NULL;
	this->normal_vectors = NULL;

	this->text_box = NULL;

	this->characters_map = NULL;
	this->character_map_was_loaded = false;

	this->characters_boundaries_map_filename = NULL;
	this->uv_character_values = (GLfloat*)malloc(224 * 12 * sizeof(GLfloat));

	this->character_boundaries_L = (GLfloat*)malloc(224 * sizeof(GLfloat));
	this->character_boundaries_R = (GLfloat*)malloc(224 * sizeof(GLfloat));
	this->character_boundaries_T = (GLfloat*)malloc(224 * sizeof(GLfloat));
	this->character_boundaries_B = (GLfloat*)malloc(224 * sizeof(GLfloat));

	this->character_offset_y = (GLfloat*)malloc(224 * sizeof(GLfloat));

	this->character_map_was_loaded = src_node_figure.character_map_was_loaded;

	this->setCharactersMapFilename(src_node_figure.characters_boundaries_map_filename);
	
	this->horizontal_spacing = src_node_figure.horizontal_spacing;
	this->vertical_spacing = src_node_figure.vertical_spacing;
	this->max_text_width = src_node_figure.max_text_width;
	this->max_text_height = src_node_figure.max_text_height;

	if (src_node_figure.characters_map)
	{
		this->characters_map = createFromImageData(src_node_figure.characters_map);
	}

	if (this->character_map_was_loaded)
	{
		this->character_max_width = src_node_figure.character_max_width;
		this->character_max_height = src_node_figure.character_max_height;

		memcpy(this->uv_character_values, src_node_figure.uv_character_values, 224 * 12 * sizeof(GLfloat));

		memcpy(this->character_boundaries_L, src_node_figure.character_boundaries_L, 224 * sizeof(GLfloat));
		memcpy(this->character_boundaries_R, src_node_figure.character_boundaries_R, 224 * sizeof(GLfloat));
		memcpy(this->character_boundaries_T, src_node_figure.character_boundaries_T, 224 * sizeof(GLfloat));
		memcpy(this->character_boundaries_B, src_node_figure.character_boundaries_B, 224 * sizeof(GLfloat));
	}

	this->copyFigure2d(src_node_figure);
}



TYPOGRAPHY_CLASS TYPOGRAPHY_CLASS::operator=(const TYPOGRAPHY_CLASS& src_node_figure)
{
	if (this != &src_node_figure)
	{
		this->character_map_was_loaded = src_node_figure.character_map_was_loaded;
		
		this->setText(src_node_figure.text_box);
		this->setCharactersMapFilename(src_node_figure.characters_boundaries_map_filename);
		
		this->horizontal_spacing = src_node_figure.horizontal_spacing;
		this->vertical_spacing = src_node_figure.vertical_spacing;
		this->max_text_width = src_node_figure.max_text_width;
		this->max_text_height = src_node_figure.max_text_height;

		if (src_node_figure.characters_map && !this->characters_map)
		{
			this->characters_map = createFromImageData(src_node_figure.characters_map);
		}
		else if (src_node_figure.characters_map && !this->characters_map)
		{
			copyImageData(src_node_figure.characters_map, this->characters_map);
		}
		else
		{
			freeImageData(this->characters_map);
			this->characters_map = NULL;
		}

		if (this->character_map_was_loaded)
		{
			this->character_max_width = src_node_figure.character_max_width;
			this->character_max_height = src_node_figure.character_max_height;

			memcpy(this->uv_character_values, src_node_figure.uv_character_values, 224 * 12 * sizeof(GLfloat));

			memcpy(this->character_boundaries_L, src_node_figure.character_boundaries_L, 224 * sizeof(GLfloat));
			memcpy(this->character_boundaries_R, src_node_figure.character_boundaries_R, 224 * sizeof(GLfloat));
			memcpy(this->character_boundaries_T, src_node_figure.character_boundaries_T, 224 * sizeof(GLfloat));
			memcpy(this->character_boundaries_B, src_node_figure.character_boundaries_B, 224 * sizeof(GLfloat));
		}

		this->copyFigure2d(src_node_figure);
	}

	return *this;
}



TYPOGRAPHY_CLASS::~TYPOGRAPHY_CLASS()
{
	if (characters_map)
	{
		freeImageData(characters_map);
	}

	if (characters_boundaries_map_filename)
	{
		free(characters_boundaries_map_filename);
	}

	if (text_box)
	{
		free(text_box);
	}

	free(uv_character_values);
	free(character_offset_y);

	free(character_boundaries_L);
	free(character_boundaries_R);
	free(character_boundaries_T);
	free(character_boundaries_B);
}



void TYPOGRAPHY_CLASS::setText(const char * src_text)
{
	initializeVerticesPositions();
	initializeColorValues();
	initializeUVValues();

	if (text_box)
	{
		free(text_box);
		text_box = NULL;
	}

	if (!src_text)
	{
		return;
	}

	unsigned int filename_length = (unsigned int)strlen(src_text);
	text_box = (char*)calloc(filename_length + 1, sizeof(char));

	strcpy(text_box, src_text);

	if (filename_length * 2 != triangles_count)
	{
		if (vertices_positions)
		{
			free(vertices_positions);
		}

		if (color_values)
		{
			free(color_values);
		}

		if (uv_values)
		{
			free(uv_values);
		}

		if (normal_vectors)
		{
			free(normal_vectors);
		}
	}

	triangles_count = filename_length * 2;
	vertices_positions = (glm::vec3*)malloc(3 * triangles_count * sizeof(glm::vec3));
	color_values = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));
	uv_values = (GLfloat*)malloc(2 * 3 * triangles_count * sizeof(GLfloat));
	normal_vectors = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));

	glm::vec3 * vertices_positions_ptr = vertices_positions;
	GLfloat * color_values_ptr = color_values;
	GLfloat * uv_values_ptr = uv_values;
	GLfloat * normal_vectors_ptr = normal_vectors;

	const GLfloat character_mapped_max_height = max_text_height;
	const GLfloat character_mapped_max_width = character_mapped_max_height * (GLfloat)character_max_width / (GLfloat)character_max_height;

	GLfloat offset_x = 0.0f;
	GLfloat offset_y = 0.0f;

	GLfloat max_textbox_width = 0.0f;
	GLfloat max_textbox_height = -character_mapped_max_height;

	for (unsigned int char_position = 0; char_position < filename_length; char_position++)
	{
		const unsigned int char_id = (unsigned int)(unsigned char)*(text_box + char_position) - 32;
		GLfloat current_character_mapped_width = character_mapped_max_width *
			(GLfloat)(*(character_boundaries_R + char_id) - *(character_boundaries_L + char_id) + 1) /
			(GLfloat)character_max_width;

		GLfloat current_character_mapped_height = character_mapped_max_height *
			(GLfloat)(*(character_boundaries_B + char_id) - *(character_boundaries_T + char_id) + 1) /
			(GLfloat)character_max_height;

		const GLfloat current_character_offset_y = *(character_offset_y + char_id) / (GLfloat)character_max_height * character_mapped_max_height;

		if ((offset_x + current_character_mapped_width) > max_text_width)
		{
			if (max_textbox_width < offset_x)
			{
				max_textbox_width = offset_x;
			}

			offset_x = 0.0f;
			offset_y -= max_text_height;

			if (max_textbox_height > offset_y)
			{
				max_textbox_height = offset_y;
			}
		}

		*(vertices_positions_ptr++) = glm::vec3(offset_x, offset_y - current_character_mapped_height - current_character_offset_y, 0.0f);
		*(vertices_positions_ptr++) = glm::vec3(offset_x, offset_y - current_character_offset_y, 0.0f);
		*(vertices_positions_ptr++) = glm::vec3(offset_x + current_character_mapped_width, offset_y - current_character_offset_y, 0.0f);

		*(vertices_positions_ptr++) = glm::vec3(offset_x, offset_y - current_character_mapped_height - current_character_offset_y, 0.0f);
		*(vertices_positions_ptr++) = glm::vec3(offset_x + current_character_mapped_width, offset_y - current_character_mapped_height - current_character_offset_y, 0.0f);
		*(vertices_positions_ptr++) = glm::vec3(offset_x + current_character_mapped_width, offset_y - current_character_offset_y, 0.0f);
		

		*(color_values_ptr++) = 1.0f;
		*(color_values_ptr++) = 1.0f;
		*(color_values_ptr++) = 1.0f;

		*(color_values_ptr++) = 1.0f;
		*(color_values_ptr++) = 1.0f;
		*(color_values_ptr++) = 1.0f;

		*(color_values_ptr++) = 1.0f;
		*(color_values_ptr++) = 1.0f;
		*(color_values_ptr++) = 1.0f;

		*(color_values_ptr++) = 1.0f;
		*(color_values_ptr++) = 1.0f;
		*(color_values_ptr++) = 1.0f;

		*(color_values_ptr++) = 1.0f;
		*(color_values_ptr++) = 1.0f;
		*(color_values_ptr++) = 1.0f;

		*(color_values_ptr++) = 1.0f;
		*(color_values_ptr++) = 1.0f;
		*(color_values_ptr++) = 1.0f;


		*(normal_vectors_ptr++) = 0.0f;
		*(normal_vectors_ptr++) = 0.0f;
		*(normal_vectors_ptr++) = 1.0f;

		*(normal_vectors_ptr++) = 0.0f;
		*(normal_vectors_ptr++) = 0.0f;
		*(normal_vectors_ptr++) = 1.0f;

		*(normal_vectors_ptr++) = 0.0f;
		*(normal_vectors_ptr++) = 0.0f;
		*(normal_vectors_ptr++) = 1.0f;

		*(normal_vectors_ptr++) = 0.0f;
		*(normal_vectors_ptr++) = 0.0f;
		*(normal_vectors_ptr++) = 1.0f;

		*(normal_vectors_ptr++) = 0.0f;
		*(normal_vectors_ptr++) = 0.0f;
		*(normal_vectors_ptr++) = 1.0f;

		*(normal_vectors_ptr++) = 0.0f;
		*(normal_vectors_ptr++) = 0.0f;
		*(normal_vectors_ptr++) = 1.0f;


		*(uv_values_ptr++) = *(uv_character_values + 12 * char_id);
		*(uv_values_ptr++) = *(uv_character_values + 12 * char_id + 1);
		*(uv_values_ptr++) = *(uv_character_values + 12 * char_id + 2);
		*(uv_values_ptr++) = *(uv_character_values + 12 * char_id + 3);
		*(uv_values_ptr++) = *(uv_character_values + 12 * char_id + 4);
		*(uv_values_ptr++) = *(uv_character_values + 12 * char_id + 5);
		*(uv_values_ptr++) = *(uv_character_values + 12 * char_id + 6);
		*(uv_values_ptr++) = *(uv_character_values + 12 * char_id + 7);
		*(uv_values_ptr++) = *(uv_character_values + 12 * char_id + 8);
		*(uv_values_ptr++) = *(uv_character_values + 12 * char_id + 9);
		*(uv_values_ptr++) = *(uv_character_values + 12 * char_id + 10);
		*(uv_values_ptr++) = *(uv_character_values + 12 * char_id + 11);

		offset_x += current_character_mapped_width;
	}

	if (max_textbox_width < offset_x)
	{
		max_textbox_width = offset_x;
	}

	glm::mat4 centering_matrix = glm::mat4(1.0f);
	centering_matrix[0].w = -max_textbox_width / 2.0f;
	centering_matrix[1].w = -max_textbox_height / 2.0f;
	 
	// Center the text box accroding to the width and height of the bounding box:
	vertices_positions_ptr = vertices_positions;
	for (unsigned int char_position = 0; char_position < filename_length; char_position++)
	{
		*(vertices_positions_ptr) = glm::vec4(*(vertices_positions_ptr), 1.0f) * centering_matrix;
		vertices_positions_ptr++;

		*(vertices_positions_ptr) = glm::vec4(*(vertices_positions_ptr), 1.0f) * centering_matrix;
		vertices_positions_ptr++;

		*(vertices_positions_ptr) = glm::vec4(*(vertices_positions_ptr), 1.0f) * centering_matrix;
		vertices_positions_ptr++;

		*(vertices_positions_ptr) = glm::vec4(*(vertices_positions_ptr), 1.0f) * centering_matrix;
		vertices_positions_ptr++;

		*(vertices_positions_ptr) = glm::vec4(*(vertices_positions_ptr), 1.0f) * centering_matrix;
		vertices_positions_ptr++;

		*(vertices_positions_ptr) = glm::vec4(*(vertices_positions_ptr), 1.0f) * centering_matrix;
		vertices_positions_ptr++;
	}
}



void TYPOGRAPHY_CLASS::setCharactersMapFilename(const char * src_characters_boundaries_map_filename)
{
	if (characters_boundaries_map_filename)
	{
		free(characters_boundaries_map_filename);
		characters_boundaries_map_filename = NULL;
	}

	if (!src_characters_boundaries_map_filename)
	{
		return;
	}

	unsigned int filename_length = (unsigned int)strlen(src_characters_boundaries_map_filename) + 1;
	characters_boundaries_map_filename = (char*)malloc(filename_length * sizeof(char));

	strcpy(characters_boundaries_map_filename, src_characters_boundaries_map_filename);
}

void TYPOGRAPHY_CLASS::setHorizontalSpacing(const float src_horizontal_spacing)
{
	horizontal_spacing = (GLfloat)src_horizontal_spacing;
}

void TYPOGRAPHY_CLASS::setVerticalSpacing(const float src_vertical_spacing)
{
	vertical_spacing = (GLfloat)src_vertical_spacing;
}

void TYPOGRAPHY_CLASS::setBoundingBox(const float src_max_width, const float src_max_height)
{
	max_text_width = (GLfloat)src_max_width;
	max_text_height = (GLfloat)src_max_height;
}

void TYPOGRAPHY_CLASS::setFontColor(const unsigned char src_r_channel, const unsigned char src_g_channel, const unsigned char src_b_channel)
{
	r_channel_font = src_r_channel;
	g_channel_font = src_g_channel;
	b_channel_font = src_b_channel;

	if (!texture_data)
	{
		return;
	}

	if (!characters_map)
	{
		characters_map = createFromImageData(texture_data);
	}

	if (color_channels == 3)
	{
		for (unsigned int i = 0; i <  texture_data->width*texture_data->height; i+=3)
		{
			if (*(characters_map->image_data.unsigned_character_image_data + i) == 0 && *(characters_map->image_data.unsigned_character_image_data + i + 1) == 0 && *(characters_map->image_data.unsigned_character_image_data + i + 2) == 255)
			{
				*(texture_data->image_data.unsigned_character_image_data + i) = r_channel_font;
				*(texture_data->image_data.unsigned_character_image_data + i + 1) = g_channel_font;
				*(texture_data->image_data.unsigned_character_image_data + i + 2) = b_channel_font;
			}
		}
	}
	else
	{
		for (unsigned int i = 0; i < texture_data->width*texture_data->height; i++)
		{
			if (*(characters_map->image_data.unsigned_character_image_data + i) == 255)
			{
				*(texture_data->image_data.unsigned_character_image_data + i) = r_channel_font;
			}
		}
	}
}



void TYPOGRAPHY_CLASS::setBackgroundColor(const unsigned char src_r_channel, const unsigned char src_g_channel, const unsigned char src_b_channel)
{
	r_channel_bg = src_r_channel;
	g_channel_bg = src_g_channel;
	b_channel_bg = src_b_channel;

	if (!texture_data)
	{
		return;
	}

	if (!characters_map)
	{
		characters_map = createFromImageData(texture_data);
	}

	if (color_channels == 3)
	{
		for (unsigned int i = 0; i < texture_data->width*texture_data->height; i += 3)
		{
			if (*(characters_map->image_data.unsigned_character_image_data + i) == 0 && *(characters_map->image_data.unsigned_character_image_data + i + 1) == 255 && *(characters_map->image_data.unsigned_character_image_data + i + 2) == 0)
			{
				*(texture_data->image_data.unsigned_character_image_data + i) = r_channel_bg;
				*(texture_data->image_data.unsigned_character_image_data + i + 1) = g_channel_bg;
				*(texture_data->image_data.unsigned_character_image_data + i + 2) = b_channel_bg;
			}
		}
	}
	else
	{
		for (unsigned int i = 0; i < texture_data->width*texture_data->height; i++)
		{
			if (*(characters_map->image_data.unsigned_character_image_data + i) == 0)
			{
				*(texture_data->image_data.unsigned_character_image_data + i) = r_channel_bg;
			}
		}
	}
}



void TYPOGRAPHY_CLASS::initializeVerticesPositions()
{
	loadCharacterMap();
}


void TYPOGRAPHY_CLASS::initializeColorValues()
{
	loadCharacterMap();
}


void TYPOGRAPHY_CLASS::initializeUVValues()
{
	loadCharacterMap();
}


void TYPOGRAPHY_CLASS::initializeNormalVectors()
{
	loadCharacterMap();
}


void TYPOGRAPHY_CLASS::loadCharacterMap()
{
	if (!characters_boundaries_map_filename || character_map_was_loaded)
	{
		return;
	}

	FILE * fp_character_boundary_map = fopen(characters_boundaries_map_filename, "r");
	if (!fp_character_boundary_map)
	{
		fprintf(stderr, "<<Error: The characters boundaries map file: \'%s\' could not be opened>>\n", characters_boundaries_map_filename);
		return;
	}

	char header[512];
	// Read the file header, it must contain the name of each column:
	fgets(header, 512, fp_character_boundary_map);

	GLfloat* uv_character_values_ptr = uv_character_values;
	GLfloat* character_offset_y_ptr = character_offset_y;
	GLfloat* character_boundaries_L_ptr = character_boundaries_L;
	GLfloat* character_boundaries_T_ptr = character_boundaries_T;
	GLfloat* character_boundaries_R_ptr = character_boundaries_R;
	GLfloat* character_boundaries_B_ptr = character_boundaries_B;
	
	character_max_width = 0;
	character_max_height = 0;

	for (unsigned int char_id = 0; char_id < 224; char_id++)
	{
		// Read the character code:
		fscanf(fp_character_boundary_map, "%s %f %f %f %f %f", header,
			character_boundaries_L_ptr, character_boundaries_T_ptr, character_boundaries_R_ptr, character_boundaries_B_ptr, character_offset_y_ptr++);

		const unsigned int current_character_width = *character_boundaries_R_ptr - *character_boundaries_L_ptr;
		const unsigned int current_character_height = *character_boundaries_B_ptr - *character_boundaries_T_ptr;

		if (character_max_width < current_character_width)
		{
			character_max_width = current_character_width;
		}

		if (character_max_height < current_character_height)
		{
			character_max_height = current_character_height;
		}

		character_boundaries_L_ptr++;
		character_boundaries_T_ptr++;
		character_boundaries_R_ptr++;
		character_boundaries_B_ptr++;

		// U coordinate 1
		fscanf(fp_character_boundary_map, "%f", uv_character_values_ptr);
		// V coordinate 1
		fscanf(fp_character_boundary_map, "%f", ++uv_character_values_ptr);

		// U coordinate 2
		fscanf(fp_character_boundary_map, "%f", ++uv_character_values_ptr);
		// V coordinate 2
		fscanf(fp_character_boundary_map, "%f", ++uv_character_values_ptr);
		
		// U coordinate 3
		fscanf(fp_character_boundary_map, "%f", ++uv_character_values_ptr);
		// V coordinate 3
		fscanf(fp_character_boundary_map, "%f", ++uv_character_values_ptr);

		// U coordinate 4
		fscanf(fp_character_boundary_map, "%f", ++uv_character_values_ptr);
		// V coordinate 4
		fscanf(fp_character_boundary_map, "%f", ++uv_character_values_ptr);

		// U coordinate 5
		fscanf(fp_character_boundary_map, "%f", ++uv_character_values_ptr);
		// V coordinate 5
		fscanf(fp_character_boundary_map, "%f", ++uv_character_values_ptr);

		// U coordinate 5
		fscanf(fp_character_boundary_map, "%f", ++uv_character_values_ptr);
		// V coordinate 5
		fscanf(fp_character_boundary_map, "%f", ++uv_character_values_ptr);
		uv_character_values_ptr++;
	}
	character_max_width++;
	character_max_height++;

	fclose(fp_character_boundary_map);

	character_map_was_loaded = true;
}
