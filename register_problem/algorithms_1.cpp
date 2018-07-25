#include "algorithms_1.h"




char * readPGM(const char * src_filename, unsigned int * dst_width, unsigned int * dst_height, unsigned int * dst_max_value)
{
	FILE * fp_image = fopen(src_filename, "r");

	char temp[512];
	// Read magic number:
	fgets(temp, 512, fp_image);

	// Read commentary:
	fgets(temp, 512, fp_image);

	// Read size of the image:
	unsigned int width;
	unsigned int height;
	fscanf(fp_image, "%i %i", &width, &height);

	if (dst_width)
	{
		*dst_width = width;
	}

	if (dst_height)
	{
		*dst_height = height;
	}

	// Read the max value:
	unsigned int max_value;
	fscanf(fp_image, "%i", &max_value);
	if (dst_max_value)
	{
		*dst_max_value = max_value;
	}

	char * new_image = (char*)malloc(height * width * sizeof(char));
	
	fscanf(fp_image, "%s", new_image);

	fclose(fp_image);

	return new_image;
}



void savePGM(char * image, const char * filename, const unsigned int src_width, const unsigned int src_height, const unsigned int src_max_value)
{
	FILE * fp_image = fopen(filename, "w");
	
	fprintf(fp_image, "P2\n");
	fprintf(fp_image, "# CREATOR: FerCer\n");
	fprintf(fp_image, "%i %i\n%i\n", src_width, src_height, src_max_value);
	for (unsigned int position = 0; position < (src_height * src_width); position++)
	{
		fprintf(fp_image, "%i\n", (int)*(image + position));
	}
	fclose(fp_image);
}



UNION_FIND * initialize(const unsigned int data_size)
{
	UNION_FIND * new_union = (UNION_FIND*)malloc(data_size * sizeof(UNION_FIND));
	for (unsigned int union_id = 0; union_id < data_size; union_id++)
	{
		(new_union + union_id)->my_root = union_id;
		(new_union + union_id)->my_tree_size = 1;
	}
	
	return new_union;
}



int rootUnion(const unsigned int union_root, UNION_FIND * union_data)
{
	if (union_root == (union_data + union_root)->my_root)
	{
		return union_root;
	}

	(union_data + union_root)->my_root = rootUnion((union_data + union_root)->my_root, union_data);

	return (union_data + union_root)->my_root;
}



int areConnected(const unsigned int union_a, const unsigned int union_b, UNION_FIND * union_data)
{
	const unsigned int root_a = rootUnion(union_a, union_data);
	const unsigned int root_b = rootUnion(union_b, union_data);
	return  root_a == root_b;
}



void connectUnion(const unsigned int union_a, const unsigned int union_b, UNION_FIND * union_data, const unsigned int data_size)
{
	const int root_a = rootUnion(union_a, union_data);
	const int root_b = rootUnion(union_b, union_data);
	if ((union_data + root_a)->my_tree_size < (union_data + root_b)->my_tree_size)
	{
		(union_data + root_a)->my_root = root_b;
		(union_data + root_a)->my_tree_size += (union_data + root_b)->my_tree_size;
	}
	else
	{
		(union_data + root_b)->my_root = root_a;
		(union_data + root_b)->my_tree_size += (union_data + root_a)->my_tree_size;
	}
}



void labelConnectedComponents(char * image, const unsigned int src_width, const unsigned int src_height, unsigned int * dst_max_value)
{
	UNION_FIND * union_data = initialize(src_height * src_width);

	// Naive connected components:
	/// Upper left corner
	{
		const unsigned int position_i = 0;
		const unsigned int position_j = 0;

		const int current_value = *(image + position_i * src_width + position_j);

		// Rigth:
		if (*(image + position_i * src_width + position_j + 1) == current_value)
		{
			if (!areConnected(position_i*src_width + position_j, position_i * src_width + position_j + 1, union_data))
			{
				connectUnion(position_i*src_width + position_j, position_i * src_width + position_j + 1, union_data, src_height * src_width);
			}
		}

		// Lower rigth:
		if (*(image + (position_i + 1) * src_width + position_j + 1) == current_value)
		{
			if (!areConnected(position_i*src_width + position_j, (position_i + 1) * src_width + position_j + 1, union_data))
			{
				connectUnion(position_i*src_width + position_j, (position_i + 1) * src_width + position_j + 1, union_data, src_height * src_width);
			}
		}

		// Lower center:
		if (*(image + (position_i + 1) * src_width + position_j) == current_value)
		{
			if (!areConnected(position_i*src_width + position_j, (position_i + 1) * src_width + position_j, union_data))
			{
				connectUnion(position_i*src_width + position_j, (position_i + 1) * src_width + position_j, union_data, src_height * src_width);
			}
		}
	}

	/// Upper row
	{
		const unsigned int position_i = 0;
		for (unsigned int position_j = 1; position_j < (src_width - 1); position_j++)
		{
			const int current_value = *(image + position_i * src_width + position_j);

			// Rigth:
			if (*(image + position_i * src_width + position_j + 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, position_i * src_width + position_j + 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, position_i * src_width + position_j + 1, union_data, src_height * src_width);
				}
			}

			// Lower rigth:
			if (*(image + (position_i + 1) * src_width + position_j + 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i + 1) * src_width + position_j + 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i + 1) * src_width + position_j + 1, union_data, src_height * src_width);
				}
			}

			// Lower center:
			if (*(image + (position_i + 1) * src_width + position_j) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i + 1) * src_width + position_j, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i + 1) * src_width + position_j, union_data, src_height * src_width);
				}
			}

			// Lower left:
			if (*(image + (position_i + 1) * src_width + position_j - 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i + 1) * src_width + position_j - 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i + 1) * src_width + position_j - 1, union_data, src_height * src_width);
				}
			}

			/*
			// Left:
			if (*(image + position_i * src_width + position_j - 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, position_i * src_width + position_j - 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, position_i * src_width + position_j - 1, union_data, src_height * src_width);
				}
			}
			*/
		}
	}

	/// Upper right corner
	{
		const unsigned int position_i = 0;
		const unsigned int position_j = (src_width - 1);

		const int current_value = *(image + position_i * src_width + position_j);

		// Lower center:
		if (*(image + (position_i + 1) * src_width + position_j) == current_value)
		{
			if (!areConnected(position_i*src_width + position_j, (position_i + 1) * src_width + position_j, union_data))
			{
				connectUnion(position_i*src_width + position_j, (position_i + 1) * src_width + position_j, union_data, src_height * src_width);
			}
		}
		/*
		// Lower left:
		if (*(image + (position_i + 1) * src_width + position_j - 1) == current_value)
		{
			if (!areConnected(position_i*src_width + position_j, (position_i + 1) * src_width + position_j - 1, union_data))
			{
				connectUnion(position_i*src_width + position_j, (position_i + 1) * src_width + position_j - 1, union_data, src_height * src_width);
			}
		}

		// Left:
		if (*(image + position_i * src_width + position_j - 1) == current_value)
		{
			if (!areConnected(position_i*src_width + position_j, position_i * src_width + position_j - 1, union_data))
			{
				connectUnion(position_i*src_width + position_j, position_i * src_width + position_j - 1, union_data, src_height * src_width);
			}
		}
		*/
	}


	/// Center
	for (unsigned int position_i = 1; position_i < (src_height - 1); position_i++)
	{
		/// Left column
		{
			const unsigned int position_j = 0;

			const int current_value = *(image + position_i * src_width + position_j);
			/*
			// Upper center:
			if (*(image + (position_i - 1) * src_width + position_j) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i - 1) * src_width + position_j, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i - 1) * src_width + position_j, union_data, src_height * src_width);
				}
			}
			*/
			// Upper rigth:
			if (*(image + (position_i - 1) * src_width + position_j + 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i - 1) * src_width + position_j + 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i - 1) * src_width + position_j + 1, union_data, src_height * src_width);
				}
			}

			// Rigth:
			if (*(image + position_i * src_width + position_j + 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, position_i * src_width + position_j + 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, position_i * src_width + position_j + 1, union_data, src_height * src_width);
				}
			}

			// Lower rigth:
			if (*(image + (position_i + 1) * src_width + position_j + 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i + 1) * src_width + position_j + 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i + 1) * src_width + position_j + 1, union_data, src_height * src_width);
				}
			}

			// Lower center:
			if (*(image + (position_i + 1) * src_width + position_j) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i + 1) * src_width + position_j, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i + 1) * src_width + position_j, union_data, src_height * src_width);
				}
			}
		}
		
		for (unsigned int position_j = 1; position_j < (src_width - 1); position_j++)
		{
			const int current_value = *(image + position_i * src_width + position_j);
			/*
			// Upper left:
			if (*(image + (position_i - 1) * src_width + position_j - 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i - 1) * src_width + position_j - 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i - 1) * src_width + position_j - 1, union_data, src_height * src_width);
				}
			}

			// Upper center:
			if (*(image + (position_i - 1) * src_width + position_j) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i - 1) * src_width + position_j, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i - 1) * src_width + position_j, union_data, src_height * src_width);
				}
			}
			*/

			// Upper rigth:
			if (*(image + (position_i - 1) * src_width + position_j + 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i - 1) * src_width + position_j + 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i - 1) * src_width + position_j + 1, union_data, src_height * src_width);
				}
			}

			// Rigth:
			if (*(image + position_i * src_width + position_j + 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, position_i * src_width + position_j + 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, position_i * src_width + position_j + 1, union_data, src_height * src_width);
				}
			}

			// Lower rigth:
			if (*(image + (position_i + 1) * src_width + position_j + 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i + 1) * src_width + position_j + 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i + 1) * src_width + position_j + 1, union_data, src_height * src_width);
				}
			}

			// Lower center:
			if (*(image + (position_i + 1) * src_width + position_j) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i + 1) * src_width + position_j, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i + 1) * src_width + position_j, union_data, src_height * src_width);
				}
			}

			// Lower left:
			if (*(image + (position_i + 1) * src_width + position_j - 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i + 1) * src_width + position_j - 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i + 1) * src_width + position_j - 1, union_data, src_height * src_width);
				}
			}

			/*
			// Left:
			if (*(image + position_i * src_width + position_j - 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, position_i * src_width + position_j - 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, position_i * src_width + position_j - 1, union_data, src_height * src_width);
				}
			}
			*/
		}
		
		/// Right column
		{
			const unsigned int position_j = (src_width - 1);

			const int current_value = *(image + position_i * src_width + position_j);

			/*
			// Upper left:
			if (*(image + (position_i - 1) * src_width + position_j - 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i - 1) * src_width + position_j - 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i - 1) * src_width + position_j - 1, union_data, src_height * src_width);
				}
			}

			// Upper center:
			if (*(image + (position_i - 1) * src_width + position_j) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i - 1) * src_width + position_j, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i - 1) * src_width + position_j, union_data, src_height * src_width);
				}
			}
			*/
			// Lower center:
			if (*(image + (position_i + 1) * src_width + position_j) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i + 1) * src_width + position_j, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i + 1) * src_width + position_j, union_data, src_height * src_width);
				}
			}
			/*
			// Lower left:
			if (*(image + (position_i + 1) * src_width + position_j - 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i + 1) * src_width + position_j - 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i + 1) * src_width + position_j - 1, union_data, src_height * src_width);
				}
			}

			// Left:
			if (*(image + position_i * src_width + position_j - 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, position_i * src_width + position_j - 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, position_i * src_width + position_j - 1, union_data, src_height * src_width);
				}
			}
			*/
		}
	}

	/// Lower left corner
	{
		const unsigned int position_i = (src_height - 1);
		const unsigned int position_j = 0;

		const int current_value = *(image + position_i * src_width + position_j);
		/*
		// Upper center:
		if (*(image + (position_i - 1) * src_width + position_j) == current_value)
		{
			if (!areConnected(position_i*src_width + position_j, (position_i - 1) * src_width + position_j, union_data))
			{
				connectUnion(position_i*src_width + position_j, (position_i - 1) * src_width + position_j, union_data, src_height * src_width);
			}
		}

		// Upper rigth:
		if (*(image + (position_i - 1) * src_width + position_j + 1) == current_value)
		{
			if (!areConnected(position_i*src_width + position_j, (position_i - 1) * src_width + position_j + 1, union_data))
			{
				connectUnion(position_i*src_width + position_j, (position_i - 1) * src_width + position_j + 1, union_data, src_height * src_width);
			}
		}
		*/
		// Rigth:
		if (*(image + position_i * src_width + position_j + 1) == current_value)
		{
			if (!areConnected(position_i*src_width + position_j, position_i * src_width + position_j + 1, union_data))
			{
				connectUnion(position_i*src_width + position_j, position_i * src_width + position_j + 1, union_data, src_height * src_width);
			}
		}
	}

	/// Lower row
	{
		const unsigned int position_i = (src_height - 1);
		for (unsigned int position_j = 1; position_j < (src_width - 1); position_j++)
		{
			const int current_value = *(image + position_i * src_width + position_j);
			/*
			// Upper left:
			if (*(image + (position_i - 1) * src_width + position_j - 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i - 1) * src_width + position_j - 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i - 1) * src_width + position_j - 1, union_data, src_height * src_width);
				}
			}

			// Upper center:
			if (*(image + (position_i - 1) * src_width + position_j) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i - 1) * src_width + position_j, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i - 1) * src_width + position_j, union_data, src_height * src_width);
				}
			}

			// Upper rigth:
			if (*(image + (position_i - 1) * src_width + position_j + 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, (position_i - 1) * src_width + position_j + 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, (position_i - 1) * src_width + position_j + 1, union_data, src_height * src_width);
				}
			}
			*/
			// Rigth:
			if (*(image + position_i * src_width + position_j + 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, position_i * src_width + position_j + 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, position_i * src_width + position_j + 1, union_data, src_height * src_width);
				}
			}
			/*
			// Left:
			if (*(image + position_i * src_width + position_j - 1) == current_value)
			{
				if (!areConnected(position_i*src_width + position_j, position_i * src_width + position_j - 1, union_data))
				{
					connectUnion(position_i*src_width + position_j, position_i * src_width + position_j - 1, union_data, src_height * src_width);
				}
			}
			*/
		}
	}
	/*
	/// Lower right corner
	{
		const unsigned int position_i = (src_height - 1);
		const unsigned int position_j = (src_width - 1);

		const int current_value = *(image + position_i * src_width + position_j);

		// Upper left:
		if (*(image + (position_i - 1) * src_width + position_j - 1) == current_value)
		{
			if (!areConnected(position_i*src_width + position_j, (position_i - 1) * src_width + position_j - 1, union_data))
			{
				connectUnion(position_i*src_width + position_j, (position_i - 1) * src_width + position_j - 1, union_data, src_height * src_width);
			}
		}

		// Upper center:
		if (*(image + (position_i - 1) * src_width + position_j) == current_value)
		{
			if (!areConnected(position_i*src_width + position_j, (position_i - 1) * src_width + position_j, union_data))
			{
				connectUnion(position_i*src_width + position_j, (position_i - 1) * src_width + position_j, union_data, src_height * src_width);
			}
		}

		// Left:
		if (*(image + position_i * src_width + position_j - 1) == current_value)
		{
			if (!areConnected(position_i*src_width + position_j, position_i * src_width + position_j - 1, union_data))
			{
				connectUnion(position_i*src_width + position_j, position_i * src_width + position_j - 1, union_data, src_height * src_width);
			}
		}
	}
	*/

	// Dump the union data to the image data:
	int * components_labels = (int*)calloc(src_height * src_width, sizeof(int));
	int last_label = 1;
	int max_label = 0;
	for (unsigned int position = 0; position < (src_height * src_width); position++)
	{
		const int current_root = rootUnion(position, union_data);
		if (!*(components_labels + current_root))
		{
			*(components_labels + current_root) = last_label;
			last_label++;
		}

		*(image + position) = *(components_labels + current_root) - 1;
	}
	*dst_max_value = last_label - 1;

	free(components_labels);
	free(union_data);
}