#ifndef OBJECTS_MANAGER_CLASS_H_INCLUDED
#define OBJECTS_MANAGER_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "geometries_manager_class.h"
#include <vector>

#include <string.h>

typedef struct node_geometry_location
{
	object_geometry_type node_geometry_type;
	node_geometry geometry;
	glm::vec4 position;

	node_geometry_location()
	{
		node_geometry_type = OBJGEOM_UNASSIGNED;
		position = glm::vec4(0.0, 0.0, 0.0, 0.0);
	}

	node_geometry_location(const node_geometry_location & src_node_object)
	{
		node_geometry_type = src_node_object.node_geometry_type;
		position = src_node_object.position;
		geometry = src_node_object.geometry;
	}

	node_geometry_location(const node_geometry_location * src_node_object)
	{
		node_geometry_type = src_node_object->node_geometry_type;
		position = src_node_object->position;
		geometry = src_node_object->geometry;
	}
} node_geometry_location;


class OBJECTS_MANAGER
{
public:	

	OBJECTS_MANAGER();
	OBJECTS_MANAGER(GEOMETRIES_MANAGER * src_geometries_manager);
	OBJECTS_MANAGER(const OBJECTS_MANAGER& src_node_figure);
	~OBJECTS_MANAGER();
	
	void assignGeometriesManager(GEOMETRIES_MANAGER * src_geometries_manager);

	void addNode(const object_geometry_type src_new_node_type, const GLfloat src_position_x = 0.0f, const GLfloat src_position_y = 0.0f, const GLfloat src_position_z = 0.0f, const GLfloat src_scale = 1.0f);

	void moveNode(const unsigned int src_node_index, const float delta_x, const float delta_y, const float delta_z = 0.0f);

	void moveAllNodes(const float delta_x, const float delta_y, const float delta_z = 0.0f);

	void scaleAllNodes(const float scale_axis_x, const float scale_axis_y, const float scale_axis_z = 1.0f);

	void rotateAllNodes(const float angle_axis_x, const float angle_axis_y, const float angle_axis_z = 0.0f);

	unsigned int getNodesCount();
	unsigned int getVerticesCount();

	void dumpNodesGeometries(GLfloat * dst_vertices_positions, GLfloat * dst_uv_coordinates, GLfloat * dst_normal_vectors);

private:
	
	unsigned int nodes_count;

	bool a_node_was_added;
	bool a_node_was_moved;
	bool all_nodes_were_moved;

	std::vector<unsigned int> node_was_moved;
	std::vector<unsigned int> node_was_added;
	std::vector<unsigned int> vertices_accumulated_count;

	glm::mat4 nodes_scale;
	glm::mat4 nodes_rotation;
	glm::vec4 nodes_position;

	GEOMETRIES_MANAGER * my_geometries_manager;

	std::vector<node_geometry_location*> objects_geometries_locations;

	void dumpNodesGeometriesMovedOnly(GLfloat * dst_vertices_positions, GLfloat * dst_uv_coordinates, GLfloat * dst_normal_vectors);
	void dumpNodesGeometriesNewOnly(GLfloat * dst_vertices_positions, GLfloat * dst_uv_coordinates, GLfloat * dst_normal_vectors);
};

#endif // OBJECTS_MANAGER_CLASS_H_INCLUDED
