#ifndef FIGURE_2D_CLASS_H_INCLUDED
#define FIGURE_2D_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string.h>

class FIGURE_2D
{
public:
	FIGURE_2D()
	{
		vertices_positions = NULL;
		color_values = NULL;
		uv_values = NULL;
		normal_vectors = NULL;

		figure_position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		figure_scale = glm::mat4(1.0f);

		triangles_count = 0;

		texture_data = NULL;
	}


	FIGURE_2D(const float initial_position_x, const float initial_position_y, const float initial_position_z = 0.0f, const float src_scale = 1.0)
	{
		vertices_positions = NULL;
		color_values = NULL;
		uv_values = NULL;
		normal_vectors = NULL;

		figure_position = glm::vec4(initial_position_x, initial_position_y, initial_position_z, 1.0f);
		figure_scale = glm::scale(glm::vec3(src_scale, src_scale, 1.0f));

		triangles_count = 0;
	}

	FIGURE_2D(const FIGURE_2D& src_node_figure)
	{
		this->vertices_positions = NULL;
		this->color_values = NULL;
		this->uv_values = NULL;
		this->normal_vectors = NULL;

		this->triangles_count = 0;
		
		copyFigure2d(src_node_figure);
	}
	
	virtual ~FIGURE_2D()
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

	void moveFigure(const float delta_x, const float delta_y, const float delta_z = 0.0f)
	{
		glm::mat4 translation_matrix = glm::translate(glm::mat4(), glm::vec3(delta_x, delta_y, delta_z));
		figure_position = translation_matrix * figure_position;
	}

	unsigned int getTrianglesCount()
	{
		return triangles_count;
	}

	GLfloat * getVerticesPositions()
	{
		return vertices_positions;
	}

	GLfloat * getColorValues()
	{
		return color_values;
	}

	GLfloat * getUVValues()
	{
		return uv_values;
	}

	GLfloat * getNormalVectors()
	{
		return normal_vectors;
	}

	unsigned int getColorChannels()
	{
		return color_channels;
	}

	unsigned int getTextureWidth()
	{
		return width;
	}

	unsigned int getTextureHeight()
	{
		return height;
	}

	unsigned char * getTextureData()
	{
		return texture_data;
	}

	void loadTexture(const char * src_filename)
	{
		FILE * fp_texture = fopen(src_filename, "r");

		if (!fp_texture)
		{
			fprintf(stderr, "<<Error: The texture file \'%s\' could not be opened>>\n", src_filename);
			return;
		}

		char magic_number[3];
		fgets(magic_number, 2, fp_texture);
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

protected:	
	GLfloat * vertices_positions;
	GLfloat * color_values;
	GLfloat * uv_values;
	GLfloat * normal_vectors;

	unsigned int triangles_count;

	void copyFigure2d(const FIGURE_2D& src_node_figure)
	{
		/* Copy the vertices data */
		if (this->vertices_positions && !src_node_figure.vertices_positions)
		{
			free(this->vertices_positions);
		}
		else if (!this->vertices_positions && src_node_figure.vertices_positions)
		{
			this->vertices_positions = (GLfloat*)malloc(3 * 3 * src_node_figure.triangles_count * sizeof(GLfloat));
		}
		else if ((this->vertices_positions && src_node_figure.vertices_positions) &&
			(this->triangles_count != src_node_figure.triangles_count))
		{
			free(this->vertices_positions);
			this->vertices_positions = (GLfloat*)malloc(3 * 3 * src_node_figure.triangles_count * sizeof(GLfloat));
		}

		if (src_node_figure.vertices_positions)
		{
			memcpy(this->vertices_positions, src_node_figure.vertices_positions, 3 * 3 * src_node_figure.triangles_count * sizeof(GLfloat));
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
		this->figure_position = src_node_figure.figure_position;
		this->figure_scale = src_node_figure.figure_scale;
	}

	virtual void initializeVerticesPositions() = 0;
	virtual void initializeColorValues() = 0;
	virtual void initializeUVValues() = 0;
	virtual void initializeNormalVectors() = 0;

private:
	glm::vec4 figure_position;
	glm::mat4 figure_scale;

	unsigned char * texture_data;
	unsigned int color_channels;
	unsigned int width;
	unsigned int height;


	void loadPGM_ascii(const char * src_filename)
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
		fscanf(fp_texture, "%i %i\n", &width, &height);

		double max_intensity;
		fscanf(fp_texture, "%lf\n", &max_intensity);
		
		texture_data = (unsigned char*)calloc(width * height, sizeof(unsigned char));

		unsigned char * pix_intensity = texture_data;
		for (unsigned int pix_position = 0; pix_position < width*height; pix_position++)
		{
			fscanf(fp_texture, "%c\n", pix_intensity++);
		}

		fclose(fp_texture);
	}



	void loadPGM_raw(const char * src_filename)
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
		width = atoi(width_string);

		char height_string[] = "\0\0\0\0\0\0\0";
		string_position = 0;
		do {
			fread(height_string + string_position, sizeof(char), 1, fp_texture);
		} while (*(height_string + string_position++) != '\n');
		height = atoi(height_string);

		
		char max_intensity_string[] = "\0\0\0\0\0\0\0";
		string_position = 0;
		do {
			fread(max_intensity_string + string_position, sizeof(char), 1, fp_texture);
		} while (*(max_intensity_string + string_position++) != '\n');
		double max_intensity = atof(max_intensity_string);
		
		texture_data = (unsigned char*)calloc(width * height, sizeof(unsigned char));
		fread(texture_data, sizeof(unsigned char), width * height, fp_texture);
		fclose(fp_texture);
	}


	void loadPPM_ascii(const char * src_filename)
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
		fscanf(fp_texture, "%i %i\n", &width, &height);

		double max_intensity;
		fscanf(fp_texture, "%lf\n", &max_intensity);

		texture_data = (unsigned char*)calloc(3 * width * height, sizeof(unsigned char));

		unsigned char * pix_intensity = texture_data;
		for (unsigned int pix_position = 0; pix_position < width*height; pix_position++)
		{
			fscanf(fp_texture, "%c\n", pix_intensity++);
		}

		fclose(fp_texture);
	}



	void loadPPM_raw(const char * src_filename)
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
		width = atoi(width_string);

		char height_string[] = "\0\0\0\0\0\0\0";
		string_position = 0;
		do {
			fread(height_string + string_position, sizeof(char), 1, fp_texture);
		} while (*(height_string + string_position++) != '\n');
		height = atoi(height_string);
		
		char max_intensity_string[] = "\0\0\0\0\0\0\0";
		string_position = 0;
		do {
			fread(max_intensity_string + string_position, sizeof(char), 1, fp_texture);
		} while (*(max_intensity_string + string_position++) != '\n');
		double max_intensity = atof(max_intensity_string);

		texture_data = (unsigned char*)calloc(3 * width * height, sizeof(unsigned char));
		fread(texture_data, sizeof(unsigned char), 3 * width * height, fp_texture);
		fclose(fp_texture);
	}
};

#endif // FIGURE_2D_CLASS_H_INCLUDED
