#include "interface_manager_class.h"

INTERFACE_MANAGER::INTERFACE_MANAGER()
{
	my_objects_manager.assignGeometriesManager(&my_geometries_manager);
}

INTERFACE_MANAGER::INTERFACE_MANAGER(const INTERFACE_MANAGER & src_node_figure)
{
	this->my_objects_manager.assignGeometriesManager(&my_geometries_manager);
	this->objects_geometries_locations_by_material = src_node_figure.objects_geometries_locations_by_material;
}

INTERFACE_MANAGER::~INTERFACE_MANAGER()
{
}

void INTERFACE_MANAGER::loadObj(const char * src_obj_filename)
{
	my_geometries_manager.loadObj(src_obj_filename);
}

void INTERFACE_MANAGER::assignTextureMap(char * src_material_identifier, IMG_DATA * src_texture_map)
{
	// Assign the texture map to the specified material identifier:
	texture_map_pair looking_for_material(src_material_identifier, NULL, false);
	std::vector<texture_map_pair>::iterator my_geometries_manager_textures_end = my_geometries_manager.getTexturesPairsEnd();

	std::vector<texture_map_pair>::iterator my_geometries_manager_textures_it = std::find(my_geometries_manager.getTexturesPairsIni(), my_geometries_manager_textures_end, looking_for_material);

	if (my_geometries_manager_textures_it != my_geometries_manager_textures_end)
	{
		my_geometries_manager_textures_it->texture_map = src_texture_map;
	}
}

void INTERFACE_MANAGER::addNode(const object_geometry_type src_new_node_type, const GLfloat src_position_x, const GLfloat src_position_y, const GLfloat src_position_z, const GLfloat src_scale)
{
	my_objects_manager.addNode(src_new_node_type, src_position_x, src_position_y, src_position_z, src_scale);
}

void INTERFACE_MANAGER::moveNode(const unsigned int src_node_index, const float delta_x, const float delta_y, const float delta_z)
{
	my_objects_manager.moveNode(src_node_index, delta_x, delta_y, delta_z);
}

void INTERFACE_MANAGER::moveAllNodes(const float delta_x, const float delta_y, const float delta_z)
{
	my_objects_manager.moveAllNodes(delta_x, delta_y, delta_z);
}

void INTERFACE_MANAGER::scaleAllNodes(const float scale_axis_x, const float scale_axis_y, const float scale_axis_z)
{
	my_objects_manager.scaleAllNodes(scale_axis_x, scale_axis_y, scale_axis_z);
}

void INTERFACE_MANAGER::rotateAllNodes(const float angle_axis_x, const float angle_axis_y, const float angle_axis_z)
{
	my_objects_manager.rotateAllNodes(angle_axis_x, angle_axis_y, angle_axis_z);
}
