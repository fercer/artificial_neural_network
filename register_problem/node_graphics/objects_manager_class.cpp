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
	all_nodes_were_moved = false;

	vertices_accumulated_count.push_back(0);
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
	all_nodes_were_moved = false;

	vertices_accumulated_count.push_back(0);
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
	this->all_nodes_were_moved = src_node_figure.all_nodes_were_moved;
	this->node_was_moved = src_node_figure.node_was_moved;

	this->vertices_accumulated_count = src_node_figure.vertices_accumulated_count;

	// Copy the nodes objects
	for (unsigned int node_index = 0; node_index < (unsigned int)src_node_figure.objects_geometries_locations.size(); node_index++)
	{
		node_geometry_location * current_node_geometry = new node_geometry_location(src_node_figure.objects_geometries_locations.at(node_index));
		this->objects_geometries_locations.push_back(current_node_geometry);
	}
}



OBJECTS_MANAGER::~OBJECTS_MANAGER()
{
	std::vector<node_geometry_location*>::iterator objects_geometries_locations_ini = objects_geometries_locations.begin();
	std::vector<node_geometry_location*>::iterator objects_geometries_locations_end = objects_geometries_locations.end();
	std::vector<node_geometry_location*>::iterator objects_geometries_locations_it;

	for (objects_geometries_locations_it = objects_geometries_locations_ini; objects_geometries_locations_it != objects_geometries_locations_end; objects_geometries_locations_it++)
	{
		delete *objects_geometries_locations_it;
	}
}

void OBJECTS_MANAGER::assignGeometriesManager(GEOMETRIES_MANAGER * src_geometries_manager)
{
	my_geometries_manager = src_geometries_manager;
}



void OBJECTS_MANAGER::addNode(const object_geometry_type src_new_node_type, const GLfloat src_position_x, const GLfloat src_position_y, const GLfloat src_position_z, const GLfloat src_scale)
{
	node_geometry_location * new_node_object = new node_geometry_location;

	new_node_object->position = glm::vec4(src_position_x, src_position_y, src_position_z, 1.0);
	new_node_object->node_geometry_type = src_new_node_type;

	new_node_object->geometry = my_geometries_manager->getGeometry(src_new_node_type);

	objects_geometries_locations.push_back(new_node_object);

	vertices_accumulated_count.push_back(new_node_object->geometry.triangles_count * 3 + vertices_accumulated_count.at(nodes_count));
	node_was_added.push_back(nodes_count);
	a_node_was_added = true;

	nodes_count++;
}



void OBJECTS_MANAGER::moveNode(const unsigned int src_node_index, const float delta_x, const float delta_y, const float delta_z)
{
	objects_geometries_locations.at(src_node_index)->position = objects_geometries_locations.at(src_node_index)->position + glm::vec4(delta_x, delta_y, delta_z, 0.0f);

	node_was_moved.push_back(src_node_index);
	a_node_was_moved = true;
}



void OBJECTS_MANAGER::moveAllNodes(const float delta_x, const float delta_y, const float delta_z)
{
	nodes_position = nodes_position + glm::vec4(delta_x, delta_y, delta_z, 0.0f);
	all_nodes_were_moved = true;
}



void OBJECTS_MANAGER::scaleAllNodes(const float scale_axis_x, const float scale_axis_y, const float scale_axis_z)
{
	nodes_scale[0].x = scale_axis_x;
	nodes_scale[1].y = scale_axis_y;
	nodes_scale[2].z = scale_axis_z;

	all_nodes_were_moved = true;
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

	all_nodes_were_moved = true;
}



unsigned int OBJECTS_MANAGER::getNodesCount()
{
	return nodes_count;
}

unsigned int OBJECTS_MANAGER::getVerticesCount()
{
	return vertices_accumulated_count.at(nodes_count);
}



void OBJECTS_MANAGER::dumpNodesGeometries(GLfloat * dst_vertices_positions, GLfloat * dst_uv_coordinates, GLfloat * dst_normal_vectors)
{
	if (a_node_was_added)
	{
		dumpNodesGeometriesNewOnly(dst_vertices_positions, dst_uv_coordinates, dst_normal_vectors);
	}

	if (a_node_was_moved)
	{
		dumpNodesGeometriesMovedOnly(dst_vertices_positions, dst_uv_coordinates, dst_normal_vectors);
	}
}



void OBJECTS_MANAGER::dumpNodesGeometriesMovedOnly(GLfloat * dst_vertices_positions, GLfloat * dst_uv_coordinates, GLfloat * dst_normal_vectors)
{
	// Iterate for each node object in this manager:
	GLfloat * dst_vertices_positions_it;

	std::vector<node_geometry_location*>::iterator objects_geometries_locations_ini = objects_geometries_locations.begin();
	std::vector<node_geometry_location*>::iterator objects_geometries_locations_it;

	std::vector<glm::vec4>::iterator vertices_positions_end, vertices_positions_it;

	glm::vec4 temp_position;

	std::vector<unsigned int>::iterator node_was_moved_ini = node_was_moved.begin();
	std::vector<unsigned int>::iterator node_was_moved_end = node_was_moved.end();
	std::vector<unsigned int>::iterator node_was_moved_it;

	for (node_was_moved_it = node_was_moved_ini; node_was_moved_it != node_was_moved_end; node_was_moved_it++)
	{
		objects_geometries_locations_it = objects_geometries_locations_ini + *node_was_moved_it;
		dst_vertices_positions_it = dst_vertices_positions + 3 * vertices_accumulated_count.at(*node_was_moved_it);

		// Perform the transformation of each node previous to the dumping:
		vertices_positions_it = (*objects_geometries_locations_it)->geometry.vertices_positions.begin();
		vertices_positions_end = (*objects_geometries_locations_it)->geometry.vertices_positions.end();

		for (; vertices_positions_it < vertices_positions_end; vertices_positions_it++)
		{
			temp_position = nodes_rotation * nodes_scale * *vertices_positions_it + (*objects_geometries_locations_it)->position + nodes_position;

			// Copy each position to the "dst_vertices_positions" array:
			*(dst_vertices_positions_it++) = (GLfloat)temp_position.x;
			*(dst_vertices_positions_it++) = (GLfloat)temp_position.y;
			*(dst_vertices_positions_it++) = (GLfloat)temp_position.z;
		}
	}

	node_was_moved.clear();
	a_node_was_moved = false;
}



void OBJECTS_MANAGER::dumpNodesGeometriesNewOnly(GLfloat * dst_vertices_positions, GLfloat * dst_uv_coordinates, GLfloat * dst_normal_vectors)
{
	// Iterate for each node object in this manager:
	GLfloat * dst_vertices_positions_it;
	GLfloat * dst_uv_coordinates_it;
	GLfloat * dst_normal_vectors_it;

	std::vector<node_geometry_location*>::iterator objects_geometries_locations_ini = objects_geometries_locations.begin();
	std::vector<node_geometry_location*>::iterator objects_geometries_locations_it;

	std::vector<glm::vec4>::iterator vertices_positions_end, vertices_positions_it;
	std::vector<glm::vec2>::iterator uv_coordinates_end, uv_coordinates_it;
	std::vector<glm::vec3>::iterator normal_vectors_end, normal_vectors_it;

	glm::vec4 temp_position;

	std::vector<unsigned int>::iterator node_was_added_ini = node_was_added.begin();
	std::vector<unsigned int>::iterator node_was_added_end = node_was_added.end();
	std::vector<unsigned int>::iterator node_was_added_it;

	for (node_was_added_it = node_was_added_ini; node_was_added_it != node_was_added_end; node_was_added_it++)
	{
		objects_geometries_locations_it = objects_geometries_locations_ini + *node_was_added_it;
		dst_vertices_positions_it = dst_vertices_positions + 3 * vertices_accumulated_count.at(*node_was_added_it);
		dst_uv_coordinates_it = dst_uv_coordinates + 2 * vertices_accumulated_count.at(*node_was_added_it);
		dst_normal_vectors_it = dst_normal_vectors + 3 * vertices_accumulated_count.at(*node_was_added_it);

		// Perform the transformation of each node previous to the dumping:
		vertices_positions_it = (*objects_geometries_locations_it)->geometry.vertices_positions.begin();
		vertices_positions_end = (*objects_geometries_locations_it)->geometry.vertices_positions.end();
		
		uv_coordinates_it = (*objects_geometries_locations_it)->geometry.uv_coordinates.begin();
		uv_coordinates_end = (*objects_geometries_locations_it)->geometry.uv_coordinates.end();

		normal_vectors_it = (*objects_geometries_locations_it)->geometry.normal_vectors.begin();
		normal_vectors_end = (*objects_geometries_locations_it)->geometry.normal_vectors.end();

		for (; vertices_positions_it < vertices_positions_end; vertices_positions_it++, uv_coordinates_it++, normal_vectors_it++)
		{
			temp_position = nodes_rotation * nodes_scale * *vertices_positions_it + (*objects_geometries_locations_it)->position + nodes_position;

			// Copy each position to the "dst_vertices_positions" array:
			*(dst_vertices_positions_it++) = (GLfloat)temp_position.x;
			*(dst_vertices_positions_it++) = (GLfloat)temp_position.y;
			*(dst_vertices_positions_it++) = (GLfloat)temp_position.z;

			// Copy each uv coordinate to the "dst_uv_coordinates_it" array:
			*(dst_uv_coordinates_it++) = (GLfloat)uv_coordinates_it->x;
			*(dst_uv_coordinates_it++) = (GLfloat)uv_coordinates_it->y;

			// Copy each normal vector to the "dst_normal_vectors" array:
			*(dst_normal_vectors_it++) = (GLfloat)normal_vectors_it->x;
			*(dst_normal_vectors_it++) = (GLfloat)normal_vectors_it->y;
			*(dst_normal_vectors_it++) = (GLfloat)normal_vectors_it->z;
		}
	}

	node_was_added.clear();
	a_node_was_added = false;
}