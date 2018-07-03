#ifndef TEXT_IMAGE_OPERATION_CLASS_H_INCLUDED
#define TEXT_IMAGE_OPERATION_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_operation_class.h"
#include "load_image_operation_class.h"
#include "save_image_scalar_operation_class.h"

class TEXT_IMAGE_OPERATION :
	public IMAGE_OPERATION
{
public:
	TEXT_IMAGE_OPERATION();
	TEXT_IMAGE_OPERATION(const TEXT_IMAGE_OPERATION & src_text_image_operation);
	TEXT_IMAGE_OPERATION operator = (const TEXT_IMAGE_OPERATION & src_text_image_operation);
	~TEXT_IMAGE_OPERATION();

protected:
	void performOperation();

private:
	IMG_DATA ** character_set;
	LOAD_IMAGE_OPERATION character_loader;
	bool characters_set_was_loaded;
	unsigned int max_height;
	ROI_BBOX * character_dimensions;

	void loadCharacters();
	void addCharacter(const unsigned int src_position_x, const unsigned int src_position_y, const unsigned int src_character_code);
	void computeCanvas();
};

#endif // TEXT_IMAGE_OPERATION_CLASS_H_INCLUDED
