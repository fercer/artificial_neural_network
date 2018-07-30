#include "geometries_manager_class.h"

GEOMETRIES_MANAGER::GEOMETRIES_MANAGER()
{
	obj_read_status = 0;
	mtl_read_status = 0;
	texture_read_status = 0;
	texture_map = NULL;
}



GEOMETRIES_MANAGER::GEOMETRIES_MANAGER(const GEOMETRIES_MANAGER & src_node_figure)
{
	obj_read_status = 0;
	mtl_read_status = 0;
	texture_read_status = 0;
	texture_map = NULL;
}



GEOMETRIES_MANAGER::~GEOMETRIES_MANAGER()
{
	std::vector<node_geometry*>::iterator objects_compounds_ini = objects_compounds.begin();
	std::vector<node_geometry*>::iterator objects_compounds_end = objects_compounds.end();
	std::vector<node_geometry*>::iterator objects_compounds_it;

	for (objects_compounds_it = objects_compounds_ini; objects_compounds_it != objects_compounds_end; objects_compounds_it++)
	{
		delete *objects_compounds_it;
	}

	if (texture_map)
	{
		freeImageData(texture_map);
	}
}



void GEOMETRIES_MANAGER::loadObj(const char * src_obj_filename)
{
	obj_read_status = readObjFile(src_obj_filename);
}


GEOMETRIES_MANAGER::node_geometry * GEOMETRIES_MANAGER::getGeometry(const object_geometry_type src_object_type)
{
	std::vector<node_geometry*>::iterator objects_compounds_ini = objects_compounds.begin();
	std::vector<node_geometry*>::iterator objects_compounds_end = objects_compounds.end();
	std::vector<node_geometry*>::iterator objects_compounds_it = objects_compounds_ini;

	do
	{
		if ((*objects_compounds_it)->node_geometry_type == src_object_type)
		{
			break;
		}
	} while (++objects_compounds_it != objects_compounds_end);
	
	if (objects_compounds_it == objects_compounds_end)
	{
		objects_compounds_it = objects_compounds_ini;
	}

	return *objects_compounds_it;
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

	object_geometry_type active_object = OBJGEOM_UNASSIGNED;
	node_geometry * active_object_compound = NULL;

	char * extract_string_ini;
	char * extract_string_end;
	char * new_line_position;
	unsigned int characters_count;

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
			object_name[8] = '\0';
			strncpy(object_name, read_line + 2, 8);

			object_code = *object_name * *object_name;
			object_code += *(object_name + 1) * *(object_name + 1);
			object_code += *(object_name + 2) * *(object_name + 2);

			object_index = atoi(object_name + 4);

			if (object_code == 27451) /* Icon: */
			{
				switch (object_index)
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
				switch (object_index)
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
			objects_compounds.push_back(active_object_compound);
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
				*(numeric_value + characters_count) = '\0';
				p_1 = (float)atof(numeric_value);

				extract_string_ini = extract_string_end + 1;
				extract_string_end = strchr(extract_string_ini, ' ');
				characters_count = extract_string_end  - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				*(numeric_value + characters_count) = '\0';
				p_3 = (float)atof(numeric_value);

				extract_string_ini = extract_string_end + 1;
				characters_count = new_line_position - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				*(numeric_value + characters_count) = '\0';
				p_2 = (float)atof(numeric_value);

				unique_position_vertices.push_back(glm::vec4(p_1, p_2, p_3, 1.0));

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
				p_3 = (float)atof(numeric_value);

				extract_string_ini = extract_string_end + 1;
				characters_count = new_line_position - extract_string_ini;
				strncpy(numeric_value, extract_string_ini, characters_count);
				*(numeric_value + characters_count) = '\0';
				p_2 = (float)atof(numeric_value);

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
			extract_string_ini = strchr(read_line, ' ') + 1;
			strcpy(active_object_compound->material_identifier, extract_string_ini);
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
			continue;
			break;
		}
	}

	fclose(fp_obj);

	/* Read the material file */
	if (strlen(material_filename) > 0)
	{
		mtl_read_status = readMtlFile(material_filename);
	}

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

			// Open the texture image:
			if (texture_map)
			{
				freeImageData(texture_map);
			}
			texture_map = readTextureFile(texture_filename);

			texture_read_status = texture_map ? 1 : 0;

			break;

		default:
			fprintf(stderr, "<<Error: Unknwon .mtl key \"%s\" found in \"%s\">>\n", read_line, src_mtl_filename);
			break;
		}
	}

	fclose(fp_material);
	
	return 1;
}


IMG_DATA * GEOMETRIES_MANAGER::readTextureFile(const char * src_texture_filename)
{
	FILE * fp_texture = fopen(src_texture_filename, "r");

	if (!fp_texture)
	{
		fprintf(stderr, "<<Error: The texture map file \"%s\" could not be opened>>\n", src_texture_filename);
		return NULL;
	}
	
	char magic_number[3];
	fgets(magic_number, 3, fp_texture);
	fclose(fp_texture);

	if (*magic_number != 'P')
	{
		fprintf(stderr, "<<Error: The texture file \'%s\' is not in PGM or PPM format>>\n", src_texture_filename);
		return NULL;
	}

	IMG_DATA *new_texture_map = NULL;
	
	switch ((int)*(magic_number + 1))
	{
	case (int)'2':
		new_texture_map = loadPGM_ascii(src_texture_filename);
		break;

	case (int)'5':
		new_texture_map = loadPGM_raw(src_texture_filename);
		break;

	case (int)'3':
		new_texture_map = loadPPM_ascii(src_texture_filename);
		break;

	case (int)'6':
		new_texture_map = loadPPM_raw(src_texture_filename);
		break;

	default:
		fprintf(stderr, "<<Error: The texture file \'%s\' is an unkoun PGM or PPM format>>\n", src_texture_filename);
		break;
	}

	return new_texture_map;
}





IMG_DATA * GEOMETRIES_MANAGER::loadPGM_ascii(const char * src_filename)
{
	const unsigned int color_channels = 1;

	FILE * fp_texture = fopen(src_filename, "r");

	char magic_number[3];

	// Read magic number:
	fgets(magic_number, 3, fp_texture);

	// Read the new line character after the magic number:
	char temporal_characters = fgetc(fp_texture);

	// Verify if the file contains a commentary:
	fpos_t image_body_file_position;
	fgetpos(fp_texture, &image_body_file_position);

	temporal_characters = fgetc(fp_texture);
	if (temporal_characters == '#')
	{
		// The file contains a commentary:
		while (temporal_characters != '\n')
		{
			temporal_characters = fgetc(fp_texture);
		}
	}
	else
	{
		fsetpos(fp_texture, &image_body_file_position);
	}

	// Read the width, height and maximum intensity of the image:
	int width, height;
	fscanf(fp_texture, "%i %i\n", &width, &height);

	double max_intensity;
	fscanf(fp_texture, "%lf\n", &max_intensity);

	IMG_DATA * new_texture_data = createVoidImage(width, height, IMG_UCHAR, color_channels);

	unsigned char * pix_intensity = new_texture_data->image_data.unsigned_character_image_data;
	for (int pix_position = 0; pix_position < width*height*color_channels; pix_position++)
	{
		fscanf(fp_texture, "%c\n", pix_intensity++);
	}

	fclose(fp_texture);

	return new_texture_data;
}



IMG_DATA * GEOMETRIES_MANAGER::loadPGM_raw(const char * src_filename)
{

	const unsigned int color_channels = 1;

	FILE * fp_texture = fopen(src_filename, "rb");

	char temporal_characters;
	char magic_number;

	// Read magic number:
	fread(&temporal_characters, sizeof(char), 1, fp_texture);
	fread(&magic_number, sizeof(char), 1, fp_texture);

	// Read the new line character:
	fread(&temporal_characters, sizeof(char), 1, fp_texture);

	// Verify if the file contains a commentary:
	fpos_t image_body_file_position;
	fgetpos(fp_texture, &image_body_file_position);

	fread(&temporal_characters, sizeof(char), 1, fp_texture);
	if (temporal_characters == '#')
	{
		// The file contains a commentary:
		while (temporal_characters != '\n')
		{
			fread(&temporal_characters, sizeof(char), 1, fp_texture);
		}
	}
	else
	{
		fsetpos(fp_texture, &image_body_file_position);
	}

	// Read the width, height and maximum intensity of the image:
	char width_string[] = "\0\0\0\0\0\0\0";
	unsigned int string_position = 0;
	do
	{
		fread(width_string + string_position, sizeof(char), 1, fp_texture);
	} while (*(width_string + string_position++) != ' ');
	int width = atoi(width_string);

	char height_string[] = "\0\0\0\0\0\0\0";
	string_position = 0;
	do {
		fread(height_string + string_position, sizeof(char), 1, fp_texture);
	} while (*(height_string + string_position++) != '\n');
	int height = atoi(height_string);


	char max_intensity_string[] = "\0\0\0\0\0\0\0";
	string_position = 0;
	do {
		fread(max_intensity_string + string_position, sizeof(char), 1, fp_texture);
	} while (*(max_intensity_string + string_position++) != '\n');
	double max_intensity = atof(max_intensity_string);

	IMG_DATA * new_texture_data = createVoidImage(width, height, IMG_UCHAR, color_channels);
	fread(new_texture_data->image_data.unsigned_character_image_data, sizeof(unsigned char), width * height * color_channels, fp_texture);
	fclose(fp_texture);
	
	return new_texture_data;
}


IMG_DATA * GEOMETRIES_MANAGER::loadPPM_ascii(const char * src_filename)
{

	const unsigned int color_channels = 3;

	FILE * fp_texture = fopen(src_filename, "r");

	char magic_number[3];

	// Read magic number:
	fgets(magic_number, 3, fp_texture);

	// Read the new line character after the magic number:
	char temporal_characters = fgetc(fp_texture);

	// Verify if the file contains a commentary:
	fpos_t image_body_file_position;
	fgetpos(fp_texture, &image_body_file_position);

	temporal_characters = fgetc(fp_texture);
	if (temporal_characters == '#')
	{
		// The file contains a commentary:
		while (temporal_characters != '\n')
		{
			temporal_characters = fgetc(fp_texture);
		}
	}
	else
	{
		fsetpos(fp_texture, &image_body_file_position);
	}

	// Read the width, height and maximum intensity of the image:
	int width, height;
	fscanf(fp_texture, "%i %i\n", &width, &height);

	double max_intensity;
	fscanf(fp_texture, "%lf\n", &max_intensity);

	IMG_DATA * new_texture_data = createVoidImage(width, height, IMG_UCHAR, color_channels);

	unsigned char * pix_intensity = new_texture_data->image_data.unsigned_character_image_data;
	for (int pix_position = 0; pix_position < width*height*color_channels; pix_position++)
	{
		fscanf(fp_texture, "%c\n", pix_intensity++);
	}

	fclose(fp_texture);

	return new_texture_data;
}



IMG_DATA * GEOMETRIES_MANAGER::loadPPM_raw(const char * src_filename)
{

	const unsigned int color_channels = 3;

	FILE * fp_texture = fopen(src_filename, "rb");

	char temporal_characters;
	char magic_number;

	// Read magic number:
	fread(&temporal_characters, sizeof(char), 1, fp_texture);
	fread(&magic_number, sizeof(char), 1, fp_texture);

	// Read the new line character:
	fread(&temporal_characters, sizeof(char), 1, fp_texture);

	// Verify if the file contains a commentary:
	fpos_t image_body_file_position;
	fgetpos(fp_texture, &image_body_file_position);

	fread(&temporal_characters, sizeof(char), 1, fp_texture);
	if (temporal_characters == '#')
	{
		// The file contains a commentary:
		while (temporal_characters != '\n')
		{
			fread(&temporal_characters, sizeof(char), 1, fp_texture);
		}
	}
	else
	{
		fsetpos(fp_texture, &image_body_file_position);
	}

	// Read the width, height and maximum intensity of the image:
	char width_string[] = "\0\0\0\0\0\0\0";
	unsigned int string_position = 0;
	do
	{
		fread(width_string + string_position, sizeof(char), 1, fp_texture);
	} while (*(width_string + string_position++) != ' ');
	int width = atoi(width_string);

	char height_string[] = "\0\0\0\0\0\0\0";
	string_position = 0;
	do {
		fread(height_string + string_position, sizeof(char), 1, fp_texture);
	} while (*(height_string + string_position++) != '\n');
	int height = atoi(height_string);

	char max_intensity_string[] = "\0\0\0\0\0\0\0";
	string_position = 0;
	do {
		fread(max_intensity_string + string_position, sizeof(char), 1, fp_texture);
	} while (*(max_intensity_string + string_position++) != '\n');
	double max_intensity = atof(max_intensity_string);

	IMG_DATA * new_texture_data = createVoidImage(width, height, IMG_UCHAR, color_channels);
	fread(new_texture_data->image_data.unsigned_character_image_data, sizeof(unsigned char), width * height, fp_texture);
	fclose(fp_texture);

	return new_texture_data;
}
