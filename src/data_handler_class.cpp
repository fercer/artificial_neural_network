#include "data_handler_class.h"



data_handler::data_handler()
{
	data_size = 0;
	number_of_variables = 0;
	number_of_outputs = 0;

	data_filename[0] = '\0';
}


data_handler::~data_handler()
{
	if (input_data)
	{
		for (unsigned int i = 0; i < data_size; i++)
		{
			free(*(input_data + i));
			free(*(output_data + i));
		}

		free(input_data);
		free(output_data);
	}
}


// Loads the information from the specified file
void data_handler::loadData()
{
	using namespace std;
	ifstream fp_data_file;
	fp_data_file.open(data_filename);

	vector<double> input_values, groundtruth_values;

	/* Look for the input varibles */
	string line;
	string data_kind("");
	while (data_kind.compare("in:") != 0)
	{
		getline(fp_data_file, line);
		stringstream read_string(line);
		read_string >> data_kind;
	}

	do
	{
		stringstream read_string(line);

		read_string >> data_kind;
		if (data_kind.compare("in:") == 0)
		{
			data_size++;
			number_of_variables = 0;
			double current_input_value;
			while (read_string >> current_input_value) {
				input_values.push_back(current_input_value);
				number_of_variables++;
			}
		}

		if (data_kind.compare("out:") == 0)
		{
			number_of_outputs = 0;
			double current_output_value;
			while (read_string >> current_output_value) {
				groundtruth_values.push_back(current_output_value);
				number_of_outputs++;
			}
		}
		getline(fp_data_file, line);
	} while (!fp_data_file.eof());
	fp_data_file.close();

	output_data = (int**)malloc(data_size * sizeof(int*));
	input_data = (double**)malloc(data_size * sizeof(double*));
	vector<double>::iterator output_values_it = groundtruth_values.begin();
	vector<double>::iterator input_values_it = input_values.begin();
	for (unsigned int i = 0; i < data_size; i++)
	{
		*(output_data + i) = (int*)malloc(number_of_outputs * sizeof(int));
		*(input_data + i) = (double*)malloc(number_of_variables * sizeof(double));

		for (unsigned int j = 0; j < number_of_variables; j++, input_values_it++)
		{
			*(*(input_data + i) + j) = *input_values_it;
		}

		for (unsigned int j = 0; j < number_of_outputs; j++, output_values_it++)
		{
			*(*(output_data + i) + j) = (int)(*output_values_it);
		}
	}
}


// Assigns the path to the data source
void data_handler::setFilename(const char * src_filename)
{
	sprintf(data_filename, "%s", src_filename);
}

unsigned int data_handler::getDataSize()
{
	return data_size;
}

unsigned int data_handler::getInputSize()
{
	return number_of_variables;
}

unsigned int data_handler::getOutputSize()
{
	return number_of_outputs;
}

double ** data_handler::getInputData()
{
	return input_data;
}

int ** data_handler::getOutputData()
{
	return output_data;
}
