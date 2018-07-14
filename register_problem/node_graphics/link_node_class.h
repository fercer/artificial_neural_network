#ifndef LINK_NODE_CLASS_H_INCLUDED
#define LINK_NODE_CLASS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "figure_2d_class.h"

class LINK_NODE:
	public FIGURE_2D
{
public:
	LINK_NODE();
	LINK_NODE(const LINK_NODE& src_node_figure);

	LINK_NODE operator=(const LINK_NODE& src_node_figure);
	~LINK_NODE();

private:

	void initializeVerticesPositions();
	void initializeColorValues();
	void initializeUVValues();
	void initializeNormalVectors();
};

#endif // LINK_NODE_CLASS_H_INCLUDED
