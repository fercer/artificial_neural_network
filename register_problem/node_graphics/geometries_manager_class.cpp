#include "geometries_manager_class.h"

GEOMETRIES_MANAGER::GEOMETRIES_MANAGER()
{
	objects_compounds_geometry.push_back(node_geometry(OBJGEOM_LINK_ICON));
	objects_compounds_geometry.push_back(node_geometry(OBJGEOM_IMAGE_LINK_ICON));
	objects_compounds_geometry.push_back(node_geometry(OBJGEOM_NUMERIC_NODE));
	objects_compounds_geometry.push_back(node_geometry(OBJGEOM_STRING_NODE));
	objects_compounds_geometry.push_back(node_geometry(OBJGEOM_NODE_OPERATION));
	objects_compounds_geometry.push_back(node_geometry(OBJGEOM_IMAGE_SCALAR_OPERATION));
	objects_compounds_geometry.push_back(node_geometry(OBJGEOM_IMAGE_OPERATION));
}



GEOMETRIES_MANAGER::GEOMETRIES_MANAGER(const GEOMETRIES_MANAGER & src_node_figure)
{
	objects_compounds_geometry.push_back(node_geometry(OBJGEOM_LINK_ICON));
	objects_compounds_geometry.push_back(node_geometry(OBJGEOM_IMAGE_LINK_ICON));
	objects_compounds_geometry.push_back(node_geometry(OBJGEOM_NUMERIC_NODE));
	objects_compounds_geometry.push_back(node_geometry(OBJGEOM_STRING_NODE));
	objects_compounds_geometry.push_back(node_geometry(OBJGEOM_NODE_OPERATION));
	objects_compounds_geometry.push_back(node_geometry(OBJGEOM_IMAGE_SCALAR_OPERATION));
	objects_compounds_geometry.push_back(node_geometry(OBJGEOM_IMAGE_OPERATION));

}



GEOMETRIES_MANAGER::~GEOMETRIES_MANAGER()
{

}



void GEOMETRIES_MANAGER::loadObj(const char * src_obj_filename)
{
	readObjFile(src_obj_filename);
}



node_geometry GEOMETRIES_MANAGER::getGeometry(const object_geometry_type src_object_type)
{
	std::vector<node_geometry>::iterator objects_compounds_ini = objects_compounds_geometry.begin();
	std::vector<node_geometry>::iterator objects_compounds_end = objects_compounds_geometry.end();

	node_geometry searching_for_node(src_object_type);
	std::vector<node_geometry>::iterator objects_compounds_it = std::find(objects_compounds_ini, objects_compounds_end, searching_for_node);

	if (objects_compounds_it == objects_compounds_end)
	{
		return NULL;
	}

	return *objects_compounds_it;
}

TEXTURES_MANAGER * GEOMETRIES_MANAGER::getTexturesManager()
{
	return &compounds_textures_manager;
}


int GEOMETRIES_MANAGER::readObjFile(const char * src_obj_filename)
{
	FILE * fp_obj = fopen(src_obj_filename, "r");
	if (!fp_obj)
	{
		fprintf(stderr, "<<Error: Obj file: \"%s\", could not be opened>>\n", src_obj_filename);
		return 0;
	}

	char read_line[512];
	char material_filename[64];
	*material_filename = '\0';

	std::vector<glm::vec4> unique_position_vertices;
	std::vector<glm::vec2> unique_uv_coordinates;
	std::vector<glm::vec3> unique_normal_vectors;

	compound_geometry * active_object_compound = NULL;

	char * extract_string_ini;
	char * extract_string_end;
	char * new_line_position;
	unsigned int characters_count;
	char * material_identifier_string;

	char object_name[9];
	int object_index;
	int object_code;
	char numeric_value[64];

	GLfloat p_1, p_2, p_3;
	int index_v, index_t, index_n;

	// Read the lines of the .obj file:
	while (1)
	{
		fgets(read_line, 512, fp_obj);

		if (feof(fp_obj))
		{
			break;
		}

		new_line_position = strchr(read_line, '\n');
		const unsigned int characters_in_line = new_line_position - read_line;
		*(read_line + characters_in_line) = '\0';

		switch ((int)*read_line)
		{
			/* ------------------------------ Commentary ------------------------------*/
		case (int)'#':
			// Ignore the commentaries ...
			break;

			/*------------------------------ Material filename ------------------------------ */
		case (int)'m':
			extract_string_ini = strchr(read_line, ' ');
			characters_count = new_line_position - extract_string_ini;
			strncpy(material_filename, extract_string_ini + 1, characters_count);
			break;
			
			/* ------------------------------ Object identifier: ------------------------------
			Ico_001: Link icon,
			Ico_002: Image link icon,
			Obj_001: Numeric node,
			Obj_002: String node,
			Obj_003: Numeric nodes operation,
			Obj_004: Image scalar operation,
			Obj_005: Image operation
			*/
		case (int)'o':
			object_name[8] = '\0';
			strncpy(object_name, read_line + 2, 8);

			object_code = *object_name * *object_name;
			object_code += *(object_name + 1) * *(object_name + 1);
			object_code += *(object_name + 2) * *(object_name + 2);

			object_index = atoi(object_name + 4);

			active_object_compound = new compound_geometry;

			if (object_code == 27451) /* Icon: */
			{
				switch (object_index)
				{
				case 1:
					objects_compounds_geometry.at(0).compounds_in_object.push_back(active_object_compound);
					break;

				case 2:
					objects_compounds_geometry.at(1).compounds_in_object.push_back(active_object_compound);
					break;

				default:
					fprintf(stderr, "<<Error: Icon \"%i\" not identified in object \"%s\", while reading \"%s\">>\n", object_index, object_name, src_obj_filename);
					break;
				}
			}
			else if (object_code == 27081) /* Node object */
			{
				switch (object_index)
				{
				case 1:
					objects_compounds_geometry.at(2).compounds_in_object.push_back(active_object_compound);
					break;

				case 2:
					objects_compounds_geometry.at(3).compounds_in_object.push_back(active_object_compound);
					break;

				case 3:
					objects_compounds_geometry.at(4).compounds_in_object.push_back(active_object_compound);
					break;

				case 4:
					objects_compounds_geometry.at(5).compounds_in_object.push_back(active_object_compound);
					break;

				case 5:
					objects_compounds_geometry.at(6).compounds_in_object.push_back(active_object_compound);
					break;

				default:
					fprintf(stderr, "<<Error: Node object \"%i\" not identified in object \"%s\", while reading \"%s\">>\n", object_index, object_name, src_obj_filename);
					break;
				}
			}
			else
			{
				fprintf(stderr, "<<Error: Object \"%s\" not identified while reading \"%s\">>\n", object_name, src_obj_filename);
				continue;
			}

			break;

			/* -------- Vertex identifier (vertex position, texture and normals) ----------- */
		case (int)'v':
			switch ((int)*(read_line + 1))
			{
			case (int)' ': /* Position vertex */
				extract_string_ini = read_line + 2;
				extract_string_end = strchr(extract_string_ini, ' ');
				characters_count = extract_string_end - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				*(numeric_value + characters_count) = '\0';
				p_1 = (float)atof(numeric_value);

				extract_string_ini = extract_string_end + 1;
				extract_string_end = strchr(extract_string_ini, ' ');
				characters_count = extract_string_end  - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				*(numeric_value + characters_count) = '\0';
				p_2 = (float)atof(numeric_value);

				extract_string_ini = extract_string_end + 1;
				characters_count = new_line_position - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				*(numeric_value + characters_count) = '\0';
				p_3 = (float)atof(numeric_value);

				unique_position_vertices.push_back(glm::vec4(p_1, p_3, p_2, 1.0));

				break;

			case (int)'t': /* Texture uv coordinate */
				extract_string_ini = read_line + 3;
				extract_string_end = strchr(extract_string_ini, ' ');
				characters_count = extract_string_end - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				*(numeric_value + characters_count) = '\0';
				p_1 = (float)atof(numeric_value);

				extract_string_ini = extract_string_end + 1;
				characters_count = new_line_position - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				*(numeric_value + characters_count) = '\0';
				p_2 = (float)atof(numeric_value);

				unique_uv_coordinates.push_back(glm::vec2(p_1, p_2));

				break;

			case (int)'n': /* Normal vector */
				extract_string_ini = read_line + 3;
				extract_string_end = strchr(extract_string_ini, ' ');
				characters_count = extract_string_end - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				*(numeric_value + characters_count) = '\0';
				p_1 = (float)atof(numeric_value);

				extract_string_ini = extract_string_end + 1;
				extract_string_end = strchr(extract_string_ini, ' ');
				characters_count = extract_string_end - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				*(numeric_value + characters_count) = '\0';
				p_2 = (float)atof(numeric_value);

				extract_string_ini = extract_string_end + 1;
				characters_count = new_line_position - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				*(numeric_value + characters_count) = '\0';
				p_3 = (float)atof(numeric_value);

				unique_normal_vectors.push_back(glm::vec3(p_1, p_3, p_2));
				break;

			default:
				fprintf(stderr, "<<Error: Unknown vertex identifier \"%s\" found while reading \"%s\">>\n", read_line, src_obj_filename);
				break;
			}

			break;

			/* ------------------------------ Use material (Material used for this object) ------------------------------ */
		case (int)'u':
			extract_string_ini = strchr(read_line, ' ') + 1;
			characters_count = new_line_position - extract_string_ini;
			material_identifier_string = new char[characters_count + 1];
			strcpy(material_identifier_string, extract_string_ini);

			compounds_textures_manager.newTexture(material_identifier_string, (IMG_DATA*)NULL);

			active_object_compound->material_identifier = new char[characters_count + 1];
			strcpy(active_object_compound->material_identifier, material_identifier_string);

			break;

			/* ------------------------------ Smoothing (Off in general) ------------------------------ */
		case (int)'s':
			// Ignore the smoothing index ...
			break;

			/* ------------------------------ Face, conformed of three vertices identifiers ------------------------------ */
		case (int)'f':
			/* Read first vertex position */
			extract_string_ini = read_line + 2;
			extract_string_end = strchr(extract_string_ini, '/');
			characters_count = extract_string_end - extract_string_ini;
			strncpy(numeric_value, extract_string_ini, characters_count);
			*(numeric_value + characters_count) = '\0';
			index_v = atoi(numeric_value) - 1;

			active_object_compound->vertices_positions.push_back(unique_position_vertices.at(index_v));

			/* Read first vertex texture uv coordinates */
			extract_string_ini = extract_string_end + 1;
			extract_string_end = strchr(extract_string_ini, '/');
			characters_count = extract_string_end - extract_string_ini;
			strncpy(numeric_value, extract_string_ini, characters_count);
			*(numeric_value + characters_count) = '\0';
			index_t = atoi(numeric_value) - 1;

			active_object_compound->uv_coordinates.push_back(unique_uv_coordinates.at(index_t));
			
			/* Read first vertex normal vector */
			extract_string_ini = extract_string_end + 1;
			extract_string_end = strchr(extract_string_ini, ' ');
			characters_count = extract_string_end - extract_string_ini;
			strncpy(numeric_value, extract_string_ini, characters_count);
			*(numeric_value + characters_count) = '\0';
			index_n = atoi(numeric_value) - 1;

			active_object_compound->normal_vectors.push_back(unique_normal_vectors.at(index_n));


			/* Read second vertex position */
			extract_string_ini = extract_string_end + 1;
			extract_string_end = strchr(extract_string_ini, '/');
			characters_count = extract_string_end - extract_string_ini;
			strncpy(numeric_value, extract_string_ini, characters_count);
			*(numeric_value + characters_count) = '\0';
			index_v = atoi(numeric_value) - 1;

			active_object_compound->vertices_positions.push_back(unique_position_vertices.at(index_v));

			/* Read second vertex texture uv coordinates */
			extract_string_ini = extract_string_end + 1;
			extract_string_end = strchr(extract_string_ini, '/');
			characters_count = extract_string_end - extract_string_ini;
			strncpy(numeric_value, extract_string_ini, characters_count);
			*(numeric_value + characters_count) = '\0';
			index_t = atoi(numeric_value) - 1;

			active_object_compound->uv_coordinates.push_back(unique_uv_coordinates.at(index_t));

			/* Read second vertex normal vector */
			extract_string_ini = extract_string_end + 1;
			extract_string_end = strchr(extract_string_ini, ' ');
			characters_count = extract_string_end - extract_string_ini;
			strncpy(numeric_value, extract_string_ini, characters_count);
			*(numeric_value + characters_count) = '\0';
			index_n = atoi(numeric_value) - 1;

			active_object_compound->normal_vectors.push_back(unique_normal_vectors.at(index_n));


			/* Read third vertex position */
			extract_string_ini = extract_string_end + 1;
			extract_string_end = strchr(extract_string_ini, '/');
			characters_count = extract_string_end - extract_string_ini;
			strncpy(numeric_value, extract_string_ini, characters_count);
			*(numeric_value + characters_count) = '\0';
			index_v = atoi(numeric_value) - 1;

			active_object_compound->vertices_positions.push_back(unique_position_vertices.at(index_v));

			/* Read third vertex texture uv coordinates */
			extract_string_ini = extract_string_end + 1;
			extract_string_end = strchr(extract_string_ini, '/');
			characters_count = extract_string_end - extract_string_ini;
			strncpy(numeric_value, extract_string_ini, characters_count);
			*(numeric_value + characters_count) = '\0';
			index_t = atoi(numeric_value) - 1;

			active_object_compound->uv_coordinates.push_back(unique_uv_coordinates.at(index_t));

			/* Read third vertex normal vector */
			extract_string_ini = extract_string_end + 1;
			characters_count = new_line_position - extract_string_ini;
			strncpy(numeric_value, extract_string_ini, characters_count);
			*(numeric_value + characters_count) = '\0';
			index_n = atoi(numeric_value) - 1;

			active_object_compound->normal_vectors.push_back(unique_normal_vectors.at(index_n));

			active_object_compound->triangles_count = active_object_compound->triangles_count + 1;

			break;

		default:
			fprintf(stderr, "<<Error: Unknown identifier \"%s\" found while reading \"%s\">>\n", read_line, src_obj_filename);
			break;
		}
	}

	fclose(fp_obj);

	/* Read the material file */
	if (strlen(material_filename) > 0)
	{
		readMtlFile(material_filename);
	}

	// Center all the objects compounds:
	centerObjectCompound();

	return 1;
}

int GEOMETRIES_MANAGER::readMtlFile(const char * src_mtl_filename)
{
	FILE * fp_material = fopen(src_mtl_filename, "r");

	if (!fp_material)
	{
		fprintf(stderr, "<<Error: Mtl file: \"%s\", could not be opened>>\n", src_mtl_filename);
		return 0;
	}
	
	char read_line[512];
	char material_name[32];
	IMG_DATA * new_texture_map;
	char texture_filename[512];

	char * extract_string_ini;
	char * extract_string_end;
	char * new_line_position;
	unsigned int characters_count;
	
	while (1)
	{
		fgets(read_line, 512, fp_material);
		if (feof(fp_material))
		{
			break;
		}

		new_line_position = strchr(read_line, '\n');
		const unsigned int characters_in_line = new_line_position - read_line;
		*(read_line + characters_in_line) = '\0';

		switch ((int)*read_line)
		{
		case (int)'#':
			// Ignore  the commentaries ...
			break;

		case (int)'n':
			// New material found: Copy the material identifier
			strcpy(material_name, read_line + 7);
			break;

		case (int)'N':
		case (int)'K':
		case (int)'d':
		case (int)'i':
		case (int)'\0':
			// Ignore the material characteristics
			break;

		case (int)'m':
			// Copy the texture filename:
			strcpy(texture_filename, read_line + 7);			
			compounds_textures_manager.reassignTexture(material_name, texture_filename);
			break;

		default:
			fprintf(stderr, "<<Error: Unknwon .mtl key \"%s\" found in \"%s\">>\n", read_line, src_mtl_filename);
			break;
		}
	}

	fclose(fp_material);
	
	return 1;
}



void GEOMETRIES_MANAGER::centerObjectCompound()
{
	std::vector<compound_geometry*>::iterator objects_compounds_ini = objects_compounds.begin();
	std::vector<compound_geometry*>::iterator objects_compounds_end = objects_compounds.end();
	std::vector<compound_geometry*>::iterator objects_compounds_it;

	std::vector<glm::vec4>::iterator vertices_positions_ini;
	std::vector<glm::vec4>::iterator vertices_positions_end;
	std::vector<glm::vec4>::iterator vertices_positions_it;

	glm::vec4 center_xy;

	for (objects_compounds_it = objects_compounds_ini; objects_compounds_it != objects_compounds_end; objects_compounds_it++)
	{
		vertices_positions_ini = (*objects_compounds_it)->vertices_positions.begin();
		vertices_positions_end = (*objects_compounds_it)->vertices_positions.end();
		center_xy = glm::vec4(0.0f);

		for (vertices_positions_it = vertices_positions_ini; vertices_positions_it != vertices_positions_end; vertices_positions_it++)
		{
			center_xy = center_xy + *vertices_positions_it;
		}

		center_xy = center_xy / (float)(*objects_compounds_it)->vertices_positions.size();
		center_xy.z = 0.0f;
		center_xy.w = 0.0f;

		for (vertices_positions_it = vertices_positions_ini; vertices_positions_it != vertices_positions_end; vertices_positions_it++)
		{
			*vertices_positions_it = *vertices_positions_it - center_xy;
		}
	}
}

