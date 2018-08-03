#include "textures_manager_class.h"

TEXTURES_MANAGER::TEXTURES_MANAGER()
{

}



TEXTURES_MANAGER::TEXTURES_MANAGER(const TEXTURES_MANAGER & src_node_figure)
{

}



TEXTURES_MANAGER::~TEXTURES_MANAGER()
{

}



bool TEXTURES_MANAGER::newTexture(const char * src_material_identifier, const char * src_texture_filename)
{
	std::vector<texture_map_pair>::iterator texture_pairs_ini = texture_pairs.begin();
	std::vector<texture_map_pair>::iterator texture_pairs_end = texture_pairs.end();
	
	texture_map_pair looking_for_material(src_material_identifier, NULL, false);
	std::vector<texture_map_pair>::iterator texture_pairs_it = std::find(texture_pairs_ini, texture_pairs_end, looking_for_material);
	
	if (texture_pairs_it == texture_pairs_end)
	{
		return false;
	}

	newTexture_from_file(src_material_identifier, src_texture_filename);

	return true;
}



bool TEXTURES_MANAGER::newTexture(const char * src_material_identifier, IMG_DATA * src_texture)
{
	std::vector<texture_map_pair>::iterator texture_pairs_ini = texture_pairs.begin();
	std::vector<texture_map_pair>::iterator texture_pairs_end = texture_pairs.end();

	texture_map_pair looking_for_material(src_material_identifier, NULL, false);
	std::vector<texture_map_pair>::iterator texture_pairs_it = std::find(texture_pairs_ini, texture_pairs_end, looking_for_material);

	if (texture_pairs_it == texture_pairs_end)
	{
		return false;
	}

	newTexture_from_existing(src_material_identifier, src_texture);

	return true;
}

bool TEXTURES_MANAGER::reassignTexture(const char * src_material_identifier, const char * src_texture_filename)
{
	std::vector<texture_map_pair>::iterator texture_pairs_ini = texture_pairs.begin();
	std::vector<texture_map_pair>::iterator texture_pairs_end = texture_pairs.end();

	texture_map_pair looking_for_material(src_material_identifier, NULL, false);
	std::vector<texture_map_pair>::iterator texture_pairs_it = std::find(texture_pairs_ini, texture_pairs_end, looking_for_material);

	if (texture_pairs_it == texture_pairs_end)
	{
		return false;
	}

	if (texture_pairs_it->self_allocated_texture_map)
	{
		freeImageData(texture_pairs_it->texture_map);
		texture_pairs_it->self_allocated_texture_map = false;
	}

	FILE * fp_texture = fopen(src_texture_filename, "r");

	if (!fp_texture)
	{
		fprintf(stderr, "<<Error: The texture map file \"%s\" could not be opened>>\n", src_texture_filename);
		return;
	}

	char magic_number[3];
	fgets(magic_number, 3, fp_texture);
	fclose(fp_texture);

	if (*magic_number != 'P')
	{
		fprintf(stderr, "<<Error: The texture file \'%s\' is not in PGM or PPM format>>\n", src_texture_filename);
		return;
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

	texture_pairs_it->texture_map = new_texture_map;
	texture_pairs_it->self_allocated_texture_map = true;

	return true;
}


bool TEXTURES_MANAGER::reassignTexture(const char * src_material_identifier, IMG_DATA * src_texture)
{
	std::vector<texture_map_pair>::iterator texture_pairs_ini = texture_pairs.begin();
	std::vector<texture_map_pair>::iterator texture_pairs_end = texture_pairs.end();

	texture_map_pair looking_for_material(src_material_identifier, NULL, false);
	std::vector<texture_map_pair>::iterator texture_pairs_it = std::find(texture_pairs_ini, texture_pairs_end, looking_for_material);

	if (texture_pairs_it == texture_pairs_end)
	{
		return false;
	}

	if (texture_pairs_it->self_allocated_texture_map)
	{
		freeImageData(texture_pairs_it->texture_map);
		texture_pairs_it->self_allocated_texture_map = false;
	}

	texture_pairs_it->texture_map = src_texture;

	return true;
}

IMG_DATA * TEXTURES_MANAGER::getTexture(const char * src_material_identifier)
{
	std::vector<texture_map_pair>::iterator texture_pairs_ini = texture_pairs.begin();
	std::vector<texture_map_pair>::iterator texture_pairs_end = texture_pairs.end();

	texture_map_pair looking_for_material(src_material_identifier, NULL, false);
	std::vector<texture_map_pair>::iterator texture_pairs_it = std::find(texture_pairs_ini, texture_pairs_end, looking_for_material);

	if (texture_pairs_it == texture_pairs_end)
	{
		return NULL;
	}

	return texture_pairs_it->texture_map;
}



bool compare_texture_map_pair(texture_map_pair texture_a, texture_map_pair texture_b)
{
	return texture_a == texture_b;
}



void TEXTURES_MANAGER::newTexture_from_file(const char * src_material_identifier, const char * src_texture_filename)
{
	FILE * fp_texture = fopen(src_texture_filename, "r");

	if (!fp_texture)
	{
		fprintf(stderr, "<<Error: The texture map file \"%s\" could not be opened>>\n", src_texture_filename);
		return;
	}

	char magic_number[3];
	fgets(magic_number, 3, fp_texture);
	fclose(fp_texture);

	if (*magic_number != 'P')
	{
		fprintf(stderr, "<<Error: The texture file \'%s\' is not in PGM or PPM format>>\n", src_texture_filename);
		return;
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
	
	texture_pairs.push_back(texture_map_pair(src_material_identifier, new_texture_map, true));
}





IMG_DATA * TEXTURES_MANAGER::loadPGM_ascii(const char * src_filename)
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



IMG_DATA * TEXTURES_MANAGER::loadPGM_raw(const char * src_filename)
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


IMG_DATA * TEXTURES_MANAGER::loadPPM_ascii(const char * src_filename)
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



IMG_DATA * TEXTURES_MANAGER::loadPPM_raw(const char * src_filename)
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



void TEXTURES_MANAGER::newTexture_from_existing(const char * src_material_identifier, IMG_DATA * src_texture)
{
	texture_pairs.push_back(texture_map_pair(src_material_identifier, src_texture, false));
}