#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/args_fercer.h"
#include "../src/random_numbers_generator.h"

int main(int argc, char* argv[])
{

	ARGUMENTS my_args;
	my_args.setArgv(argv);
	my_args.setArgc(argc);

	my_args.newArgument("Path to the iris dataset.", "f", "filename", "NULL", false);
	my_args.newArgument("Path to the output training xml file.", "or", "output-training", "NULL", false);
	my_args.newArgument("Path to the output testing xml file.", "oe", "output-testing", "NULL", false);
	my_args.newArgument("Training data size.", "rs", "training-size", 100, true);
	my_args.newArgument("Testing data size.", "es", "testing-size", 50, true);
	my_args.newArgument("RNG seed.", "rng", "rng-seed", 0, true);

	my_args.showHelp();

	char * input_filename = my_args.getArgumentCHAR("-f");
	char * output_training_filename = my_args.getArgumentCHAR("-or");
	char * output_testing_filename = my_args.getArgumentCHAR("-oe");
	const unsigned int training_size = my_args.getArgumentINT("-rs");
	const unsigned int testing_size = my_args.getArgumentINT("-es");

	STAUS * my_seed = initSeed(my_args.getArgumentINT("-rng"));

	FILE * fp_iris = fopen(input_filename, "r");
	FILE * fp_training = fopen(output_training_filename, "w");
	FILE * fp_testing = fopen(output_testing_filename, "w");
	FILE * fp_pattern;

	double var_1, var_2, var_3, var_4;
	char label_string[512];
	int label_1, label_2, label_3;

	unsigned int training_count = 0;
	unsigned int testing_count = 0;

	for (unsigned int pattern_idx = 0; pattern_idx < 150; pattern_idx++)
	{

		fscanf(fp_iris, "%lf %lf %lf %lf %s", &var_1, &var_2, &var_3, &var_4, label_string);
		//fgets(label_string, 511, fp_iris);

		if (strcmp(label_string, "setosa") == 0)
		{
			label_1 = 1;
			label_2 = 0;
			label_3 = 0;
		}
		else if(strcmp(label_string, "versicolor") == 0)
		{
			label_1 = 0;
			label_2 = 1;
			label_3 = 0;
		}
		else
		{
			label_1 = 0;
			label_2 = 0;
			label_3 = 1;
		}

		if (training_count >= training_size)
		{
			// Assign the current pattern to the testing set:
			fp_pattern = fp_testing;
			testing_count++;
		}
		else if (testing_count >= testing_size)
		{
			// Assign the current pattern to the training set:
			fp_pattern = fp_training;
			training_count++;
		}
		else
		{
			// Assign the current pattern either to the testing set, or training set, with probabiliy P(training_set) = 150 / training_size, and P(testing_set) = 150 / testing_size:
			if (HybTaus(0.0, 1.0, my_seed) <= ((double)training_size/ 150.0))
			{
				fp_pattern = fp_training;
				training_count++;
			}
			else
			{
				fp_pattern = fp_testing;
				testing_count++;
			}
		}

		fprintf(fp_pattern, "in:\t%f\t%f\t%f\t%f\nout:\t%i\t%i\t%i\n", var_1, var_2, var_3, var_4, label_1, label_2, label_3);
	}
	fclose(fp_iris);
	fclose(fp_training);
	fclose(fp_testing);

	free(my_seed);

	return 0;
}