#include "node_image_fg_class.h"

NODE_IMAGE_FG::NODE_IMAGE_FG()
{
	triangles_count = 2;
	vertices_positions = (glm::vec3*)malloc(3 * triangles_count * sizeof(glm::vec3));
	color_values = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));
	uv_values = (GLfloat*)malloc(2 * 3 * triangles_count * sizeof(GLfloat));
	normal_vectors = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));

	initializeVerticesPositions();
	initializeColorValues();
	initializeUVValues();
}



NODE_IMAGE_FG::NODE_IMAGE_FG(const NODE_IMAGE_FG& src_node_figure)
{
	triangles_count = 2;
	vertices_positions = (glm::vec3*)malloc(3 * triangles_count * sizeof(glm::vec3));
	color_values = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));
	uv_values = (GLfloat*)malloc(2 * 3 * triangles_count * sizeof(GLfloat));
	normal_vectors = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));

	copyFigure2d(src_node_figure);
}



NODE_IMAGE_FG NODE_IMAGE_FG::operator=(const NODE_IMAGE_FG& src_node_figure)
{
	if (this != &src_node_figure)
	{
		copyFigure2d(src_node_figure);
	}

	return *this;
}



NODE_IMAGE_FG::~NODE_IMAGE_FG()
{
}



void NODE_IMAGE_FG::initializeVerticesPositions()
{
	glm::vec3 * vertices_positions_ptr = vertices_positions;

	(vertices_positions_ptr)->x = -2.25f;
	(vertices_positions_ptr)->y = -2.25f;
	(vertices_positions_ptr++)->z = 0.2f;

	(vertices_positions_ptr)->x = -2.25f;
	(vertices_positions_ptr)->y = 2.25f;
	(vertices_positions_ptr++)->z = 0.2f;

	(vertices_positions_ptr)->x = 2.25f;
	(vertices_positions_ptr)->y = -2.25f;
	(vertices_positions_ptr++)->z = 0.2f;

	(vertices_positions_ptr)->x = -2.25f;
	(vertices_positions_ptr)->y = 2.25f;
	(vertices_positions_ptr++)->z = 0.2f;

	(vertices_positions_ptr)->x = 2.25f;
	(vertices_positions_ptr)->y = 2.25f;
	(vertices_positions_ptr++)->z = 0.2f;

	(vertices_positions_ptr)->x = 2.25f;
	(vertices_positions_ptr)->y = -2.25f;
	(vertices_positions_ptr++)->z = 0.2f;
}


void NODE_IMAGE_FG::initializeColorValues()
{
	GLfloat * color_values_ptr = color_values;

	/* Logo to identify the node type: */
	*(color_values_ptr++) = 255.0f;
	*(color_values_ptr++) = 255.0f;
	*(color_values_ptr++) = 255.0f;

	*(color_values_ptr++) = 255.0f;
	*(color_values_ptr++) = 255.0f;
	*(color_values_ptr++) = 255.0f;

	*(color_values_ptr++) = 255.0f;
	*(color_values_ptr++) = 255.0f;
	*(color_values_ptr++) = 255.0f;

	*(color_values_ptr++) = 255.0f;
	*(color_values_ptr++) = 255.0f;
	*(color_values_ptr++) = 255.0f;

	*(color_values_ptr++) = 255.0f;
	*(color_values_ptr++) = 255.0f;
	*(color_values_ptr++) = 255.0f;

	*(color_values_ptr++) = 255.0f;
	*(color_values_ptr++) = 255.0f;
	*(color_values_ptr++) = 255.0f;
}


void NODE_IMAGE_FG::initializeUVValues()
{
	GLfloat * uv_values_ptr = uv_values;

	*(uv_values_ptr++) = 0.0f;
	*(uv_values_ptr++) = 0.5f;

	*(uv_values_ptr++) = 0.0f;
	*(uv_values_ptr++) = 0.0f;

	*(uv_values_ptr++) = 1.0f;
	*(uv_values_ptr++) = 0.5f;

	*(uv_values_ptr++) = 0.0f;
	*(uv_values_ptr++) = 0.0f;

	*(uv_values_ptr++) = 1.0f;
	*(uv_values_ptr++) = 0.0f;

	*(uv_values_ptr++) = 1.0f;
	*(uv_values_ptr++) = 0.5f;
}


void NODE_IMAGE_FG::initializeNormalVectors()
{
	GLfloat * normal_vectors_ptr = normal_vectors;

	/* Logo to identify the node type: */
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
}
