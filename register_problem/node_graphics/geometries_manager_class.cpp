#include "geometries_manager_class.h"

GEOMETRIES_MANAGER::GEOMETRIES_MANAGER()
{
}

GEOMETRIES_MANAGER::GEOMETRIES_MANAGER(const GEOMETRIES_MANAGER & src_node_figure)
{
}

GEOMETRIES_MANAGER::~GEOMETRIES_MANAGER()
{
}

void GEOMETRIES_MANAGER::loadObj(const char * src_obj_filename)
{

}

int GEOMETRIES_MANAGER::readObjFile(const char * src_obj_filename)
{
	FILE * fp_obj = fopen(src_obj_filename, "r");
	if (!fp_obj)
	{
		fprintf(stderr, "<<Error: Obj file: \"%s\", could not be opened>>\n", src_obj_filename);
		return -1;
	}

	char read_line[512];
	char material_filename[64];
	std::vector<glm::vec4> unique_position_vertices;
	std::vector<glm::vec2> unique_uv_coordinates;
	std::vector<glm::vec3> unique_normal_vectors;

	object_geometry_type active_object = OBJGEOM_UNASSIGNED;
	node_geometry * active_object_compound = NULL;

	GLfloat p_1, p_2, p_3;

	// Read the lines of the .obj file:
	while (1)
	{
		fgets(read_line, 512, fp_obj);

		if (feof(fp_obj))
		{
			break;
		}

		char * new_line_position = strchr(read_line, '\n');
		const unsigned int characters_in_line = new_line_position - read_line;
		*(read_line + characters_in_line) = '\0';

		char * extract_string_ini;
		char * extract_string_end;
		unsigned int characters_count;
		switch ((int)*read_line)
		{
			/* ------------------------------ Commentary ------------------------------*/
		case (int)'#':
			// Ignore the commentaries ...
			break;

			/*------------------------------ Material filename------------------------------ */
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
			char object_name[9];
			object_name[8] = '\0';
			strncpy(object_name, read_line + 2, 8);

			unsigned int object_code = *object_name * *object_name;
			object_code += *(object_name + 1) * *(object_name + 1);
			object_code += *(object_name + 2) * *(object_name + 2);

			unsigned int object_index = atoi(object_name + 4);
			char numeric_value[64];

			if (object_code == 27451) /* Icon: */
			{
				switch (object_code)
				{
				case 1:
					active_object = OBJGEOM_LINK_ICON;
					break;

				case 2:
					active_object = OBJGEOM_IMAGE_LINK_ICON;
					break;

				default:
					fprintf(stderr, "<<Error: Icon \"%i\" not identified in object \"%s\", while reading \"%s\">>\n", object_index, object_name, src_obj_filename);
					continue;
					break;
				}
			}
			else if (object_code == 27081) /* Node object */
			{
				switch (object_code)
				{
				case 1:
					active_object = OBJGEOM_NUMERIC_NODE;
					break;

				case 2:
					active_object = OBJGEOM_STRING_NODE;
					break;

				case 3:
					active_object = OBJGEOM_NODE_OPERATION;
					break;

				case 4:
					active_object = OBJGEOM_IMAGE_SCALAR_OPERATION;
					break;

				case 5:
					active_object = OBJGEOM_IMAGE_OPERATION;
					break;

				default:
					fprintf(stderr, "<<Error: Node object \"%i\" not identified in object \"%s\", while reading \"%s\">>\n", object_index, object_name, src_obj_filename);
					continue;
					break;
				}
			}
			else
			{
				fprintf(stderr, "<<Error: Object \"%s\" not identified while reading \"%s\">>\n", object_name, src_obj_filename);
				continue;
			}

			active_object_compound = new node_geometry;
			active_object_compound->node_geometry_type = active_object;
			obejcts_compounds.push_back(active_object_compound);
			break;

			/* ------------------------------ Vertex identifier (vertex position, texture and normals) ------------------------------ */
		case (int)'v':
			switch ((int)*(read_line + 1))
			{
			case (int)' ': /* Position vertex */
				extract_string_ini = read_line + 2;
				extract_string_end = strchr(extract_string_ini, ' ');
				characters_count = extract_string_end - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				p_1 = (float)atof(numeric_value);

				extract_string_ini = extract_string_end + 1;
				extract_string_end = strchr(extract_string_ini, ' ');
				characters_count = extract_string_end  - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				p_2 = (float)atof(numeric_value);

				extract_string_ini = extract_string_end + 1;
				characters_count = new_line_position - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				p_3 = (float)atof(numeric_value);

				unique_position_vertices.push_back(glm::vec4(p_1, p_2, p_3, 1.0));

				break;

			case (int)'t': /* Texture uv coordinate */
				extract_string_ini = read_line + 3;
				extract_string_end = strchr(extract_string_ini, ' ');
				characters_count = extract_string_end - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				p_1 = (float)atof(numeric_value);

				extract_string_ini = extract_string_end + 1;
				characters_count = new_line_position - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				p_2 = (float)atof(numeric_value);

				unique_uv_coordinates.push_back(glm::vec2(p_1, p_2));

				break;

			case (int)'n': /* Normal vector */
				extract_string_ini = read_line + 3;
				extract_string_end = strchr(extract_string_ini, ' ');
				characters_count = extract_string_end - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				p_1 = (float)atof(numeric_value);

				extract_string_ini = extract_string_end + 1;
				extract_string_end = strchr(extract_string_ini, ' ');
				characters_count = extract_string_end - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				p_2 = (float)atof(numeric_value);

				extract_string_ini = extract_string_end + 1;
				characters_count = new_line_position - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				p_3 = (float)atof(numeric_value);

				unique_normal_vectors.push_back(glm::vec3(p_1, p_2, p_3));
				break;

			default:
				fprintf(stderr, "<<Error: Unknown vertex identifier \"%s\" found while reading \"%s\">>\n", read_line, src_obj_filename);
				continue;
				break;
			}


			break;

			/* ------------------------------ Use material (Material used for this object) ------------------------------ */
		case (int)'u':
			extract_string_ini = strchr(read_line, ' ');
			strcpy(active_object_compound->material_identifier, extract_string_ini);
			break;

			/* ------------------------------ Smoothing (Off in general) ------------------------------ */
		case (int)'s':
			// Ignore the smoothing index ...
			break;

			/* ------------------------------ Face, conformed of three vertices identifiers ------------------------------ */
		case (int)'f':
			extract_string_ini = 
			break;

		default:
			fprintf(stderr, "<<Error: Unknown identifier \"%s\" found while reading \"%s\">>\n", read_line, src_obj_filename);
			continue;
			break;
		}
	}

	fclose(fp_obj);

	return 0;
}

int GEOMETRIES_MANAGER::readMtlFile(const char * src_mtl_filename)
{
	return 0;
}

int GEOMETRIES_MANAGER::readTextureFile(const char * src_texture_filename)
{
	return 0;
}
