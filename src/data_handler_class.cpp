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
			free(*(output_float_data + i));
		}

		free(input_data);
		free(output_data);
		free(output_float_data);
	}
}


// Loads the information from the specified file
void data_handler::loadData()
{
	(this->*load_data_method)();
}


void data_handler::loadDataTXT()
{
	DATA_LIST_NODE inputs_list_head;
	inputs_list_head.next_entry = NULL;
	DATA_LIST_NODE * inputs_list_tail = &inputs_list_head;

	DATA_LIST_NODE outputs_list_head;
	outputs_list_head.next_entry = NULL;
	DATA_LIST_NODE * outputs_list_tail = &outputs_list_head;

	DATA_LIST_NODE outputs_float_list_head;
	outputs_float_list_head.next_entry = NULL;
	DATA_LIST_NODE * outputs_float_list_tail = &outputs_float_list_head;


	number_of_variables = 0;
	data_size = 0;
	number_of_outputs = 0;

	using namespace std;
	FILE * fp_data_file = fopen(data_filename, "r");

	// Read the first input to define the number of variables:
	/* Look for the input varibles */
	char line[1024];
	char * current_character;
	char * next_character;
	char read_value[1024];
	while (!strstr(line, "in:"))
	{
		current_character = fgets(line, 1024, fp_data_file);
	}

	current_character = strpbrk(line, "\t");
	next_character = strpbrk(current_character + 1, "\t\n");
	do
	{
		current_character = next_character;
		next_character = strpbrk(current_character + 1, "\t\n");

		number_of_variables++;
	} while (next_character);

	while (!strstr(line, "out:"))
	{
		current_character = fgets(line, 1024, fp_data_file);
	}

	current_character = strpbrk(line, "\t");
	next_character = strpbrk(current_character + 1, "\t\n");
	do
	{
		current_character = next_character;
		next_character = strpbrk(current_character + 1, "\t\n");

		number_of_outputs++;
	} while (next_character);

	rewind(fp_data_file);

	current_character = fgets(line, 1024, fp_data_file);
	/* Look for the input varibles */
	while (!strstr(line, "in:"))
	{
		current_character = fgets(line, 1024, fp_data_file);
	}

	do
	{
		if (strstr(line, "in:"))
		{
			data_size++;
			
			inputs_list_tail->next_entry = new DATA_LIST_NODE;
			inputs_list_tail = inputs_list_tail->next_entry;
			inputs_list_tail->next_entry = NULL;
			inputs_list_tail->variable_value.input = (double*)malloc(number_of_variables * sizeof(double));

			unsigned int current_input_index = 0;
			current_character = strpbrk(line, "\t");
			next_character = strpbrk(current_character + 1, "\t\n");
			do
			{
				strncpy(read_value, current_character + 1, next_character - current_character);

				current_character = next_character;
				next_character = strpbrk(current_character + 1, "\t\n");

				*(inputs_list_tail->variable_value.input + current_input_index) = atof(read_value);

				current_input_index++;
			} while (next_character);
		}
		
		if (strstr(line, "out:"))
		{
			outputs_list_tail->next_entry = new DATA_LIST_NODE;
			outputs_list_tail = outputs_list_tail->next_entry;
			outputs_list_tail->next_entry = NULL;
			outputs_list_tail->variable_value.output = (int*)malloc(number_of_outputs * sizeof(int));

			outputs_float_list_tail->next_entry = new DATA_LIST_NODE;
			outputs_float_list_tail = outputs_float_list_tail->next_entry;
			outputs_float_list_tail->next_entry = NULL;
			outputs_float_list_tail->variable_value.output_float = (double*)malloc(number_of_outputs * sizeof(double));

			unsigned int current_output_index = 0;
			current_character = strpbrk(line, "\t");
			next_character = strpbrk(current_character + 1, "\t\n");
			do
			{
				strncpy(read_value, current_character + 1, next_character - current_character);

				current_character = next_character;
				next_character = strpbrk(current_character + 1, "\t\n");

				*(outputs_list_tail->variable_value.output + current_output_index) = atoi(read_value);
				*(outputs_float_list_tail->variable_value.output_float + current_output_index) = atof(read_value);

				current_output_index++;
			} while (next_character);
		}
		current_character = fgets(line, 1024, fp_data_file);
	} while (!feof(fp_data_file));

	fclose(fp_data_file);

	output_data = (int**)malloc(data_size * sizeof(int*));
	output_float_data = (double**)malloc(data_size * sizeof(double*));
	input_data = (double**)malloc(data_size * sizeof(double*));

	DATA_LIST_NODE * current_input_node;
	DATA_LIST_NODE * next_input_node = inputs_list_head.next_entry;

	DATA_LIST_NODE * current_output_node;
	DATA_LIST_NODE * next_output_node = outputs_list_head.next_entry;

	DATA_LIST_NODE * current_output_float_node;
	DATA_LIST_NODE * next_output_float_node = outputs_float_list_head.next_entry;

	unsigned int current_pattern = 0;
	while (next_output_node)
	{
		current_input_node = next_input_node;
		next_input_node = current_input_node->next_entry;

		current_output_node = next_output_node;
		next_output_node = current_output_node->next_entry;

		current_output_float_node = next_output_float_node;
		next_output_float_node = current_output_float_node->next_entry;

		*(input_data + current_pattern) = current_input_node->variable_value.input;
		*(output_data + current_pattern) = current_output_node->variable_value.output;
		*(output_float_data + current_pattern) = current_output_float_node->variable_value.output_float;

		delete current_input_node;
		delete current_output_node;
		delete current_output_float_node;

		current_pattern++;
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

	DATA_LIST_NODE outputs_float_list_head;
	outputs_float_list_head.next_entry = NULL;
	DATA_LIST_NODE * outputs_float_list_tail = &outputs_float_list_head;

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

	// Iterate over the first output to known the number of expected outputs:
	for (xml_node<> * output_node = root_node->first_node("OutputData"); output_node; output_node = output_node->next_sibling())
	{
		xml_node<> * output_pattern_node = output_node->first_node("Output");
		for (xml_node<> * label_node = output_pattern_node->first_node("Feature"); label_node; label_node = label_node->next_sibling())
		{
			number_of_outputs++;
		}
	}


	// Iterate over the Outputs:
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


			outputs_float_list_tail->next_entry = new DATA_LIST_NODE;
			outputs_float_list_tail = outputs_float_list_tail->next_entry;
			outputs_float_list_tail->next_entry = NULL;
			outputs_float_list_tail->variable_value.output_float = (double*)malloc(number_of_variables * sizeof(double));

			unsigned int current_label = 0;
			for (xml_node<> * label_node = output_pattern_node->first_node("Label"); label_node; label_node = label_node->next_sibling(), current_label++)
			{
				char * label_node_character_value = label_node->first_attribute("value")->value();

				*(outputs_list_tail->variable_value.output + current_label) = atoi(label_node_character_value);
				*(outputs_float_list_tail->variable_value.output + current_label) = atof(label_node_character_value);
			}
		}
	}



	output_data = (int**)malloc(data_size * sizeof(int*));
	output_float_data = (double**)malloc(data_size * sizeof(double*));
	input_data = (double**)malloc(data_size * sizeof(double*));

	DATA_LIST_NODE * current_input_node;
	DATA_LIST_NODE * next_input_node = inputs_list_head.next_entry;

	DATA_LIST_NODE * current_output_node;
	DATA_LIST_NODE * next_output_node = outputs_list_head.next_entry;

	DATA_LIST_NODE * current_output_float_node;
	DATA_LIST_NODE * next_output_float_node = outputs_float_list_head.next_entry;

	unsigned int current_pattern = 0;
	while (next_output_node)
	{
		current_input_node = next_input_node;
		next_input_node = current_input_node->next_entry;

		current_output_node = next_output_node;
		next_output_node = current_output_node->next_entry;
		
		current_output_float_node = next_output_float_node;
		next_output_float_node = current_output_float_node->next_entry;

		*(input_data + current_pattern) = current_input_node->variable_value.input;
		*(output_data + current_pattern) = current_output_node->variable_value.output;
		*(output_float_data + current_pattern) = current_output_node->variable_value.output_float;

		delete current_input_node;
		delete current_output_node;
		delete current_output_float_node;

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

double ** data_handler::getOutputFloatData()
{
	return output_float_data;
}

