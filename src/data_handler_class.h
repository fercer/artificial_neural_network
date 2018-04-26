#ifndef DATA_HANDLER_CLASS_INCLUDED
#define DATA_HANDLER_CLASS_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class data_handler
{
public:
	data_handler();
	~data_handler();

	// Loads the information from the specified file
	void loadData();

	// Assigns the path to the data source
	void setFilename(const char * src_filename);

	unsigned int getDataSize();
	unsigned int getInputSize();
	unsigned int getOutputSize();

	double ** getInputData();
	int ** getOutputData();

private:
	char data_filename[512];
	unsigned int data_size;
	unsigned int number_of_variables;
	unsigned int number_of_outputs;

	double ** input_data;
	int ** output_data;
};

#endif // DATA_HANDLER_CLASS_INCLUDED