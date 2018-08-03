#ifndef TEXTURES_MANAGER_CLASS_H_INCLUDED
#define TEXTURES_MANAGER_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>
#include <iterator>

#include <string.h>
#include "../image_functions.h"

typedef struct texture_map_pair
{
	char * material_identifier;
	IMG_DATA * texture_map;
	bool self_allocated_texture_map;

	texture_map_pair()
	{
		material_identifier = NULL;
		texture_map = NULL;
		self_allocated_texture_map = false;
	}

	texture_map_pair(const char * src_material_identifier, IMG_DATA * src_texture_map, bool is_self_allocated = false)
	{
		unsigned int material_identifier_length = (unsigned int)strlen(src_material_identifier);
		material_identifier = new char[material_identifier_length + 1];
		strcpy(material_identifier, src_material_identifier);

		texture_map = src_texture_map;
		self_allocated_texture_map = is_self_allocated;
	}

	texture_map_pair(const texture_map_pair& src_texture_map_pair)
	{
		unsigned int material_identifier_length = (unsigned int)strlen(src_texture_map_pair.material_identifier);
		material_identifier = new char[material_identifier_length + 1];
		strcpy(material_identifier, src_texture_map_pair.material_identifier);
		texture_map = src_texture_map_pair.texture_map;
		self_allocated_texture_map = false;
	}

	texture_map_pair(const texture_map_pair * src_texture_map_pair)
	{
		unsigned int material_identifier_length = (unsigned int)strlen(src_texture_map_pair->material_identifier);
		material_identifier = new char[material_identifier_length + 1];
		strcpy(material_identifier, src_texture_map_pair->material_identifier);
		texture_map = src_texture_map_pair->texture_map;
		self_allocated_texture_map = false;
	}

	texture_map_pair & operator=(const texture_map_pair & src_texture_map_pair)
	{
		if (this != &src_texture_map_pair)
		{
			if (this->material_identifier)
			{
				delete[] this->material_identifier;
			}

			unsigned int material_identifier_length = (unsigned int)strlen(src_texture_map_pair.material_identifier);
			material_identifier = new char[material_identifier_length + 1];
			strcpy(material_identifier, src_texture_map_pair.material_identifier);

			this->texture_map = src_texture_map_pair.texture_map;
			this->self_allocated_texture_map = false;
		}

		return *this;
	}

	~texture_map_pair()
	{
		if (self_allocated_texture_map)
		{
			freeImageData(texture_map);
		}

		if (this->material_identifier)
		{
			delete[] this->material_identifier;
		}
	}

	inline bool operator==(const texture_map_pair& right_texture_pair)
	{
		if (strcmp(material_identifier, right_texture_pair.material_identifier) == 0)
		{
			return true;
		}

		return false;
	}
} texture_map_pair;


class TEXTURES_MANAGER
{
public:
	TEXTURES_MANAGER();
	TEXTURES_MANAGER(const TEXTURES_MANAGER& src_node_figure);
	~TEXTURES_MANAGER();
	
	bool newTexture(const char * src_material_identifier, const char * src_texture_filename);
	bool newTexture(const char * src_material_identifier, IMG_DATA * src_texture);
	bool reassignTexture(const char * src_material_identifier, const char * src_texture_filename);
	bool reassignTexture(const char * src_material_identifier, IMG_DATA * src_texture);

	IMG_DATA * getTexture(const char * src_material_identifier);

private:
	void newTexture_from_file(const char * src_material_identifier, const char * src_texture_filename);
	void newTexture_from_existing(const char * src_material_identifier, IMG_DATA * src_texture);

	std::vector<texture_map_pair> texture_pairs;

	IMG_DATA * loadPGM_ascii(const char * src_filename);
	IMG_DATA * loadPGM_raw(const char * src_filename);
	IMG_DATA * loadPPM_ascii(const char * src_filename);
	IMG_DATA * loadPPM_raw(const char * src_filename);
};



bool compare_texture_map_pair(texture_map_pair texture_a, texture_map_pair texture_b);

#endif // TEXTURES_MANAGER_CLASS_H_INCLUDED
