#ifndef GEOMETRIES_MANAGER_CLASS_H_INCLUDED
#define GEOMETRIES_MANAGER_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include "GL/glew.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

#include <string.h>
#include "../image_functions.h"

class GEOMETRIES_MANAGER
{
public:
	typedef enum object_geometry_type
	{
		OBJGEOM_UNASSIGNED = -1,
		OBJGEOM_LINK_ICON = 0,
		OBJGEOM_IMAGE_LINK_ICON = 1,
		OBJGEOM_NUMERIC_NODE = 2,
		OBJGEOM_STRING_NODE = 3,
		OBJGEOM_NODE_OPERATION = 4,
		OBJGEOM_IMAGE_SCALAR_OPERATION = 5,
		OBJGEOM_IMAGE_OPERATION = 6
	} object_geometry_type;
	
	typedef struct node_geometry
	{
		object_geometry_type node_geometry_type;
		unsigned int triangles_count;
		char material_identifier[32];
		std::vector<glm::vec4> vertices_positions;
		std::vector<glm::vec2> uv_coordinates;
		std::vector<glm::vec3> normal_vectors;

		node_geometry()
		{
			node_geometry_type = OBJGEOM_UNASSIGNED;
			triangles_count = 0;
			*material_identifier = '\0';			
		}

		node_geometry(const node_geometry & src_node_geometry)
		{
			node_geometry_type = src_node_geometry.node_geometry_type;
			triangles_count = src_node_geometry.triangles_count;
			strcpy(material_identifier, src_node_geometry.material_identifier);
			vertices_positions = src_node_geometry.vertices_positions;
			uv_coordinates = src_node_geometry.uv_coordinates;
			normal_vectors = src_node_geometry.normal_vectors;
		}

		node_geometry(const node_geometry * src_node_geometry)
		{
			node_geometry_type = src_node_geometry->node_geometry_type;
			triangles_count = src_node_geometry->triangles_count;
			strcpy(material_identifier, src_node_geometry->material_identifier);
			vertices_positions = src_node_geometry->vertices_positions;
			uv_coordinates = src_node_geometry->uv_coordinates;
			normal_vectors = src_node_geometry->normal_vectors;
		}
	} node_geometry;

	GEOMETRIES_MANAGER();
	GEOMETRIES_MANAGER(const GEOMETRIES_MANAGER& src_node_figure);
	~GEOMETRIES_MANAGER();

	void loadObj(const char * src_obj_filename);
	
	node_geometry * getGeometry(const object_geometry_type src_object_type);

private:

	int mtl_read_status;
	int obj_read_status;
	int texture_read_status;

	IMG_DATA * texture_map;
	unsigned int nodes_count;
	std::vector<node_geometry*> objects_compounds;
		
	int readObjFile(const char * src_obj_filename);
	int readMtlFile(const char * src_mtl_filename);
	IMG_DATA * readTextureFile(const char * src_texture_filename);

	IMG_DATA * loadPGM_ascii(const char * src_filename);
	IMG_DATA * loadPGM_raw(const char * src_filename);
	IMG_DATA * loadPPM_ascii(const char * src_filename);
	IMG_DATA * loadPPM_raw(const char * src_filename);
};

#endif // GEOMETRIES_MANAGER_CLASS_H_INCLUDED
