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
	objs_manager.moveAllNodes(10.0f, 0.0f, -0.5f);
	objs_manager.dumpNodesGeometries(vertices_positions_ptr, uv_coordinates_ptr, normal_vectors_ptr);
	objs_manager.moveAllNodes(0.0f, 10.0f, 0.5f);
	objs_manager.dumpNodesGeometries(vertices_positions_ptr, uv_coordinates_ptr, normal_vectors_ptr);

	FILE * fp_dump = fopen("dumped_vertices.dat", "w");
	GLfloat * vertices_positions_ptr_it = vertices_positions_ptr;
	for (unsigned int vertex_index = 0; vertex_index < vertices_count; vertex_index++)
	{
		fprintf(fp_dump, "%i\t%f\t%f\t%f\n", vertex_index, (float)*(vertices_positions_ptr_it++), (float)*(vertices_positions_ptr_it++), (float)*(vertices_positions_ptr_it++));
	}
	fclose(fp_dump);

	free(vertices_positions_ptr);
	free(uv_coordinates_ptr);
	free(normal_vectors_ptr);

	return 0;
}