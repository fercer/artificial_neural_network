#include "node_figure_class.h"

NODE_FIGURE::NODE_FIGURE()
{
	triangles_count = 8;
	vertices_positions = (glm::vec3*)malloc(3 * triangles_count * sizeof(glm::vec3));
	color_values = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));
	uv_values = (GLfloat*)malloc(2 * 3 * triangles_count * sizeof(GLfloat));
	normal_vectors = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));

	initializeVerticesPositions();
	initializeColorValues();
	initializeUVValues();
}



NODE_FIGURE::NODE_FIGURE(const NODE_FIGURE& src_node_figure)
{
	triangles_count = 8;
	vertices_positions = (glm::vec3*)malloc(3 * triangles_count * sizeof(glm::vec3));
	color_values = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));
	uv_values = (GLfloat*)malloc(2 * 3 * triangles_count * sizeof(GLfloat));
	normal_vectors = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));

	copyFigure2d(src_node_figure);
}



NODE_FIGURE NODE_FIGURE::operator=(const NODE_FIGURE& src_node_figure)
{
	if (this != &src_node_figure)
	{
		copyFigure2d(src_node_figure);
	}

	return *this;
}



NODE_FIGURE::~NODE_FIGURE()
{
}



void NODE_FIGURE::initializeVerticesPositions()
{
	glm::vec3 * vertices_positions_ptr = vertices_positions;

	/* Logo to identify the node type: */
	(vertices_positions_ptr)->x = -2.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -2.5f;
	(vertices_positions_ptr)->y = 2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -1.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -2.5f;
	(vertices_positions_ptr)->y = 2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -1.5f;
	(vertices_positions_ptr)->y = 2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -1.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	/* Node's name background: */
	(vertices_positions_ptr)->x = -1.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -1.5f;
	(vertices_positions_ptr)->y = 2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = 2.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -1.5f;
	(vertices_positions_ptr)->y = 2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = 2.5f;
	(vertices_positions_ptr)->y = 2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = 2.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;


	/* Node canvas: */
	(vertices_positions_ptr)->x = -2.5f;
	(vertices_positions_ptr)->y = -2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -2.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = 2.5f;
	(vertices_positions_ptr)->y = -2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -2.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = 2.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = 2.5f;
	(vertices_positions_ptr)->y = -2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	/* Node Value: */
	(vertices_positions_ptr)->x = -2.25f;
	(vertices_positions_ptr)->y = -1.5f;
	(vertices_positions_ptr++)->z = 0.1f;

	(vertices_positions_ptr)->x = -2.25f;
	(vertices_positions_ptr)->y = -0.5f;
	(vertices_positions_ptr++)->z = 0.1f;

	(vertices_positions_ptr)->x = 2.25f;
	(vertices_positions_ptr)->y = -1.5f;
	(vertices_positions_ptr++)->z = 0.1f;

	(vertices_positions_ptr)->x = -2.25f;
	(vertices_positions_ptr)->y = -0.5f;
	(vertices_positions_ptr++)->z = 0.1f;

	(vertices_positions_ptr)->x = 2.25f;
	(vertices_positions_ptr)->y = -0.5f;
	(vertices_positions_ptr++)->z = 0.1f;

	(vertices_positions_ptr)->x = 2.25f;
	(vertices_positions_ptr)->y = -1.5f;
	(vertices_positions_ptr++)->z = 0.1f;
}



void NODE_FIGURE::initializeColorValues()
{
	GLfloat * color_values_ptr = color_values;

	/* Logo to identify the node type: */
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	/* Node's name background: */
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	/* Node canvas */
	*(color_values_ptr++) = 147.0f / 255.0f;
	*(color_values_ptr++) = 157.0f / 255.0f;
	*(color_values_ptr++) = 172.0f / 255.0f;

	*(color_values_ptr++) = 147.0f / 255.0f;
	*(color_values_ptr++) = 157.0f / 255.0f;
	*(color_values_ptr++) = 172.0f / 255.0f;

	*(color_values_ptr++) = 147.0f / 255.0f;
	*(color_values_ptr++) = 157.0f / 255.0f;
	*(color_values_ptr++) = 172.0f / 255.0f;

	*(color_values_ptr++) = 147.0f / 255.0f;
	*(color_values_ptr++) = 157.0f / 255.0f;
	*(color_values_ptr++) = 172.0f / 255.0f;

	*(color_values_ptr++) = 147.0f / 255.0f;
	*(color_values_ptr++) = 157.0f / 255.0f;
	*(color_values_ptr++) = 172.0f / 255.0f;

	*(color_values_ptr++) = 147.0f / 255.0f;
	*(color_values_ptr++) = 157.0f / 255.0f;
	*(color_values_ptr++) = 172.0f / 255.0f;

	/* Value background */
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
}

void NODE_FIGURE::initializeUVValues()
{
	GLfloat * uv_values_ptr = uv_values;

	/* Logo to identify the node type: */
	*(uv_values_ptr++) = 0.0f;
	*(uv_values_ptr++) = 1.0f;

	*(uv_values_ptr++) = 0.0f;
	*(uv_values_ptr++) = 0.75f;

	*(uv_values_ptr++) = 0.5f;
	*(uv_values_ptr++) = 1.0f;

	*(uv_values_ptr++) = 0.0f;
	*(uv_values_ptr++) = 0.75f;

	*(uv_values_ptr++) = 0.5f;
	*(uv_values_ptr++) = 0.75f;

	*(uv_values_ptr++) = 0.5f;
	*(uv_values_ptr++) = 1.0f;

	/* Node's name background: */
	*(uv_values_ptr++) = 0.0f;
	*(uv_values_ptr++) = 0.5f;

	*(uv_values_ptr++) = 0.0f;
	*(uv_values_ptr++) = 0.75f;

	*(uv_values_ptr++) = 0.5f;
	*(uv_values_ptr++) = 0.5f;

	*(uv_values_ptr++) = 0.0f;
	*(uv_values_ptr++) = 0.75f;

	*(uv_values_ptr++) = 0.5f;
	*(uv_values_ptr++) = 0.75f;

	*(uv_values_ptr++) = 0.5f;
	*(uv_values_ptr++) = 0.5f;

	/* Node canvas */
	*(uv_values_ptr++) = 0.5f;
	*(uv_values_ptr++) = 0.5f;

	*(uv_values_ptr++) = 0.5f;
	*(uv_values_ptr++) = 0.75f;

	*(uv_values_ptr++) = 1.0f;
	*(uv_values_ptr++) = 0.5f;

	*(uv_values_ptr++) = 0.5f;
	*(uv_values_ptr++) = 0.5f;

	*(uv_values_ptr++) = 0.5f;
	*(uv_values_ptr++) = 0.75f;

	*(uv_values_ptr++) = 1.0f;
	*(uv_values_ptr++) = 0.5f;

	/* Value background */
	*(uv_values_ptr++) = 1.0f;
	*(uv_values_ptr++) = 0.75f;

	*(uv_values_ptr++) = 1.0f;
	*(uv_values_ptr++) = 0.5f;
	
	*(uv_values_ptr++) = 0.5f;
	*(uv_values_ptr++) = 0.75f;

	*(uv_values_ptr++) = 1.0f;
	*(uv_values_ptr++) = 0.75f;

	*(uv_values_ptr++) = 1.0f;
	*(uv_values_ptr++) = 0.5f;

	*(uv_values_ptr++) = 0.5f;
	*(uv_values_ptr++) = 0.75f;
}

void NODE_FIGURE::initializeNormalVectors()
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
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	/* Node's name background: */
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

	/* Node canvas */
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

	/* Value background */
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
}
