#ifndef NODE_FIGURE_CLASS_H_INCLUDED
#define NODE_FIGURE_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "figure_2d_class.h"

class NODE_FIGURE :
	public FIGURE_2D
{
public:
	NODE_FIGURE()
	{
		triangles_count = 8;
		vertices_positions = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));
		color_values = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));
		uv_values = (GLfloat*)malloc(2 * 3 * triangles_count * sizeof(GLfloat));
		normal_vectors = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));

		initializeVerticesPositions();
		initializeColorValues();
		initializeUVValues();
	}

	NODE_FIGURE(const NODE_FIGURE& src_node_figure)
	{
		triangles_count = 8;
		vertices_positions = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));
		color_values = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));
		uv_values = (GLfloat*)malloc(2 * 3 * triangles_count * sizeof(GLfloat));
		normal_vectors = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));

		copyFigure2d(src_node_figure);
	}

	NODE_FIGURE operator=(const NODE_FIGURE& src_node_figure)
	{
		copyFigure2d(src_node_figure);
	}

	~NODE_FIGURE()
	{
	}

private:

	void initializeVerticesPositions()
	{
		unsigned int current_position = 0;

		/* Logo to identify the node type: */
		vertices_positions[current_position++] = -2.5f;
		vertices_positions[current_position++] = 1.5f;
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = -2.5f;
		vertices_positions[current_position++] = 2.5f;
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = -1.5f;
		vertices_positions[current_position++] = 2.5f;
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = -2.5f; 
		vertices_positions[current_position++] = 1.5f; 
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = -1.5f; 
		vertices_positions[current_position++] = 1.5f; 
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = -1.5f; 
		vertices_positions[current_position++] = 2.5f;
		vertices_positions[current_position++] = 0.0f;

		/* Node's name background: */
		vertices_positions[current_position++] = -1.5f;
		vertices_positions[current_position++] = 1.5f; 
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = -1.5f;
		vertices_positions[current_position++] = 2.5f; 
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = 2.5f; 
		vertices_positions[current_position++] = 1.5f; 
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = -1.5f;
		vertices_positions[current_position++] = 2.5f;
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = 2.5f; 
		vertices_positions[current_position++] = 2.5f; 
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = 2.5f; 
		vertices_positions[current_position++] = 1.5f;
		vertices_positions[current_position++] = 0.0f;

		/* Node canvas */
		vertices_positions[current_position++] = -2.5f; 
		vertices_positions[current_position++] = -2.5f; 
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = -2.5f;
		vertices_positions[current_position++] = 1.5f;
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = 2.5f;
		vertices_positions[current_position++] = -2.5f;
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = -2.5f;
		vertices_positions[current_position++] = 1.5f;
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = 2.5f;
		vertices_positions[current_position++] = 1.5f;
		vertices_positions[current_position++] = 0.0f;
		vertices_positions[current_position++] = 2.5f; 
		vertices_positions[current_position++] = -2.5f; 
		vertices_positions[current_position++] = 0.0f;

		/* Value background */
		vertices_positions[current_position++] = -2.0f;
		vertices_positions[current_position++] = -1.5f;
		vertices_positions[current_position++] = 0.1f;
		vertices_positions[current_position++] = -2.0f;
		vertices_positions[current_position++] = -1.0f;
		vertices_positions[current_position++] = 0.1f;
		vertices_positions[current_position++] = 2.0f;
		vertices_positions[current_position++] = -1.5f;
		vertices_positions[current_position++] = 0.1f;
		vertices_positions[current_position++] = -2.0f;
		vertices_positions[current_position++] = -1.0f;
		vertices_positions[current_position++] = 0.1f;
		vertices_positions[current_position++] = 2.0f;
		vertices_positions[current_position++] = -1.0f;
		vertices_positions[current_position++] = 0.1f;
		vertices_positions[current_position++] = 2.0f;
		vertices_positions[current_position++] = -1.5f; 
		vertices_positions[current_position++] = 0.1f;
	}

	void initializeColorValues()
	{
		unsigned int current_position = 0;

		/* Logo to identify the node type: */
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 145.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 145.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 145.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 145.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 145.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 145.0f / 255.0f;

		/* Node's name background: */
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 145.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 145.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 145.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 145.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 145.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 111.0f / 255.0f;
		color_values[current_position++] = 145.0f / 255.0f;

		/* Node canvas */
		color_values[current_position++] = 147.0f / 255.0f;
		color_values[current_position++] = 157.0f / 255.0f;
		color_values[current_position++] = 172.0f / 255.0f;
		color_values[current_position++] = 147.0f / 255.0f;
		color_values[current_position++] = 157.0f / 255.0f;
		color_values[current_position++] = 172.0f / 255.0f;
		color_values[current_position++] = 147.0f / 255.0f;
		color_values[current_position++] = 157.0f / 255.0f;
		color_values[current_position++] = 172.0f / 255.0f;
		color_values[current_position++] = 147.0f / 255.0f;
		color_values[current_position++] = 157.0f / 255.0f;
		color_values[current_position++] = 172.0f / 255.0f;
		color_values[current_position++] = 147.0f / 255.0f;
		color_values[current_position++] = 157.0f / 255.0f;
		color_values[current_position++] = 172.0f / 255.0f;
		color_values[current_position++] = 147.0f / 255.0f;
		color_values[current_position++] = 157.0f / 255.0f;
		color_values[current_position++] = 172.0f / 255.0f;

		/* Value background */
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
		color_values[current_position++] = 1.0f;
	}

	void initializeUVValues()
	{
		unsigned int current_position = 0;

		/* Logo to identify the node type: */
		uv_values[current_position++] = 0.0f;
		uv_values[current_position++] = 0.25f;
		uv_values[current_position++] = 0.0f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.25f;
		uv_values[current_position++] = 0.0f;
		uv_values[current_position++] = 0.25f;
		uv_values[current_position++] = 0.0f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.25f;

		/* Node's name background: */
		uv_values[current_position++] = 0.0f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.25f;
		uv_values[current_position++] = 0.0f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.25f;

		/* Node canvas */
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.25f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 1.0f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.25f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 1.0f;
		uv_values[current_position++] = 0.5f;

		/* Value background */
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.25f;
		uv_values[current_position++] = 1.0f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 1.0f;
		uv_values[current_position++] = 0.25f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 0.25f;
		uv_values[current_position++] = 1.0f;
		uv_values[current_position++] = 0.5f;
		uv_values[current_position++] = 1.0f;
		uv_values[current_position++] = 0.25f;
	}

	void initializeNormalVectors()
	{
		unsigned int current_position = 0;

		/* Logo to identify the node type: */
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;

		/* Node's name background: */
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;

		/* Node canvas */
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;

		/* Value background */
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 0.0f;
		normal_vectors[current_position++] = 1.0f;
	}
};

#endif // NODE_FIGURE_CLASS_H_INCLUDED
