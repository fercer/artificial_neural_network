#ifndef NODE_IMAGE_BG_CLASS_H_INCLUDED
#define NODE_IMAGE_BG_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "figure_2d_class.h"

class NODE_IMAGE_BG :
	public FIGURE_2D
{
public:
	NODE_IMAGE_BG();
	NODE_IMAGE_BG(const NODE_IMAGE_BG& src_node_figure);

	NODE_IMAGE_BG operator=(const NODE_IMAGE_BG& src_node_figure);
	~NODE_IMAGE_BG();

private:

	void initializeVerticesPositions();
	void initializeColorValues();
	void initializeUVValues();
	void initializeNormalVectors();
};

#endif // NODE_IMAGE_BG_CLASS_H_INCLUDED
