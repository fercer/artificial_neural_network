#include "objects_manager_class.h"

OBJECTS_MANAGER::OBJECTS_MANAGER()
{
	nodes_count = 0;

	nodes_position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	nodes_rotation = glm::mat4(1.0f);
	nodes_scale = glm::mat4(1.0f);

	my_geometries_manager = NULL;

	a_node_was_moved = false;
	a_node_was_added = false;
}



OBJECTS_MANAGER::OBJECTS_MANAGER(GEOMETRIES_MANAGER * src_geometries_manager)
{
	nodes_count = 0;

	nodes_position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	nodes_rotation = glm::mat4(1.0f);
	nodes_scale = glm::mat4(1.0f);

	my_geometries_manager = src_geometries_manager;

	a_node_was_moved = false;
	a_node_was_added = false;
}



OBJECTS_MANAGER::OBJECTS_MANAGER(const OBJECTS_MANAGER & src_node_figure)
{
	this->nodes_count = src_node_figure.nodes_count;

	this->nodes_position = src_node_figure.nodes_position;
	this->nodes_rotation = src_node_figure.nodes_rotation;
	this->nodes_scale = src_node_figure.nodes_scale;

	this->my_geometries_manager = src_node_figure.my_geometries_manager;

	this->a_node_was_moved = src_node_figure.a_node_was_moved;
	this->a_node_was_added = src_node_figure.a_node_was_added;
	this->moved_nodes_indices = src_node_figure.moved_nodes_indices;
	
	// Copy the nodes objects
	for (unsigned int node_index = 0; node_index < (unsigned int)src_node_figure.objects_geometries_locations.size(); node_index++)
	{
		node_geometry_location * current_node_geometry = new node_geometry_location(src_node_figure.objects_geometries_locations.at(node_index));
		this->objects_geometries_locations.push_back(current_node_geometry);
	}
}



OBJECTS_MANAGER::~OBJECTS_MANAGER()
{
}



void OBJECTS_MANAGER::assignGeometriesManager(GEOMETRIES_MANAGER * src_geometries_manager)
{
	my_geometries_manager = src_geometries_manager;
}



void OBJECTS_MANAGER::addNode(const object_geometry_type src_new_node_type, const GLfloat src_position_x, const GLfloat src_position_y, const GLfloat src_position_z)
{
	objects_geometries_locations.push_back(node_geometry_location(src_position_x, src_position_y, src_position_z));
	objects_geometries_locations.at(nodes_count).node_object_geometry = my_geometries_manager->getGeometry(src_new_node_type);

	std::vector<compound_geometry>::iterator compounds_in_object_ini = objects_geometries_locations.at(nodes_count).node_object_geometry.compounds_in_object.begin();

	std::vector<compound_geometry>::iterator compounds_in_object_end = objects_geometries_locations.at(nodes_count).node_object_geometry.compounds_in_object.end();

	std::vector<compound_geometry>::iterator compounds_in_object_it;

	std::vector<objects_by_material>::iterator my_objects_by_material_ini, my_objects_by_material_end, my_objects_by_material_it;

	for (compounds_in_object_it = compounds_in_object_ini; compounds_in_object_it != compounds_in_object_end; compounds_in_object_it++)
	{
		objects_by_material search_new_node_material(compounds_in_object_it->material_identifier);

		my_objects_by_material_ini = objects_geometries_locations_by_material.begin();
		my_objects_by_material_end = objects_geometries_locations_by_material.end();
		my_objects_by_material_it = std::find(my_objects_by_material_ini, my_objects_by_material_end, search_new_node_material);

		if (my_objects_by_material_it == my_objects_by_material_end)
		{
			my_objects_by_material_it = my_objects_by_material_ini + objects_geometries_locations_by_material.size();
			objects_geometries_locations_by_material.push_back(search_new_node_material);
		}

		my_objects_by_material_it->vertices_acummulated_initial_positions.push_back(3 * compounds_in_object_it->triangles_in_compound);
		my_objects_by_material_it->corresponding_node_indices.push_back(nodes_count);

		/* RESIZE THE MATERIAL VERTICES ARRAY */
	}

	added_nodes_indices.push_back(nodes_count);
	a_node_was_added = true;

	nodes_count++;
}



void OBJECTS_MANAGER::moveNode(const unsigned int src_node_index, const float delta_x, const float delta_y, const float delta_z)
{
	objects_geometries_locations.at(src_node_index).node_position = objects_geometries_locations.at(src_node_index).node_position + glm::vec4(delta_x, delta_y, delta_z, 0.0f);

	moved_nodes_indices.push_back(src_node_index);
	a_node_was_moved = true;
}



void OBJECTS_MANAGER::scaleNode(const unsigned int src_node_index, const float src_scale)
{
	objects_geometries_locations.at(src_node_index).node_scale[0].x = src_scale;
	objects_geometries_locations.at(src_node_index).node_scale[1].y = src_scale;
	objects_geometries_locations.at(src_node_index).node_scale[2].z = src_scale;

	moved_nodes_indices.push_back(src_node_index);
	a_node_was_moved = true;
}



void OBJECTS_MANAGER::rotateNode(const unsigned int src_node_index, const float src_rotation_angle)
{
	const float cos_rotation = cosf(src_rotation_angle);
	const float sin_rotation = sinf(src_rotation_angle);
	
	objects_geometries_locations.at(src_node_index).node_rotation[0].x = cos_rotation;
	objects_geometries_locations.at(src_node_index).node_rotation[0].y = -sin_rotation;
	objects_geometries_locations.at(src_node_index).node_rotation[1].x = sin_rotation;
	objects_geometries_locations.at(src_node_index).node_rotation[1].y = cos_rotation;

	moved_nodes_indices.push_back(src_node_index);
	a_node_was_moved = true;
}



void OBJECTS_MANAGER::moveAllNodes(const float delta_x, const float delta_y, const float delta_z)
{
	nodes_position = nodes_position + glm::vec4(delta_x, delta_y, delta_z, 0.0f);

	for (unsigned int node_index = 0; node_index < nodes_count; node_index++)
	{
		moved_nodes_indices.push_back(node_index);
	}

	a_node_was_moved = true;
}



void OBJECTS_MANAGER::scaleAllNodes(const float scale_axis_x, const float scale_axis_y, const float scale_axis_z)
{
	nodes_scale[0].x = scale_axis_x;
	nodes_scale[1].y = scale_axis_y;
	nodes_scale[2].z = scale_axis_z;

	for (unsigned int node_index = 0; node_index < nodes_count; node_index++)
	{
		moved_nodes_indices.push_back(node_index);
	}

	a_node_was_moved = true;
}



void OBJECTS_MANAGER::rotateAllNodes(const float angle_axis_x, const float angle_axis_y, const float angle_axis_z)
{
	glm::mat4 rotation_x = glm::mat4(1.0f);
	glm::mat4 rotation_y = glm::mat4(1.0f);
	glm::mat4 rotation_z = glm::mat4(1.0f);

	rotation_x[1].y = cosf(angle_axis_x);
	rotation_x[1].z = -sinf(angle_axis_x);
	rotation_x[2].y = sinf(angle_axis_x);
	rotation_x[2].z = cosf(angle_axis_x);

	rotation_y[0].x = cosf(angle_axis_y);
	rotation_y[0].z = -sinf(angle_axis_y);
	rotation_y[2].y = sinf(angle_axis_y);
	rotation_y[2].z = cosf(angle_axis_y);

	rotation_z[0].x = cosf(angle_axis_z);
	rotation_z[0].y = -sinf(angle_axis_z);
	rotation_z[1].x = sinf(angle_axis_z);
	rotation_z[1].y = cosf(angle_axis_z);

	nodes_rotation = rotation_x * rotation_y * rotation_z;

	for (unsigned int node_index = 0; node_index < nodes_count; node_index++)
	{
		moved_nodes_indices.push_back(node_index);
	}

	a_node_was_moved = true;
}



unsigned int OBJECTS_MANAGER::getNodesCount()
{
	return nodes_count;
}




void OBJECTS_MANAGER::dumpNodesGeometries()
{
	if (a_node_was_added)
	{
		dumpNodesGeometriesNewOnly();
	}

	if (a_node_was_moved)
	{
		dumpNodesGeometriesMovedOnly();
	}
}



void OBJECTS_MANAGER::dumpNodesGeometriesMovedOnly()
{
	
	moved_nodes_indices.clear();
	a_node_was_moved = false;
}



void OBJECTS_MANAGER::dumpNodesGeometriesNewOnly()
{
	std::vector<node_geometry_location>::iterator objects_geometries_locations_ini = objects_geometries_locations.begin();
	std::vector<node_geometry_location>::iterator objects_geometries_locations_end = objects_geometries_locations.end();
	std::vector<node_geometry_location>::iterator objects_geometries_locations_it;
	

	std::vector<objects_by_material>::iterator objects_geometries_locations_by_material_ini = objects_geometries_locations_by_material.begin();
	std::vector<objects_by_material>::iterator objects_geometries_locations_by_material_end = objects_geometries_locations_by_material.end();
	std::vector<objects_by_material>::iterator objects_geometries_locations_by_material_it;

	std::vector<compound_geometry>::iterator compounds_in_object_ini, compounds_in_object_end, compounds_in_object_it;

	std::vector<unsigned int>::iterator corresponding_node_indices_ini, corresponding_node_indices_end, corresponding_node_indices_it;

	GLfloat * vertices_positions_ptr;
	GLfloat * uv_coordinates_ptr;
	GLfloat * normal_vectors_ptr;

	unsigned int current_node_index;
	unsigned int current_node_index_in_material;
	unsigned int vertices_pointer_offset;
	while (!added_nodes_indices.empty())
	{
		current_node_index = added_nodes_indices.back();
		added_nodes_indices.pop_back();

		objects_geometries_locations_it = objects_geometries_locations_ini + current_node_index;

		compounds_in_object_ini = objects_geometries_locations_it->node_object_geometry.compounds_in_object.begin();
		compounds_in_object_end = objects_geometries_locations_it->node_object_geometry.compounds_in_object.end();

		for (compounds_in_object_it = compounds_in_object_ini; compounds_in_object_it != compounds_in_object_end; compounds_in_object_it++)
		{
			objects_by_material search_new_node_material(compounds_in_object_it->material_identifier);

			objects_geometries_locations_by_material_it = std::find(objects_geometries_locations_by_material_ini, objects_geometries_locations_by_material_end, search_new_node_material);

			if (objects_geometries_locations_by_material_it != objects_geometries_locations_by_material_end)
			{
				// Look for the corresponding index:
				corresponding_node_indices_ini = objects_geometries_locations_by_material_it->corresponding_node_indices.begin();
				corresponding_node_indices_end = objects_geometries_locations_by_material_it->corresponding_node_indices.end();

				corresponding_node_indices_it = std::find(corresponding_node_indices_ini, corresponding_node_indices_end, current_node_index);
				current_node_index_in_material = (unsigned int)std::distance(corresponding_node_indices_ini, corresponding_node_indices_it);

				vertices_pointer_offset = objects_geometries_locations_by_material_it->vertices_acummulated_initial_positions.at(current_node_index_in_material);

				vertices_positions_ptr = objects_geometries_locations_by_material_it->vertices_positions + 3 * vertices_pointer_offset;
				uv_coordinates_ptr = objects_geometries_locations_by_material_it->uv_coordinates + 2 * vertices_pointer_offset;
				normal_vectors_ptr = objects_geometries_locations_by_material_it->normal_vectors + 3 * vertices_pointer_offset;

				/* COPY THE VERTICES POSITIONS AFTER THE 
				LOCAL_ROTATION * LOCAL_SCALE * MODEL + LOCAL_DISPLACEMENT 
				*/
			}
		}

	}

	a_node_was_added = false;
}