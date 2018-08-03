#ifndef INTERFACE_MANAGER_CLASS_H_INCLUDED
#define INTERFACE_MANAGER_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "geometries_manager_class.h"
#include "objects_manager_class.h"

#include <vector>
#include <algorithm>

#include <string.h>


class INTERFACE_MANAGER
{
public:	
	INTERFACE_MANAGER();
	INTERFACE_MANAGER(const INTERFACE_MANAGER& src_node_figure);
	~INTERFACE_MANAGER();
	
	/* Interface for the geometries manager: */
	void loadObj(const char * src_obj_filename);

	void assignTextureMap(char * src_material_identifier, IMG_DATA * src_texture_map);

	/* Interface for the objects manager: */
	void addNode(const object_geometry_type src_new_node_type, const GLfloat src_position_x = 0.0f, const GLfloat src_position_y = 0.0f, const GLfloat src_position_z = 0.0f, const GLfloat src_scale = 1.0f);

	void moveNode(const unsigned int src_node_index, const float delta_x, const float delta_y, const float delta_z = 0.0f);

	void moveAllNodes(const float delta_x, const float delta_y, const float delta_z = 0.0f);

	void scaleAllNodes(const float scale_axis_x, const float scale_axis_y, const float scale_axis_z = 1.0f);

	void rotateAllNodes(const float angle_axis_x, const float angle_axis_y, const float angle_axis_z = 0.0f);

private:
	GEOMETRIES_MANAGER my_geometries_manager;
	OBJECTS_MANAGER my_objects_manager;

};

#endif // INTERFACE_MANAGER_CLASS_H_INCLUDED
