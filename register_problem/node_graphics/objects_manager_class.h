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
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>

#include <string.h>

typedef struct node_geometry_location
{
	node_geometry node_object_geometry;
	glm::vec4 node_position;
	glm::mat4 node_scale;
	glm::mat4 node_rotation;

	node_geometry_location()
	{
		node_position = glm::vec4(0.0, 0.0, 0.0, 0.0);
		node_scale = glm::mat4(1.0f);
		node_rotation = glm::mat4(1.0f);
	}

	node_geometry_location(const float src_pos_x = 0.0f, const float src_pos_y = 0.0f, const float src_pos_z = 0.0f, const float src_scale = 1.0f, const float src_rotation_angle = 0.0f)
	{
		node_position = glm::vec4(src_pos_x, src_pos_y, src_pos_z, 0.0);

		node_scale[0].x = src_scale;
		node_scale[1].y = src_scale;
		node_scale[2].z = src_scale;
		node_scale[3].w = 1.0;

		const float cos_rotation = cosf(src_rotation_angle);
		const float sin_rotation = sinf(src_rotation_angle);

		node_rotation[0].x = cos_rotation;
		node_rotation[0].y = -sin_rotation;
		node_rotation[1].x = sin_rotation;
		node_rotation[1].y = cos_rotation;
		node_rotation[2].z = 1.0f;
		node_rotation[3].w = 1.0f;
	}

	node_geometry_location(const node_geometry_location & src_node_object)
	{
		this->node_object_geometry = src_node_object.node_object_geometry;
		this->node_position = src_node_object.node_position;
		this->node_scale = src_node_object.node_scale;
		this->node_rotation = src_node_object.node_rotation;
	}

	node_geometry_location(const node_geometry_location * src_node_object)
	{
		this->node_object_geometry = src_node_object->node_object_geometry;
		this->node_position = src_node_object->node_position;
		this->node_scale = src_node_object->node_scale;
		this->node_rotation = src_node_object->node_rotation;
	}

	node_geometry_location& operator=(const node_geometry_location & src_node_object)
	{
		if (this != &src_node_object)
		{
			this->node_object_geometry = src_node_object.node_object_geometry;
			this->node_position = src_node_object.node_position;
			this->node_scale = src_node_object.node_scale;
			this->node_rotation = src_node_object.node_rotation;
		}
		return *this;
	}
} node_geometry_location;



typedef struct objects_by_material
{
	char * material_identifier;
	GLfloat * vertices_positions;
	GLfloat * uv_coordinates;
	GLfloat * normal_vectors;
	std::vector<unsigned int> vertices_acummulated_initial_positions;
	std::vector<unsigned int> corresponding_node_indices;

	objects_by_material()
	{
		material_identifier = NULL;
		vertices_positions = NULL;
		uv_coordinates = NULL;
		normal_vectors = NULL;
		vertices_acummulated_initial_positions.push_back(0);
	}

	objects_by_material(const char * src_material_identifier)
	{
		unsigned int material_identifier_length = (unsigned int)strlen(src_material_identifier) + 1;
		material_identifier = new char[material_identifier_length];
		strcpy(material_identifier, src_material_identifier);

		vertices_positions = NULL;
		uv_coordinates = NULL;
		normal_vectors = NULL;

		vertices_acummulated_initial_positions.push_back(0);
	}

	objects_by_material(const objects_by_material & src_objects_by_material)
	{
		unsigned int material_identifier_length = (unsigned int)strlen(src_objects_by_material.material_identifier) + 1;
		this->material_identifier = new char[material_identifier_length];
		strcpy(this->material_identifier, src_objects_by_material.material_identifier);

		const unsigned int vertices_count = (unsigned int)src_objects_by_material.vertices_acummulated_initial_positions.size();

		if (vertices_count > 0)
		{
			this->vertices_positions = new GLfloat[vertices_count * 3];
			memcpy(this->vertices_positions, src_objects_by_material.vertices_positions, vertices_count * 3 * sizeof(GLfloat));

			this->uv_coordinates = new GLfloat[vertices_count * 2];
			memcpy(this->uv_coordinates, src_objects_by_material.uv_coordinates, vertices_count * 2 * sizeof(GLfloat));

			this->normal_vectors = new GLfloat[vertices_count * 3];
			memcpy(this->normal_vectors, src_objects_by_material.normal_vectors, vertices_count * 3 * sizeof(GLfloat));
		}

		this->vertices_acummulated_initial_positions = src_objects_by_material.vertices_acummulated_initial_positions;
	}

	objects_by_material(objects_by_material * src_objects_by_material)
	{

		unsigned int material_identifier_length = (unsigned int)strlen(src_objects_by_material->material_identifier) + 1;
		this->material_identifier = new char[material_identifier_length];
		strcpy(this->material_identifier, src_objects_by_material->material_identifier);

		const unsigned int vertices_count = (unsigned int)src_objects_by_material->vertices_acummulated_initial_positions.size();

		if (vertices_count > 0)
		{
			this->vertices_positions = new GLfloat[vertices_count * 3];
			memcpy(this->vertices_positions, src_objects_by_material->vertices_positions, vertices_count * 3 * sizeof(GLfloat));

			this->uv_coordinates = new GLfloat[vertices_count * 2];
			memcpy(this->uv_coordinates, src_objects_by_material->uv_coordinates, vertices_count * 2 * sizeof(GLfloat));

			this->normal_vectors = new GLfloat[vertices_count * 3];
			memcpy(this->normal_vectors, src_objects_by_material->normal_vectors, > vertices_count * 3 * sizeof(GLfloat));
		}

		this->vertices_acummulated_initial_positions = src_objects_by_material->vertices_acummulated_initial_positions;
	}

	objects_by_material& operator=(const objects_by_material & src_objects_by_material)
	{
		if (this != &src_objects_by_material)
		{
			delete[] this->material_identifier;

			unsigned int material_identifier_length = (unsigned int)strlen(src_objects_by_material.material_identifier) + 1;
			this->material_identifier = new char[material_identifier_length];
			strcpy(this->material_identifier, src_objects_by_material.material_identifier);
			
			const unsigned int vertices_count = (unsigned int)src_objects_by_material.vertices_acummulated_initial_positions.size();
			if ((vertices_count) > 0 && (this->vertices_acummulated_initial_positions.at(vertices_acummulated_initial_positions.size()-1) != vertices_count))
			{
				delete[] this->vertices_positions;
				delete[] this->uv_coordinates;
				delete[] this->normal_vectors;

				this->vertices_positions = new GLfloat[vertices_count * 3];
				this->uv_coordinates = new GLfloat[vertices_count * 2];
				this->normal_vectors = new GLfloat[vertices_count * 3];
			}

			memcpy(this->vertices_positions, src_objects_by_material.vertices_positions, vertices_count * 3 * sizeof(GLfloat);
			memcpy(this->uv_coordinates, src_objects_by_material.uv_coordinates, vertices_count * 2 * sizeof(GLfloat));
			memcpy(this->normal_vectors, src_objects_by_material.normal_vectors, vertices_count * 3 * sizeof(GLfloat));

			this->vertices_acummulated_initial_positions = src_objects_by_material.vertices_acummulated_initial_positions;
		}
		return *this;
	}


	inline bool operator==(const objects_by_material & right_objects_by_material)
	{
		if (strcmp(this->material_identifier, right_objects_by_material.material_identifier) == 0)
		{
			return true;
		}

		return false;
	}


	~objects_by_material()
	{
		if (this->material_identifier)
		{
			delete[] this->material_identifier;
		}

		if (this->vertices_positions)
		{
			delete[] this->vertices_positions;
		}

		if (this->uv_coordinates)
		{
			delete[] this->uv_coordinates;
		}

		if (this->normal_vectors)
		{
			delete[] this->normal_vectors;
		}
	}
} objects_by_material;



class OBJECTS_MANAGER
{
public:	

	OBJECTS_MANAGER();
	OBJECTS_MANAGER(GEOMETRIES_MANAGER * src_geometries_manager);
	OBJECTS_MANAGER(const OBJECTS_MANAGER& src_node_figure);
	~OBJECTS_MANAGER();
	
	void assignGeometriesManager(GEOMETRIES_MANAGER * src_geometries_manager);

	void addNode(const object_geometry_type src_new_node_type, const GLfloat src_position_x = 0.0f, const GLfloat src_position_y = 0.0f, const GLfloat src_position_z = 0.0f);

	void moveNode(const unsigned int src_node_index, const float delta_x, const float delta_y, const float delta_z = 0.0f);
	void scaleNode(const unsigned int src_node_index, const float src_scale);
	void rotateNode(const unsigned int src_node_index, const float src_rotation_angle);

	void moveAllNodes(const float delta_x, const float delta_y, const float delta_z = 0.0f);
	void scaleAllNodes(const float scale_axis_x, const float scale_axis_y, const float scale_axis_z = 1.0f);
	void rotateAllNodes(const float angle_axis_x, const float angle_axis_y, const float angle_axis_z = 0.0f);

	unsigned int getNodesCount();

	void dumpNodesGeometries();

private:
	
	unsigned int nodes_count;

	bool a_node_was_added;
	bool a_node_was_moved;

	std::list<unsigned int> moved_nodes_indices;
	std::list<unsigned int> added_nodes_indices;

	glm::mat4 nodes_scale;
	glm::mat4 nodes_rotation;
	glm::vec4 nodes_position;

	GEOMETRIES_MANAGER * my_geometries_manager;

	std::vector<node_geometry_location> objects_geometries_locations;
	std::vector<objects_by_material> objects_geometries_locations_by_material;
	
	void dumpNodesGeometriesMovedOnly();
	void dumpNodesGeometriesNewOnly();
};

#endif // OBJECTS_MANAGER_CLASS_H_INCLUDED
