#ifndef TYPOGRAPHY_CLASS_H_INCLUDED
#define TYPOGRAPHY_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "figure_2d_class.h"

class TYPOGRAPHY_CLASS :
	public FIGURE_2D
{
public:
	TYPOGRAPHY_CLASS();
	TYPOGRAPHY_CLASS(const TYPOGRAPHY_CLASS& src_node_figure);

	TYPOGRAPHY_CLASS operator=(const TYPOGRAPHY_CLASS& src_node_figure);
	~TYPOGRAPHY_CLASS();
	
	void setText(const char * src_text);
	void setCharactersMapFilename(const char * src_characters_boundaries_map_filename);

	void setHorizontalSpacing(const float src_horizontal_spacing);
	void setVerticalSpacing(const float src_vertical_spacing);
	void setBoundingBox(const float src_max_width, const float src_max_height);

	void setFontColor(const unsigned char src_r_channel, const unsigned char src_g_channel, const unsigned char src_b_channel);
	void setBackgroundColor(const unsigned char src_r_channel, const unsigned char src_g_channel, const unsigned char src_b_channel);
private:

	IMG_DATA * characters_map;
	char * text_box;

	unsigned int character_max_width;
	unsigned int character_max_height;

	bool character_map_was_loaded;
	char * characters_boundaries_map_filename;
	GLfloat * uv_character_values;

	GLfloat * character_offset_y;
	GLfloat * character_boundaries_L;
	GLfloat * character_boundaries_R;
	GLfloat * character_boundaries_T;
	GLfloat * character_boundaries_B;

	GLfloat horizontal_spacing;
	GLfloat vertical_spacing;
	GLfloat max_text_width;
	GLfloat max_text_height;
	
	unsigned char r_channel_bg;
	unsigned char g_channel_bg;
	unsigned char b_channel_bg;

	unsigned char r_channel_font;
	unsigned char g_channel_font;
	unsigned char b_channel_font;

	void initializeVerticesPositions();
	void initializeColorValues();
	void initializeUVValues();
	void initializeNormalVectors();

	void loadCharacterMap();
};

#endif // TYPOGRAPHY_CLASS_H_INCLUDED
