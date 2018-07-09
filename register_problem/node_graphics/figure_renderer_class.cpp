#include "figure_renderer_class.h"

FIGURE_RENDERER::FIGURE_RENDERER()
{
	vertices_global_positions = NULL;
	color_values = NULL;
	uv_values = NULL;
	normal_vectors = NULL;
	triangles_count = 0;
	last_triangles_count = 0;
	figures_count = 0;
	width = 0;
	height = 0;
	color_channels = 0;
	texture_data = NULL;
	
	triangles_per_figure.push_back(0);

	test_triangles_count = 2;
	test_vertices_global_positions = (GLfloat*)malloc(3 * 3 * test_triangles_count * sizeof(GLfloat));
	test_color_values = (GLfloat*)malloc(3 * 3 * test_triangles_count * sizeof(GLfloat));

	unsigned int test_current_vertex = 0;
	*(test_vertices_global_positions + test_current_vertex++) = -1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = -1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 0.0f;

	*(test_vertices_global_positions + test_current_vertex++) = -1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 0.0f;

	*(test_vertices_global_positions + test_current_vertex++) = 1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = -1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 0.0f;

	*(test_vertices_global_positions + test_current_vertex++) = -1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 0.0f;

	*(test_vertices_global_positions + test_current_vertex++) = 1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 0.0f;
	
	*(test_vertices_global_positions + test_current_vertex++) = 1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = -1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 0.0f;

	test_current_vertex = 0;
	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 0.0f;

	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 0.0f;

	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 0.0f;

	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 0.0f;
	*(test_color_values + test_current_vertex++) = 0.0f;

	*(test_color_values + test_current_vertex++) = 0.0f;
	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 0.0f;

	*(test_color_values + test_current_vertex++) = 0.0f;
	*(test_color_values + test_current_vertex++) = 0.0f;
	*(test_color_values + test_current_vertex++) = 1.0f;
}


FIGURE_RENDERER::FIGURE_RENDERER(const FIGURE_RENDERER& src_figure_renderer)
{
	this->vertices_global_positions = NULL;
	this->color_values = NULL;
	this->uv_values = NULL;
	this->normal_vectors = NULL;
	this->triangles_count = 0;
	this->last_triangles_count = 0;
	this->figures_count = 0;
	this->width = 0;
	this->height = 0;
	this->color_channels = 0;
	this->texture_data = NULL;
	
	triangles_per_figure.push_back(0);

	test_triangles_count = 2;
	test_vertices_global_positions = (GLfloat*)malloc(3 * 3 * test_triangles_count * sizeof(GLfloat));
	test_color_values = (GLfloat*)malloc(3 * 3 * test_triangles_count * sizeof(GLfloat));

	unsigned int test_current_vertex = 0;
	*(test_vertices_global_positions + test_current_vertex++) = -1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = -1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 0.0f;

	*(test_vertices_global_positions + test_current_vertex++) = -1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 0.0f;

	*(test_vertices_global_positions + test_current_vertex++) = 1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = -1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 0.0f;

	*(test_vertices_global_positions + test_current_vertex++) = -1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 0.0f;

	*(test_vertices_global_positions + test_current_vertex++) = 1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 0.0f;

	*(test_vertices_global_positions + test_current_vertex++) = 1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = -1.0f;
	*(test_vertices_global_positions + test_current_vertex++) = 0.0f;

	test_current_vertex = 0;
	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 0.0f;

	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 0.0f;

	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 0.0f;

	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 0.0f;
	*(test_color_values + test_current_vertex++) = 0.0f;

	*(test_color_values + test_current_vertex++) = 0.0f;
	*(test_color_values + test_current_vertex++) = 1.0f;
	*(test_color_values + test_current_vertex++) = 0.0f;

	*(test_color_values + test_current_vertex++) = 0.0f;
	*(test_color_values + test_current_vertex++) = 0.0f;
	*(test_color_values + test_current_vertex++) = 1.0f;
	/* Copy the content of src_figure_renderer to this */
}
	
FIGURE_RENDERER::~FIGURE_RENDERER()
{
	if (vertices_global_positions)
	{
		free(vertices_global_positions);
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



void FIGURE_RENDERER::addFigure(FIGURE_2D * src_figure)
{
	figures_array.push_back(src_figure);

	color_channels = src_figure->getColorChannels();

	const unsigned int current_width = src_figure->getTextureWidth();
	const unsigned int current_height = src_figure->getTextureHeight();

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
		src_figure->getTextureData(),
		color_channels * current_height * current_width * sizeof(unsigned char));

	width = current_width;
	height += current_height;
	
	const unsigned int current_triangles_count = src_figure->getTrianglesCount();
	
	GLfloat * current_color_values = src_figure->getColorValues();
	
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
	
	memcpy(color_values + 3 * 3 * triangles_count, current_color_values, 3 * 3 * current_triangles_count * sizeof(GLfloat));
	
	GLfloat * current_normal_vectors = src_figure->getNormalVectors();

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
		
	memcpy(normal_vectors + 3 * 3 * triangles_count, current_normal_vectors, 3 * 3 * current_triangles_count * sizeof(GLfloat));


	GLfloat * current_uv_values = src_figure->getUVValues();

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

	glm::vec3 * current_vertices_positions = src_figure->getVerticesPositions();

	if (vertices_global_positions)
	{
		GLfloat * swap_vertices_global_positions = vertices_global_positions;
		vertices_global_positions = (GLfloat*)malloc((triangles_count + current_triangles_count) * 3 * 3 * sizeof(GLfloat));
		memmove(vertices_global_positions, swap_vertices_global_positions, 3 * 3 * triangles_count * sizeof(GLfloat));
		free(swap_vertices_global_positions);
	}
	else
	{
		vertices_global_positions = (GLfloat*)malloc(current_triangles_count * 3 * 3 * sizeof(GLfloat));
	}

	glm::mat4 figure_position = src_figure->getPosition();
	glm::mat4 figure_scale = src_figure->getScale();

	for (unsigned int current_triangle = 0; current_triangle < current_triangles_count; current_triangle++)
	{
		*(uv_values + current_triangle * 3 * 2 + triangles_count * 3 * 2) =
			*(current_uv_values + current_triangle * 3 * 2);
		*(uv_values + current_triangle * 3 * 2 + triangles_count * 3 * 2 + 1) =
			*(current_uv_values + current_triangle * 3 * 2 + 1) + 2.0 * (GLfloat)figures_count;

		*(uv_values + current_triangle * 3 * 2 + triangles_count * 3 * 2 + 2) =
			*(current_uv_values + current_triangle * 3 * 2 + 2);
		*(uv_values + current_triangle * 3 * 2 + triangles_count * 3 * 2 + 3) =
			*(current_uv_values + current_triangle * 3 * 2 + 3) + 2.0 * (GLfloat)figures_count;

		*(uv_values + current_triangle * 3 * 2 + triangles_count * 3 * 2 + 4) =
			*(current_uv_values + current_triangle * 3 * 2 + 4);
		*(uv_values + current_triangle * 3 * 2 + triangles_count * 3 * 2 + 5) =
			*(current_uv_values + current_triangle * 3 * 2 + 5) + 2.0 * (GLfloat)figures_count;

		glm::vec4 vertex_1 = figure_position * figure_scale * glm::vec4(*(current_vertices_positions + 3 * current_triangle), 1.0);

		*(vertices_global_positions + current_triangle * 3 * 3 + triangles_count * 3 * 3) = vertex_1.x;
		*(vertices_global_positions + current_triangle * 3 * 3 + triangles_count * 3 * 3 + 1) = vertex_1.y;
		*(vertices_global_positions + current_triangle * 3 * 3 + triangles_count * 3 * 3 + 2) = vertex_1.z;

		glm::vec4 vertex_2 = figure_position *  figure_scale * glm::vec4(*(current_vertices_positions + 3 * current_triangle + 1), 1.0);

		*(vertices_global_positions + current_triangle * 3 * 3 + triangles_count * 3 * 3 + 3) = vertex_2.x;
		*(vertices_global_positions + current_triangle * 3 * 3 + triangles_count * 3 * 3 + 4) = vertex_2.y;
		*(vertices_global_positions + current_triangle * 3 * 3 + triangles_count * 3 * 3 + 5) = vertex_2.z;

		glm::vec4 vertex_3 = figure_position * figure_scale * glm::vec4(*(current_vertices_positions + 3 * current_triangle + 2), 1.0);

		*(vertices_global_positions + current_triangle * 3 * 3 + triangles_count * 3 * 3 + 6) = vertex_3.x;
		*(vertices_global_positions + current_triangle * 3 * 3 + triangles_count * 3 * 3 + 7) = vertex_3.y;
		*(vertices_global_positions + current_triangle * 3 * 3 + triangles_count * 3 * 3 + 8) = vertex_3.z;
	}

	triangles_count += current_triangles_count;
	triangles_per_figure.push_back(triangles_count);

	figures_count++;
}

unsigned int FIGURE_RENDERER::getTexturesWidth()
{
	return width;
}
	
unsigned int FIGURE_RENDERER::getTexturesHeight()
{
	return height;
}

unsigned int FIGURE_RENDERER::getTextureColorChannels()
{
	return color_channels;
}

unsigned int FIGURE_RENDERER::getTriangesCount()
{
	return triangles_count;
}

unsigned int FIGURE_RENDERER::getTestTriangesCount()
{
	return test_triangles_count;
}

unsigned char * FIGURE_RENDERER::getTextureData()
{
	return texture_data;
}

GLfloat * FIGURE_RENDERER::getVerticesPositions()
{
	std::vector<FIGURE_2D*>::iterator current_figure = figures_array.begin();
	std::vector<unsigned int>::iterator previous_accumulated_triangles = triangles_per_figure.begin();

	GLfloat * vertices_global_positions_ptr = vertices_global_positions;
	for (unsigned int figure_index = 0; figure_index < figures_count; figure_index++, current_figure++, previous_accumulated_triangles++)
	{
		glm::mat4 figure_position = (*current_figure)->getPosition();
		glm::mat4 figure_scale = (*current_figure)->getScale();
		glm::vec3 * current_figure_vertices = (*current_figure)->getVerticesPositions();

		for (unsigned int current_triangle = 0; current_triangle < (*current_figure)->getTrianglesCount(); current_triangle++)
		{
			glm::vec4 vertex_1 = glm::vec4(*(current_figure_vertices + current_triangle * 3), 1.0) * figure_scale * figure_position;

			*(vertices_global_positions_ptr++) = vertex_1.x;
			*(vertices_global_positions_ptr++) = vertex_1.y;
			*(vertices_global_positions_ptr++) = vertex_1.z;

			glm::vec4 vertex_2 = glm::vec4(*(current_figure_vertices + current_triangle * 3 + 1), 1.0) * figure_position * figure_scale;

			*(vertices_global_positions_ptr++) = vertex_2.x;
			*(vertices_global_positions_ptr++) = vertex_2.y;
			*(vertices_global_positions_ptr++) = vertex_2.z;

			glm::vec4 vertex_3 = glm::vec4(*(current_figure_vertices + current_triangle * 3 + 2), 1.0) * figure_position * figure_scale;

			*(vertices_global_positions_ptr++) = vertex_3.x;
			*(vertices_global_positions_ptr++) = vertex_3.y;
			*(vertices_global_positions_ptr++) = vertex_3.z;
		}
	}

	return vertices_global_positions;
}

GLfloat * FIGURE_RENDERER::getTestVerticesPositions()
{
	return test_vertices_global_positions;
}

GLfloat * FIGURE_RENDERER::getColorValues()
{
	return color_values;
}

GLfloat * FIGURE_RENDERER::getTestColorValues()
{
	return test_color_values;
}

GLfloat * FIGURE_RENDERER::getUVValues()
{
	return uv_values;
}

GLfloat * FIGURE_RENDERER::getNormalVectors()
{
	return normal_vectors;
}

