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
#include "textures_manager_class.h"

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



typedef struct compound_geometry
{
	unsigned int triangles_in_compound;
	char * material_identifier;

	std::vector<glm::vec4> vertices_positions;
	std::vector<glm::vec2> uv_coordinates;
	std::vector<glm::vec3> normal_vectors;

	compound_geometry()
	{
		material_identifier = NULL;
		triangles_in_compound = 0;
	}

	compound_geometry(const char * src_material_identifier)
	{
		unsigned int material_identifier_length = (unsigned int)strlen(src_material_identifier) + 1;
		material_identifier = new char[material_identifier_length];
		strcpy(material_identifier, src_material_identifier);

		triangles_in_compound = 0;
	}
	
	compound_geometry(const compound_geometry & src_node_geometry)
	{
		unsigned int material_identifier_length = (unsigned int)strlen(src_node_geometry.material_identifier) + 1;
		this->material_identifier = new char[material_identifier_length];
		strcpy(this->material_identifier, src_node_geometry.material_identifier);

		this->triangles_in_compound = src_node_geometry.triangles_in_compound;
				
		this->vertices_positions = src_node_geometry.vertices_positions;
		this->uv_coordinates = src_node_geometry.uv_coordinates;
		this->normal_vectors = src_node_geometry.normal_vectors;
	}

	compound_geometry(const compound_geometry * src_node_geometry)
	{
		unsigned int material_identifier_length = (unsigned int)strlen(src_node_geometry->material_identifier) + 1;
		this->material_identifier = new char[material_identifier_length];
		strcpy(this->material_identifier, src_node_geometry->material_identifier);

		this->triangles_in_compound = src_node_geometry->triangles_in_compound;

		this->vertices_positions = src_node_geometry->vertices_positions;
		this->uv_coordinates = src_node_geometry->uv_coordinates;
		this->normal_vectors = src_node_geometry->normal_vectors;
	}
	
	compound_geometry & operator=(const compound_geometry & src_node_geometry)
	{
		if (this != &src_node_geometry)
		{
			if (this->material_identifier)
			{
				delete[] this->material_identifier;
			}

			unsigned int material_identifier_length = (unsigned int)strlen(src_node_geometry.material_identifier) + 1;
			this->material_identifier = new char[material_identifier_length];
			strcpy(this->material_identifier, src_node_geometry.material_identifier);

			this->triangles_in_compound = src_node_geometry.triangles_in_compound;

			this->vertices_positions = src_node_geometry.vertices_positions;
			this->uv_coordinates = src_node_geometry.uv_coordinates;
			this->normal_vectors = src_node_geometry.normal_vectors;
		}

		return *this;
	}

	~compound_geometry()
	{
		if (this->material_identifier)
		{
			delete[] this->material_identifier;
		}
	}
	
} compound_geometry;



typedef struct node_geometry
{
	object_geometry_type compound_geometry_type;
	std::vector<compound_geometry> compounds_in_object;
	bool self_allocated_compounds;
	unsigned int triangles_in_object;

	node_geometry()
	{
		compound_geometry_type = OBJGEOM_UNASSIGNED;
		triangles_in_object = 0;
		self_allocated_compounds = true;
	}

	node_geometry(const object_geometry_type src_object_geometry_type)
	{
		compound_geometry_type = src_object_geometry_type;
		triangles_in_object = 0;
		self_allocated_compounds = true;
	}

	node_geometry(const node_geometry & src_node_geometry)
	{
		this->compound_geometry_type = src_node_geometry.compound_geometry_type;
		this->compounds_in_object = src_node_geometry.compounds_in_object;
		this->triangles_in_object = src_node_geometry.triangles_in_object;
		this->self_allocated_compounds = false;
	}

	node_geometry(node_geometry * src_node_geometry)
	{
		this->compound_geometry_type = src_node_geometry->compound_geometry_type;
		this->compounds_in_object = src_node_geometry->compounds_in_object;
		this->triangles_in_object = src_node_geometry->triangles_in_object;
		this->self_allocated_compounds = false;
	}
	
	node_geometry & operator=(const node_geometry & src_node_geometry)
	{
		if (this != &src_node_geometry)
		{
			this->compound_geometry_type = src_node_geometry.compound_geometry_type;
			this->compounds_in_object = src_node_geometry.compounds_in_object;
			this->triangles_in_object = src_node_geometry.triangles_in_object;
			this->self_allocated_compounds = false;
		}

		return *this;
	}

	inline bool operator==(const node_geometry & right_node_geometry)
	{
		return this->compound_geometry_type == right_node_geometry.compound_geometry_type;
	}

} node_geometry;


class GEOMETRIES_MANAGER
{
public:
	GEOMETRIES_MANAGER();
	GEOMETRIES_MANAGER(const GEOMETRIES_MANAGER& src_node_figure);
	~GEOMETRIES_MANAGER();

	void loadObj(const char * src_obj_filename);

	node_geometry getGeometry(const object_geometry_type src_object_type);

	TEXTURES_MANAGER * getTexturesManager();

private:
	unsigned int nodes_count;

	std::vector<node_geometry> objects_compounds_geometry;
	TEXTURES_MANAGER compounds_textures_manager;
	
	int readObjFile(const char * src_obj_filename);
	int readMtlFile(const char * src_mtl_filename);

	void centerObjectCompound();
};


#endif // GEOMETRIES_MANAGER_CLASS_H_INCLUDED
