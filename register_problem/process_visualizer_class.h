#ifndef PROCESS_VISUALIZER_CLASS_H_INCLUDED
#define PROCESS_VISUALIZER_CLASS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string.h>
#include <math.h>

#include "generic_list_class.cpp"
#include "node_graphics/figure_2d_class.h"
#include "node_graphics/node_figure_class.h"
#include "node_graphics/figure_renderer_class.h"
#include "node_graphics/typography_class.h"
#include "node_graphics/node_image_bg_class.h"
#include "node_graphics/node_image_fg_class.h"
#include "node_graphics/link_node_class.h"
#include "image_functions.h"

#include "node_scalar_class.h"
#include "nodes_scalar_operation_class.h"
#include "image_operation_class.h"
#include "image_scalar_operation_class.h"

class PROCESS_VISUALIZER
{
public:
	PROCESS_VISUALIZER();
	PROCESS_VISUALIZER(const PROCESS_VISUALIZER& src_node_leaf);
	PROCESS_VISUALIZER operator=(const PROCESS_VISUALIZER& src_node_leaf);
	~PROCESS_VISUALIZER();

	void addNodeOperation(NODE_SCALAR<double> * src_numeric_node);

	void showProcess();

private:
	bool opengl_initialized;

	FIGURE_RENDERER global_renderer;

	GLuint loadShaderProgram(const char * src_vertex_shader_filename, const char * src_fragment_shader_filename);
	GLuint loadTextures(IMG_DATA * src_textures, const unsigned int src_color_channels = 1);
	void initializeGraphicEnvironment();
};

#endif // PROCESS_VISUALIZER_CLASS_H_INCLUDED
