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
	(this->*load_data_method)();
}


void data_handler::loadDataTXT()
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



// Loads the information from the specified file
void data_handler::loadDataXML()
{
	DATA_LIST_NODE inputs_list_head;
	inputs_list_head.next_entry = NULL;
	DATA_LIST_NODE * inputs_list_tail = &inputs_list_head;

	DATA_LIST_NODE outputs_list_head;
	outputs_list_head.next_entry = NULL;
	DATA_LIST_NODE * outputs_list_tail = &outputs_list_head;

	number_of_variables = 0;
	data_size = 0;
	number_of_outputs = 0;

	using namespace rapidxml;
	using namespace std;
	xml_document<> doc;
	xml_node<> * root_node;

	// Read the xml file into a vector
	ifstream theFile(data_filename);
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');

	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);

	// Find our root node
	root_node = doc.first_node("Data");

	// Iterate over the first Input to known the number of variables:
	for (xml_node<> * input_node = root_node->first_node("InputData"); input_node; input_node = input_node->next_sibling())
	{
		xml_node<> * input_pattern_node = input_node->first_node("Input");
		for (xml_node<> * feature_node = input_pattern_node->first_node("Feature"); feature_node; feature_node = feature_node->next_sibling())
		{
			number_of_variables++;
		}
	}

	// Iterate over the Inputs:
	for (xml_node<> * input_node = root_node->first_node("InputData"); input_node; input_node = input_node->next_sibling())
	{
		for (xml_node<> * input_pattern_node = input_node->first_node("Input"); input_pattern_node; input_pattern_node = input_pattern_node->next_sibling())
		{

			if (strcmp(input_pattern_node->name(), "Input") != 0)
			{
				break;
			}

			inputs_list_tail->next_entry = new DATA_LIST_NODE;
			inputs_list_tail = inputs_list_tail->next_entry;
			inputs_list_tail->next_entry = NULL;
			inputs_list_tail->variable_value.input = (double*)malloc(number_of_variables * sizeof(double));

			unsigned int current_variable = 0;
			for (xml_node<> * feature_node = input_pattern_node->first_node("Feature"); feature_node; feature_node = feature_node->next_sibling(), current_variable++)
			{
				char * feature_node_character_value = feature_node->first_attribute("value")->value();
				double feature_node_numeric_value = atof(feature_node_character_value);
				*(inputs_list_tail->variable_value.input + current_variable) = feature_node_numeric_value;
			}
			data_size++;
		}
	}

	// Iterate over the first Input to known the number of variables:
	for (xml_node<> * output_node = root_node->first_node("OutputData"); output_node; output_node = output_node->next_sibling())
	{
		xml_node<> * output_pattern_node = output_node->first_node("Output");
		for (xml_node<> * label_node = output_pattern_node->first_node("Feature"); label_node; label_node = label_node->next_sibling())
		{
			number_of_outputs++;
		}
	}


	// Iterate over the Inputs:
	for (xml_node<> * output_node = root_node->first_node("OutputData"); output_node; output_node = output_node->next_sibling())
	{
		for (xml_node<> * output_pattern_node = output_node->first_node("Output"); output_pattern_node; output_pattern_node = output_pattern_node->next_sibling())
		{
			if (strcmp(output_pattern_node->name(), "Output") != 0)
			{
				break;
			}

			outputs_list_tail->next_entry = new DATA_LIST_NODE;
			outputs_list_tail = outputs_list_tail->next_entry;
			outputs_list_tail->next_entry = NULL;
			outputs_list_tail->variable_value.output = (int*)malloc(number_of_variables * sizeof(int));

			unsigned int current_label = 0;
			for (xml_node<> * label_node = output_pattern_node->first_node("Label"); label_node; label_node = label_node->next_sibling(), current_label++)
			{
				char * label_node_character_value = label_node->first_attribute("value")->value();
				int label_node_numeric_value = atoi(label_node_character_value);
				*(outputs_list_tail->variable_value.output + current_label) = label_node_numeric_value;
			}
		}
	}



	output_data = (int**)malloc(data_size * sizeof(int*));
	input_data = (double**)malloc(data_size * sizeof(double*));

	DATA_LIST_NODE * current_input_node;
	DATA_LIST_NODE * next_input_node = inputs_list_head.next_entry;

	DATA_LIST_NODE * current_output_node;
	DATA_LIST_NODE * next_output_node = outputs_list_head.next_entry;

	unsigned int current_pattern = 0;
	while (next_output_node)
	{
		current_input_node = next_input_node;
		next_input_node = current_input_node->next_entry;

		current_output_node = next_output_node;
		next_output_node = current_output_node->next_entry;

		*(input_data + current_pattern) = current_input_node->variable_value.input;
		*(output_data + current_pattern) = current_output_node->variable_value.output;

		delete current_input_node;
		delete current_output_node;

		current_pattern++;
	}
}


// Assigns the path to the data source
void data_handler::setFilename(const char * src_filename)
{
	sprintf(data_filename, "%s", src_filename);
	
	if (strstr(data_filename, ".xml"))
	{
		load_data_method = &data_handler::loadDataXML;
	}
	else
	{
		load_data_method = &data_handler::loadDataTXT;
	}	
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
