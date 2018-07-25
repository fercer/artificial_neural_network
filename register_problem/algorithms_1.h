#ifndef ALGORITHMS_1_INCLUDED
#define ALGORITHMS_1_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <opencv2\opencv.hpp>

typedef struct UNION_FIND {
	//UNION_FIND * my_root;
	int my_root;
	int my_tree_size;
} UNION_FIND;


char * readPGM(const char * filename, unsigned int * dst_width, unsigned int * dst_height, unsigned int * dst_max_value);

void savePGM(char * image, const char * filename, const unsigned int src_width, const unsigned int src_height, const unsigned int src_max_value);

UNION_FIND * initialize(const unsigned int data_size);

int rootUnion(const unsigned int union_root, UNION_FIND * union_data);

int areConnected(const unsigned int union_a, const unsigned int union_b, UNION_FIND * union_data);

void connectUnion(const unsigned int union_a, const unsigned int union_b, UNION_FIND * union_data, const unsigned int data_size);

void labelConnectedComponents(char * image, const unsigned int src_width, const unsigned int src_height, unsigned int * dst_max_value);


#endif // ALGORITHMS_1_INCLUDED
