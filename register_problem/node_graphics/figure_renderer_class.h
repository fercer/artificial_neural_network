#ifndef FIGURE_RENDERER_CLASS_H_INCLUDED
#define FIGURE_RENDERER_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string.h>

#include "figure_2d_class.h"


class FIGURE_RENDERER
{
public:
	FIGURE_RENDERER()
	{
		vertices_positions = NULL;
		color_values = NULL;
		uv_values = NULL;
		normal_vectors = NULL;
		triangles_count = 0;
		figures_count = 0;
		width = 0;
		height = 0;
		color_channels = 0;
		texture_data = NULL;
	}

	FIGURE_RENDERER(const FIGURE_RENDERER& src_figure_renderer)
	{
		this->vertices_positions = NULL;
		this->color_values = NULL;
		this->uv_values = NULL;
		this->normal_vectors = NULL;
		this->triangles_count = 0;
		this->figures_count = 0;
		this->width = 0;
		this->height = 0;
		this->color_channels = 0;
		this->texture_data = NULL;
		
		/* Copy the content of src_figure_renderer to this */
	}
	
	virtual ~FIGURE_RENDERER()
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
			free(texture_data);
		}
	}

	void addFigure(FIGURE_2D & src_figure)
	{
		color_channels = src_figure.getColorChannels();

		const unsigned int current_width = src_figure.getTextureWidth();
		const unsigned int current_height = src_figure.getTextureHeight();

		if (texture_data)
		{
			unsigned char * swap_texture_data = texture_data;
			texture_data = (unsigned char*)malloc(color_channels * width * (height + current_height) * sizeof(unsigned char));
			memcpy(texture_data, swap_texture_data, color_channels * width * height * sizeof(unsigned char));
			free(swap_texture_data);
		}
		else
		{
			texture_data = (unsigned char*)malloc(color_channels * current_width * current_height * sizeof(unsigned char));
		}

		memcpy(texture_data + width * height * color_channels,
			src_figure.getTextureData(),
			color_channels * current_height * current_width * sizeof(unsigned char));

		width = current_width;
		height += current_height;
		
		const unsigned int current_triangles_count = src_figure.getTrianglesCount();
		
		GLfloat * current_vertices_positions = src_figure.getVerticesPositions();
		GLfloat * current_color_values = src_figure.getColorValues();
		GLfloat * current_uv_values = src_figure.getUVValues();
		GLfloat * current_normal_vectors = src_figure.getNormalVectors();
		
		if (vertices_positions)
		{
			GLfloat * swap_vertices_positions = vertices_positions;
			vertices_positions = (GLfloat*)malloc(3 * 3 * (triangles_count + current_triangles_count) * sizeof(GLfloat));
			memcpy(vertices_positions, swap_vertices_positions, 3 * 3 * triangles_count * sizeof(GLfloat));
			free(swap_vertices_positions);
		}
		else
		{
			vertices_positions = (GLfloat*)malloc(3 * 3 * current_triangles_count * sizeof(GLfloat));
		}

		if (color_values)
		{
			GLfloat * swap_color_values = color_values;
			color_values = (GLfloat*)malloc(3 * 3 * (triangles_count + current_triangles_count) * sizeof(GLfloat));
			memcpy(color_values, swap_color_values, 3 * 3 * triangles_count * sizeof(GLfloat));
			free(swap_color_values);
		}
		else
		{
			color_values = (GLfloat*)malloc(3 * 3 * current_triangles_count * sizeof(GLfloat));
		}

		if (uv_values)
		{
			GLfloat * swap_uv_values = uv_values;
			uv_values = (GLfloat*)malloc(2 * 3 * (triangles_count + current_triangles_count) * sizeof(GLfloat));
			memcpy(uv_values, swap_uv_values, 2 * 3 * triangles_count * sizeof(GLfloat));
			free(swap_uv_values);
		}
		else
		{
			uv_values = (GLfloat*)malloc(2 * 3 * current_triangles_count * sizeof(GLfloat));
		}

		if (normal_vectors)
		{
			GLfloat * swap_normal_vectors = normal_vectors;
			normal_vectors = (GLfloat*)malloc(3 * 3 * (triangles_count + current_triangles_count) * sizeof(GLfloat));
			memcpy(normal_vectors, swap_normal_vectors, 3 * 3 * triangles_count * sizeof(GLfloat));
			free(swap_normal_vectors);
		}
		else
		{
			normal_vectors = (GLfloat*)malloc(3 * 3 * current_triangles_count * sizeof(GLfloat));
		}

		memcpy(vertices_positions + 3 * 3 * triangles_count, current_vertices_positions, 3 * 3 * current_triangles_count * sizeof(GLfloat));
		memcpy(color_values + 3 * 3 * triangles_count, current_color_values, 3 * 3 * current_triangles_count * sizeof(GLfloat));
		memcpy(normal_vectors + 3 * 3 * triangles_count, current_normal_vectors, 3 * 3 * current_triangles_count * sizeof(GLfloat));

		for (unsigned int current_triangle = 0; current_triangle < current_triangles_count; current_triangle++)
		{
			*(uv_values + current_triangle * 3 * 2 + triangles_count * 3 * 2) =
				*(current_uv_values + current_triangle * 3 * 2);
			*(uv_values + current_triangle * 3 * 2 + triangles_count * 3 * 2 + 1) =
				*(current_uv_values + current_triangle * 3 * 2 + 1) + 2.0 * (float)figures_count;

			*(uv_values + current_triangle * 3 * 2 + triangles_count * 3 * 2 + 2) =
				*(current_uv_values + current_triangle * 3 * 2 + 2);
			*(uv_values + current_triangle * 3 * 2 + triangles_count * 3 * 2 + 3) =
				*(current_uv_values + current_triangle * 3 * 2 + 3) + 2.0 * (float)figures_count;

			*(uv_values + current_triangle * 3 * 2 + triangles_count * 3 * 2 + 4) =
				*(current_uv_values + current_triangle * 3 * 2 + 4);
			*(uv_values + current_triangle * 3 * 2 + triangles_count * 3 * 2 + 5) =
				*(current_uv_values + current_triangle * 3 * 2 + 5) + 2.0 * (float)figures_count;
		}

		triangles_count += current_triangles_count;
		figures_count++;
	}

	unsigned int getTexturesWidth()
	{
		return width;
	}
	
	unsigned int getTexturesHeight()
	{
		return height;
	}

	unsigned int getTextureColorChannels()
	{
		return color_channels;
	}

	unsigned int getTriangesCount()
	{
		return triangles_count;
	}

	unsigned char * getTextureData()
	{
		return texture_data;
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

private:
	GLfloat * vertices_positions;
	GLfloat * color_values;
	GLfloat * uv_values;
	GLfloat * normal_vectors;

	unsigned int figures_count;
	unsigned int triangles_count;

	unsigned int width;
	unsigned int height;
	unsigned int color_channels;
	unsigned char * texture_data;
};

#endif // FIGURE_RENDERER_CLASS_H_INCLUDED
