#ifndef FIGURE_RENDERER_CLASS_H_INCLUDED
#define FIGURE_RENDERER_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string.h>
#include <vector>

#include "../image_functions.h"
#include "figure_2d_class.h"

class FIGURE_RENDERER
{
public:
	FIGURE_RENDERER();

	FIGURE_RENDERER(const FIGURE_RENDERER& src_figure_renderer);

	virtual ~FIGURE_RENDERER();

	void addFigure(FIGURE_2D * src_figure);
	
	unsigned int getTextureColorChannels();
	unsigned int getTriangesCount();
	IMG_DATA * getTextureData();

	GLfloat * getVerticesPositions();
	GLfloat * getColorValues();
	GLfloat * getUVValues();
	GLfloat * getNormalVectors();
	
	unsigned int getTestTriangesCount();
	GLfloat * getTestVerticesPositions();
	GLfloat * getTestColorValues();

private:
	GLfloat * test_vertices_global_positions;
	GLfloat * test_color_values;
	unsigned int test_triangles_count;

	std::vector<unsigned int> triangles_per_figure;
	std::vector<FIGURE_2D*> figures_array;

	GLfloat * vertices_global_positions;
	GLfloat * color_values;
	GLfloat * uv_values;
	GLfloat * normal_vectors;

	unsigned int figures_count;
	unsigned int last_triangles_count;
	unsigned int triangles_count;

	unsigned int color_channels;

	IMG_DATA * texture_data;
};

#endif // FIGURE_RENDERER_CLASS_H_INCLUDED
