#ifndef NODE_IMAGE_FG_CLASS_H_INCLUDED
#define NODE_IMAGE_FG_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "figure_2d_class.h"

class NODE_IMAGE_FG :
	public FIGURE_2D
{
public:
	NODE_IMAGE_FG();
	NODE_IMAGE_FG(const NODE_IMAGE_FG& src_node_figure);

	NODE_IMAGE_FG operator=(const NODE_IMAGE_FG& src_node_figure);
	~NODE_IMAGE_FG();

private:

	void initializeVerticesPositions();
	void initializeColorValues();
	void initializeUVValues();
	void initializeNormalVectors();
};

#endif // NODE_IMAGE_FG_CLASS_H_INCLUDED
