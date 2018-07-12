#include "figure_2d_class.h"

FIGURE_2D::FIGURE_2D()
{
	vertices_positions = NULL;
	color_values = NULL;
	uv_values = NULL;
	normal_vectors = NULL;

	figure_was_moved = false;
	figure_traslation = glm::mat4(1.0f);
	figure_scale = glm::mat4(1.0f);

	triangles_count = 0;

	color_channels = 3;
	texture_data = NULL;
}


FIGURE_2D::FIGURE_2D(const float initial_position_x, const float initial_position_y, const float initial_position_z, const float src_scale)
{
	vertices_positions = NULL;
	color_values = NULL;
	uv_values = NULL;
	normal_vectors = NULL;

	figure_was_moved = false;
	figure_traslation = glm::mat4(1.0f);
	figure_traslation[0].w = initial_position_x;
	figure_traslation[1].w = initial_position_y;
	figure_traslation[2].w = initial_position_z;

	figure_scale = glm::scale(glm::vec3(src_scale, src_scale, src_scale));

	triangles_count = 0;

	color_channels = 3;
	texture_data = NULL;
}

FIGURE_2D::FIGURE_2D(const FIGURE_2D& src_node_figure)
{
	this->vertices_positions = NULL;
	this->color_values = NULL;
	this->uv_values = NULL;
	this->normal_vectors = NULL;

	this->triangles_count = 0;
		
	copyFigure2d(src_node_figure);
}
	
FIGURE_2D::~FIGURE_2D()
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

	if (texture_data)
	{
		freeImageData(texture_data);
	}
}

void FIGURE_2D::moveFigure(const float delta_x, const float delta_y, const float delta_z)
{
	figure_traslation[0].w = delta_x;
	figure_traslation[1].w = delta_y;
	figure_traslation[2].w = delta_z;

	figure_was_moved = true;
}

void FIGURE_2D::scaleFigure(const float src_scale)
{
	figure_scale = glm::scale(glm::vec3(src_scale, src_scale, src_scale));
	figure_was_moved = true;
}

unsigned int FIGURE_2D::getTrianglesCount()
{
	return triangles_count;
}

glm::vec3 * FIGURE_2D::getVerticesPositions()
{
	return vertices_positions;
}

GLfloat * FIGURE_2D::getColorValues()
{
	return color_values;
}

GLfloat * FIGURE_2D::getUVValues()
{
	return uv_values;
}

GLfloat * FIGURE_2D::getNormalVectors()
{
	return normal_vectors;
}

unsigned int FIGURE_2D::getColorChannels()
{
	return color_channels;
}

IMG_DATA * FIGURE_2D::getTextureData()
{
	return texture_data;
}

void FIGURE_2D::loadTexture(const char * src_filename)
{
	FILE * fp_texture = fopen(src_filename, "r");

	if (!fp_texture)
	{
		fprintf(stderr, "<<Error: The texture file \'%s\' could not be opened>>\n", src_filename);
		return;
	}

	char magic_number[3];
	fgets(magic_number, 3, fp_texture);
	fclose(fp_texture);

	if (*magic_number != 'P')
	{
		fprintf(stderr, "<<Error: The texture file \'%s\' is not in PGM or PPM format>>\n", src_filename);
		return;
	}

	switch ((int)*(magic_number + 1))
	{
	case (int)'2':
		loadPGM_ascii(src_filename);
		break;

	case (int)'5':
		loadPGM_raw(src_filename);
		break;

	case (int)'3':
		loadPPM_ascii(src_filename);
		break;

	case (int)'6':
		loadPPM_raw(src_filename);
		break;

	default:
		fprintf(stderr, "<<Error: The texture file \'%s\' is an unkoun PGM or PPM format>>\n", src_filename);
		break;
	}
}


glm::mat4 FIGURE_2D::getPosition()
{
	return figure_traslation;
}


glm::mat4 FIGURE_2D::getScale()
{
	return figure_scale;
}


void FIGURE_2D::copyFigure2d(const FIGURE_2D& src_node_figure)
{
	/* Copy the vertices data */
	if (this->vertices_positions && !src_node_figure.vertices_positions)
	{
		free(this->vertices_positions);
	}
	else if (!this->vertices_positions && src_node_figure.vertices_positions)
	{
		this->vertices_positions = (glm::vec3*)malloc(3 * 3 * src_node_figure.triangles_count * sizeof(glm::vec3));
	}
	else if ((this->vertices_positions && src_node_figure.vertices_positions) &&
		(this->triangles_count != src_node_figure.triangles_count))
	{
		free(this->vertices_positions);
		this->vertices_positions = (glm::vec3*)malloc(3 * 3 * src_node_figure.triangles_count * sizeof(glm::vec3));
	}

	if (src_node_figure.vertices_positions)
	{
		memcpy(this->vertices_positions, src_node_figure.vertices_positions, 3 * 3 * src_node_figure.triangles_count * sizeof(glm::vec3));
	}
	
	/* Copy the color data */
	if (this->color_values && !src_node_figure.color_values)
	{
		free(this->color_values);
	}
	else if (!this->color_values && src_node_figure.color_values)
	{
		this->color_values = (GLfloat*)malloc(3 * 3 * src_node_figure.triangles_count * sizeof(GLfloat));
	}
	else if ((this->color_values && src_node_figure.color_values) &&
		(this->triangles_count != src_node_figure.triangles_count))
	{
		free(this->color_values);
		this->color_values = (GLfloat*)malloc(3 * 3 * src_node_figure.triangles_count * sizeof(GLfloat));
	}

	if (src_node_figure.color_values)
	{
		memcpy(this->color_values, src_node_figure.color_values, 3 * 3 * src_node_figure.triangles_count * sizeof(GLfloat));
	}

	/* Copy the texture data */
	if (this->uv_values && !src_node_figure.color_values)
	{
		free(this->uv_values);
	}
	else if (!this->uv_values && src_node_figure.uv_values)
	{
		this->uv_values = (GLfloat*)malloc(2 * 3 * src_node_figure.triangles_count * sizeof(GLfloat));
	}
	else if ((this->uv_values && src_node_figure.uv_values) &&
		(this->triangles_count != src_node_figure.triangles_count))
	{
		free(this->uv_values);
		this->uv_values = (GLfloat*)malloc(2 * 3 * src_node_figure.triangles_count * sizeof(GLfloat));
	}

	if (src_node_figure.uv_values)
	{
		memcpy(this->uv_values, src_node_figure.uv_values, 2 * 3 * src_node_figure.triangles_count * sizeof(GLfloat));
	}

	if (this->texture_data && src_node_figure.texture_data)
	{
		copyImageData(src_node_figure.texture_data, this->texture_data);
	}
	else if (this->texture_data && !src_node_figure.texture_data)
	{
		freeImageData(this->texture_data);
		this->texture_data = NULL;
	}
	else if (!this->texture_data && src_node_figure.texture_data)
	{
		texture_data = createFromImageData(src_node_figure.texture_data);
	}

	/* Copy the normal data */
	if (this->normal_vectors && !src_node_figure.normal_vectors)
	{
		free(this->normal_vectors);
	}
	else if (!this->normal_vectors && src_node_figure.normal_vectors)
	{
		this->normal_vectors = (GLfloat*)malloc(3 * 3 * src_node_figure.triangles_count * sizeof(GLfloat));
	}
	else if ((this->normal_vectors && src_node_figure.normal_vectors) &&
		(this->triangles_count != src_node_figure.triangles_count))
	{
		free(this->normal_vectors);
		this->normal_vectors = (GLfloat*)malloc(3 * 3 * src_node_figure.triangles_count * sizeof(GLfloat));
	}

	if (src_node_figure.normal_vectors)
	{
		memcpy(this->normal_vectors, src_node_figure.normal_vectors, 3 * 3 * src_node_figure.triangles_count * sizeof(GLfloat));
	}

	this->triangles_count = src_node_figure.triangles_count;

	/* Copy the figure position and scale: */
	this->figure_traslation = src_node_figure.figure_traslation;
	this->figure_scale = src_node_figure.figure_scale;
}


void FIGURE_2D::loadPGM_ascii(const char * src_filename)
{
	color_channels = 1;

	FILE * fp_texture = fopen(src_filename, "r");

	char magic_number[3];

	// Read magic number:
	fgets(magic_number, 3, fp_texture);

	// Read the new line character after the magic number:
	char temporal_characters = fgetc(fp_texture);

	// Verify if the file contains a commentary:
	fpos_t image_body_file_position;
	fgetpos(fp_texture, &image_body_file_position);

	temporal_characters = fgetc(fp_texture);
	if (temporal_characters == '#')
	{
		// The file contains a commentary:
		while (temporal_characters != '\n')
		{
			temporal_characters = fgetc(fp_texture);
		}
	}
	else
	{
		fsetpos(fp_texture, &image_body_file_position);
	}

	// Read the width, height and maximum intensity of the image:
	int width, height;
	fscanf(fp_texture, "%i %i\n", &width, &height);

	double max_intensity;
	fscanf(fp_texture, "%lf\n", &max_intensity);
	
	texture_data = createVoidImage(width, height, IMG_UCHAR);

	unsigned char * pix_intensity = texture_data->image_data.unsigned_character_image_data;
	for (int pix_position = 0; pix_position < width*height; pix_position++)
	{
		fscanf(fp_texture, "%c\n", pix_intensity++);
	}

	fclose(fp_texture);
}



void FIGURE_2D::loadPGM_raw(const char * src_filename)
{
	color_channels = 1;

	FILE * fp_texture = fopen(src_filename, "rb");

	char temporal_characters;
	char magic_number;

	// Read magic number:
	fread(&temporal_characters, sizeof(char), 1, fp_texture);
	fread(&magic_number, sizeof(char), 1, fp_texture);

	// Read the new line character:
	fread(&temporal_characters, sizeof(char), 1, fp_texture);

	// Verify if the file contains a commentary:
	fpos_t image_body_file_position;
	fgetpos(fp_texture, &image_body_file_position);

	fread(&temporal_characters, sizeof(char), 1, fp_texture);
	if (temporal_characters == '#')
	{
		// The file contains a commentary:
		while (temporal_characters != '\n')
		{
			fread(&temporal_characters, sizeof(char), 1, fp_texture);
		}
	}
	else
	{
		fsetpos(fp_texture, &image_body_file_position);
	}

	// Read the width, height and maximum intensity of the image:
	char width_string[] = "\0\0\0\0\0\0\0";
	unsigned int string_position = 0;
	do
	{
		fread(width_string + string_position, sizeof(char), 1, fp_texture);
	} while (*(width_string + string_position++) != ' ');
	int width = atoi(width_string);

	char height_string[] = "\0\0\0\0\0\0\0";
	string_position = 0;
	do {
		fread(height_string + string_position, sizeof(char), 1, fp_texture);
	} while (*(height_string + string_position++) != '\n');
	int height = atoi(height_string);

		
	char max_intensity_string[] = "\0\0\0\0\0\0\0";
	string_position = 0;
	do {
		fread(max_intensity_string + string_position, sizeof(char), 1, fp_texture);
	} while (*(max_intensity_string + string_position++) != '\n');
	double max_intensity = atof(max_intensity_string);
		
	texture_data = createVoidImage(width, height, IMG_UINT);
	fread(texture_data->image_data.unsigned_character_image_data, sizeof(unsigned char), width * height, fp_texture);
	fclose(fp_texture);
}


void FIGURE_2D::loadPPM_ascii(const char * src_filename)
{
	color_channels = 3;

	FILE * fp_texture = fopen(src_filename, "r");

	char magic_number[3];

	// Read magic number:
	fgets(magic_number, 3, fp_texture);

	// Read the new line character after the magic number:
	char temporal_characters = fgetc(fp_texture);

	// Verify if the file contains a commentary:
	fpos_t image_body_file_position;
	fgetpos(fp_texture, &image_body_file_position);

	temporal_characters = fgetc(fp_texture);
	if (temporal_characters == '#')
	{
		// The file contains a commentary:
		while (temporal_characters != '\n')
		{
			temporal_characters = fgetc(fp_texture);
		}
	}
	else
	{
		fsetpos(fp_texture, &image_body_file_position);
	}

	// Read the width, height and maximum intensity of the image:
	int width, height;
	fscanf(fp_texture, "%i %i\n", &width, &height);

	double max_intensity;
	fscanf(fp_texture, "%lf\n", &max_intensity);

	texture_data = createVoidImage(3 * width, height, IMG_UCHAR);

	unsigned char * pix_intensity = texture_data->image_data.unsigned_character_image_data;
	for (int pix_position = 0; pix_position < 3*width*height; pix_position++)
	{
		fscanf(fp_texture, "%c\n", pix_intensity++);
	}

	fclose(fp_texture);
}



void FIGURE_2D::loadPPM_raw(const char * src_filename)
{
	color_channels = 3;

	FILE * fp_texture = fopen(src_filename, "rb");

	char temporal_characters;
	char magic_number;

	// Read magic number:
	fread(&temporal_characters, sizeof(char), 1, fp_texture);
	fread(&magic_number, sizeof(char), 1, fp_texture);

	// Read the new line character:
	fread(&temporal_characters, sizeof(char), 1, fp_texture);

	// Verify if the file contains a commentary:
	fpos_t image_body_file_position;
	fgetpos(fp_texture, &image_body_file_position);

	fread(&temporal_characters, sizeof(char), 1, fp_texture);
	if (temporal_characters == '#')
	{
		// The file contains a commentary:
		while (temporal_characters != '\n')
		{
			fread(&temporal_characters, sizeof(char), 1, fp_texture);
		}
	}
	else
	{
		fsetpos(fp_texture, &image_body_file_position);
	}

	// Read the width, height and maximum intensity of the image:
	char width_string[] = "\0\0\0\0\0\0\0";
	unsigned int string_position = 0;
	do
	{
		fread(width_string + string_position, sizeof(char), 1, fp_texture);
	} while (*(width_string + string_position++) != ' ');
	int width = atoi(width_string);

	char height_string[] = "\0\0\0\0\0\0\0";
	string_position = 0;
	do {
		fread(height_string + string_position, sizeof(char), 1, fp_texture);
	} while (*(height_string + string_position++) != '\n');
	int height = atoi(height_string);
		
	char max_intensity_string[] = "\0\0\0\0\0\0\0";
	string_position = 0;
	do {
		fread(max_intensity_string + string_position, sizeof(char), 1, fp_texture);
	} while (*(max_intensity_string + string_position++) != '\n');
	double max_intensity = atof(max_intensity_string);

	texture_data = createVoidImage(3 * width, height, IMG_UINT);
	fread(texture_data->image_data.unsigned_character_image_data, sizeof(unsigned char), 3 * width * height, fp_texture);
	fclose(fp_texture);
}
