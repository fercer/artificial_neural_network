#ifndef PROCESS_VISUALIZER_CLASS_H_INCLUDED
#define PROCESS_VISUALIZER_CLASS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string.h>

#include "generic_list_class.cpp"

class PROCESS_VISUALIZER
{
public:
	PROCESS_VISUALIZER();
	PROCESS_VISUALIZER(const PROCESS_VISUALIZER& src_node_leaf);
	PROCESS_VISUALIZER operator=(const PROCESS_VISUALIZER& src_node_leaf);
	~PROCESS_VISUALIZER();

	void showProcess();

private:
	bool opengl_initialized;

	void initializeGraphicEnvironment();
};

#endif // PROCESS_VISUALIZER_CLASS_H_INCLUDED
