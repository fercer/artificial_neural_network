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

	my_args.newArgument("Path to the dataset.", "f", "filename", "NULL", false);
	my_args.newArgument("Number of variables.", "nv", "n-vars", 2, false);
	my_args.newArgument("Number of classes.", "nc", "n-classes", 1, false);
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
	const unsigned int n_vars = my_args.getArgumentINT("-nv");
	const unsigned int n_classes = my_args.getArgumentINT("-nc");

	STAUS * my_seed = initSeed(my_args.getArgumentINT("-rng"));

	FILE * fp_iris = fopen(input_filename, "r");
	FILE * fp_training = fopen(output_training_filename, "w");
	FILE * fp_testing = fopen(output_testing_filename, "w");
	FILE * fp_pattern;

	double * vars = (double*)malloc(n_vars * sizeof(double));
	char label_string[512];
	int labels;

	unsigned int training_count = 0;
	unsigned int testing_count = 0;
	const unsigned int data_size = testing_size + training_size;

	for (unsigned int pattern_idx = 0; pattern_idx < data_size; pattern_idx++)
	{
		for (unsigned int var_i = 0; var_i < n_vars; var_i++)
		{
			fscanf(fp_iris, "%lf", vars + var_i);
		}

		fscanf(fp_iris, "%i", &labels);
		
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
			if (HybTaus(0.0, 1.0, my_seed) <= ((double)training_size/(double)data_size))
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


		fprintf(fp_pattern, "in:");
		for (unsigned int var_i = 0; var_i < n_vars; var_i++)
		{
			fprintf(fp_pattern, "\t%f", *(vars + var_i));
		}

		fprintf(fp_pattern, "\nout:");
		if (n_classes > 2)
		{
			for (unsigned int label_i = 0; label_i < n_classes; label_i++)
			{
				if (labels == label_i)
				{
					fprintf(fp_pattern, "\t1");
				}
				else
				{
					fprintf(fp_pattern, "\t0");
				}
			}
		}
		else
		{
			fprintf(fp_pattern, "\t%i", labels);
		}
		fprintf(fp_pattern, "\n");

	}
	fclose(fp_iris);
	fclose(fp_training);
	fclose(fp_testing);

	free(my_seed);
	free(vars);
	return 0;
}