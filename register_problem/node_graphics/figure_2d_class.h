#ifndef FIGURE_2D_CLASS_H_INCLUDED
#define FIGURE_2D_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string.h>
#include "../image_functions.h"

class FIGURE_2D
{
public:
	FIGURE_2D();

	FIGURE_2D(const float initial_position_x, const float initial_position_y, const float initial_position_z = 0.0f, const float src_scale = 1.0);

	FIGURE_2D(const FIGURE_2D& src_node_figure);

	virtual ~FIGURE_2D();

	void moveFigure(const float delta_x, const float delta_y, const float delta_z = 0.0f);
	void translateFigure(const float delta_x, const float delta_y, const float delta_z = 0.0f);
	void scaleFigure(const float src_scale);

	unsigned int getTrianglesCount();

	glm::vec3 * getVerticesPositions();
	GLfloat * getColorValues();
	GLfloat * getUVValues();
	GLfloat * getNormalVectors();

	unsigned int getColorChannels();
	IMG_DATA * getTextureData();

	void loadTexture(const char * src_filename);
	void assignTexture(IMG_DATA * src_texture, const unsigned int src_color_channels = 1);

	glm::mat4 getPosition();
	glm::mat4 getScale();

protected:
	glm::vec3 * vertices_positions;
	GLfloat * color_values;
	GLfloat * uv_values;
	GLfloat * normal_vectors;

	unsigned int triangles_count;
	unsigned int color_channels;
	IMG_DATA * texture_data;

	void copyFigure2d(const FIGURE_2D& src_node_figure);

	virtual void initializeVerticesPositions() = 0;
	virtual void initializeColorValues() = 0;
	virtual void initializeUVValues() = 0;
	virtual void initializeNormalVectors() = 0;

private:
	bool figure_was_moved;

	glm::mat4 figure_traslation;
	glm::mat4 figure_scale;


	void loadPGM_ascii(const char * src_filename);

	void loadPGM_raw(const char * src_filename);

	void loadPPM_ascii(const char * src_filename);

	void loadPPM_raw(const char * src_filename);
};

#endif // FIGURE_2D_CLASS_H_INCLUDED
