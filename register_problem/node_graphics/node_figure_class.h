#ifndef NODE_FIGURE_CLASS_H_INCLUDED
#define NODE_FIGURE_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "figure_2d_class.h"

class NODE_FIGURE :
	public FIGURE_2D
{
public:
	NODE_FIGURE();
	NODE_FIGURE(const NODE_FIGURE& src_node_figure);

	NODE_FIGURE operator=(const NODE_FIGURE& src_node_figure);
	~NODE_FIGURE();

private:

	void initializeVerticesPositions();
	void initializeColorValues();
	void initializeUVValues();
	void initializeNormalVectors();
};

#endif // NODE_FIGURE_CLASS_H_INCLUDED
