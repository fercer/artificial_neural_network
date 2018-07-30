#include <stdlib.h>
#include <stdio.h>

#include <math.h>

#include "process_visualizer_class.h"
#include "../src/random_numbers_generator.h"
#include "../src/data_handler_class.h"
#include "../src/args_fercer.h"
#include "image_process_handler_class.h"

int main(int argc, char * argv[])
{

	GEOMETRIES_MANAGER geom_manager;

	geom_manager.loadObj("node_model.obj");

	GEOMETRIES_MANAGER::node_geometry my_node_geometry(geom_manager.getGeometry(GEOMETRIES_MANAGER::OBJGEOM_IMAGE_LINK_ICON));

	printf("%s, triangles count: %i\n", my_node_geometry.material_identifier, my_node_geometry.triangles_count);
	printf("Positions count: %i, uv coordinates count: %i, normals count: %i\n", (int)my_node_geometry.vertices_positions.size(), (int)my_node_geometry.uv_coordinates.size(), (int)my_node_geometry.normal_vectors.size());

	for (unsigned int triangle_index = 0; triangle_index < my_node_geometry.triangles_count; triangle_index++)
	{
			printf("[%i] positions: (%.3f, %.3f, %.3f), (%.3f, %.3f, %.3f), (%.3f, %.3f, %.3f);\nuv coordinates: (%.3f, %.3f), (%.3f, %.3f), (%.3f, %.3f);\n normal vectors: (%.3f, %.3f, %.3f), (%.3f, %.3f, %.3f), (%.3f, %.3f, %.3f)\n", triangle_index, 
				my_node_geometry.vertices_positions.at(triangle_index * 3).x, my_node_geometry.vertices_positions.at(triangle_index * 3).y, my_node_geometry.vertices_positions.at(triangle_index * 3).z,
				my_node_geometry.vertices_positions.at(triangle_index * 3 + 1).x, my_node_geometry.vertices_positions.at(triangle_index * 3 + 1).y, my_node_geometry.vertices_positions.at(triangle_index * 3 + 1).z,
				my_node_geometry.vertices_positions.at(triangle_index * 3 + 2).x, my_node_geometry.vertices_positions.at(triangle_index * 3 + 2).y, my_node_geometry.vertices_positions.at(triangle_index * 3 + 2).z,
				my_node_geometry.uv_coordinates.at(triangle_index * 3).x, my_node_geometry.uv_coordinates.at(triangle_index * 3).y,
				my_node_geometry.uv_coordinates.at(triangle_index * 3 + 1).x, my_node_geometry.uv_coordinates.at(triangle_index * 3 + 1).y,
				my_node_geometry.uv_coordinates.at(triangle_index * 3 + 2).x, my_node_geometry.uv_coordinates.at(triangle_index * 3 + 2).y,
				my_node_geometry.normal_vectors.at(triangle_index * 3).x, my_node_geometry.normal_vectors.at(triangle_index * 3).y, my_node_geometry.normal_vectors.at(triangle_index * 3).z,
				my_node_geometry.normal_vectors.at(triangle_index * 3 + 1).x, my_node_geometry.normal_vectors.at(triangle_index * 3 + 1).y, my_node_geometry.normal_vectors.at(triangle_index * 3 + 1).z,
				my_node_geometry.normal_vectors.at(triangle_index * 3 + 2).x, my_node_geometry.normal_vectors.at(triangle_index * 3 + 2).y, my_node_geometry.normal_vectors.at(triangle_index * 3 + 2).z
				);
	}

	return 0;
}