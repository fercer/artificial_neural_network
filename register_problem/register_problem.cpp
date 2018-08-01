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

	OBJECTS_MANAGER  objs_manager;
	objs_manager.assignGeometriesManager(&geom_manager);

	objs_manager.addNode(OBJGEOM_IMAGE_LINK_ICON);
	objs_manager.addNode(OBJGEOM_LINK_ICON);
	objs_manager.addNode(OBJGEOM_IMAGE_OPERATION);
	objs_manager.addNode(OBJGEOM_IMAGE_SCALAR_OPERATION);

	unsigned int vertices_count = objs_manager.getVerticesCount();
	printf("Vertices count: %i\n", vertices_count);

	GLfloat * vertices_positions_ptr = (GLfloat*)malloc(vertices_count * 3 * sizeof(GLfloat));
	GLfloat * uv_coordinates_ptr = (GLfloat*)malloc(vertices_count * 2 * sizeof(GLfloat));
	GLfloat * normal_vectors_ptr = (GLfloat*)malloc(vertices_count * 3 * sizeof(GLfloat));

	objs_manager.dumpNodesGeometries(vertices_positions_ptr, uv_coordinates_ptr, normal_vectors_ptr);

	objs_manager.moveAllNodes(10.0, 0.0, 0.0);

	objs_manager.dumpNodesGeometries(vertices_positions_ptr, uv_coordinates_ptr, normal_vectors_ptr);

	objs_manager.moveNode(0, 10.0, 0.0, 0.0);

	objs_manager.dumpNodesGeometries(vertices_positions_ptr, uv_coordinates_ptr, normal_vectors_ptr);


	objs_manager.moveNode(3, 0.0, 10.0, 0.0);

	objs_manager.dumpNodesGeometries(vertices_positions_ptr, uv_coordinates_ptr, normal_vectors_ptr);
	return 0;
}