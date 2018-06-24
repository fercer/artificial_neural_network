#ifndef IMAGE_OPERATION_CLASS_H_INCLUDED
#define IMAGE_OPERATION_CLASS_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node_scalar_class.h"
#include "generic_list_class.cpp"
#include "image_functions.h"


class IMAGE_OPERATION 
{
public:
	IMAGE_OPERATION()
	{
		inputs_have_changed = true;
		
		operation_A = NULL;
		operation_B = NULL;
		src_A_is_assigned = false;
		src_B_is_assigned = false;

		dst_img = NULL;

		src_img_A = NULL;
		src_img_B = NULL;

		ULa_x = 0;
		ULa_y = 0;
		LRa_x = 0;
		LRa_y = 0;
		ULb_x = 0;
		ULb_y = 0;
		LRb_x = 0;
		LRb_y = 0;

		parameters_count = 0;
		parameters_have_changed = true;

		input_numeric_nodes_required = 0;
		input_string_nodes_required = 0;

		strcpy(image_operation_name, "");

	}

	~IMAGE_OPERATION() 
	{
		if (dst_img)
		{
			freeImageData(this->dst_img);
		}
	}
	
	IMG_DATA * getImageData() 
	{
		if (inputs_have_changed || parameters_have_changed)
		{
			if (operation_A)
			{
				src_img_A = operation_A->getImageData();

				width_A = src_img_A->width;
				height_A = src_img_A->height;

				ULa_x = src_img_A->head_roi.UL_x;
				ULa_y = src_img_A->head_roi.UL_y;
				LRa_x = src_img_A->head_roi.LR_x;
				LRa_y = src_img_A->head_roi.LR_y;
			}

			if (operation_B)
			{
				src_img_B = operation_B->getImageData();

				width_B = src_img_B->width;
				height_B = src_img_B->height;

				ULb_x = src_img_B->head_roi.UL_x;
				ULb_y = src_img_B->head_roi.UL_y;
				LRb_x = src_img_B->head_roi.LR_x;
				LRb_y = src_img_B->head_roi.LR_y;
			}

			performOperation();

			inputs_have_changed = false;
			parameters_have_changed = false;
		}
		return dst_img;
	}

	void setInputOperationA(IMAGE_OPERATION * src_operation_A)
	{
		operation_A = src_operation_A;
		inputs_have_changed = true;
		src_A_is_assigned = true;
	}

	void setInputOperationB(IMAGE_OPERATION * src_operation_B)
	{
		operation_B = src_operation_B;
		inputs_have_changed = true;
		src_B_is_assigned = true;
	}
	
	void setImageName(const char * src_image_name)
	{
		strcpy(image_operation_name, src_image_name);
	}
	
	void assignNodeValue(const unsigned int src_node_position, const double src_node_value)
	{
		local_numeric_nodes_list.getNodeValue(src_node_position).setScalarValue(src_node_value);
		numeric_node_is_local_list.assignNodeValue(src_node_position, true);
		numeric_nodes_list.assignNodeValue(src_node_position, &local_numeric_nodes_list.getNodeValue(src_node_position));
		parameters_have_changed = true;
	}


	void assignNodeValue(const unsigned int src_node_position, const char* src_node_value)
	{
		local_string_nodes_list.getNodeValue(src_node_position).setScalarValue(src_node_value);
		string_node_is_local_list.assignNodeValue(src_node_position, true);
		string_nodes_list.assignNodeValue(src_node_position, &local_string_nodes_list.getNodeValue(src_node_position));
		parameters_have_changed = true;
	}

	void assignNodeValue(const unsigned int src_node_position, NODE_SCALAR<double> * src_node_pointer)
	{
		if (src_node_position >= input_numeric_nodes_required)
		{
			return;
		}

		if (src_node_pointer)
		{
			numeric_node_is_local_list.assignNodeValue(src_node_position, false);
			numeric_nodes_list.assignNodeValue(src_node_position, src_node_pointer);
			parameters_have_changed = true;
		}
		else
		{
			numeric_node_is_local_list.assignNodeValue(src_node_position, true);
			local_numeric_nodes_list.assignNodeValue(src_node_position, numeric_nodes_list.getNodeValue(src_node_position)->getScalarValue());
			numeric_nodes_list.assignNodeValue(src_node_position, &local_numeric_nodes_list.getNodeValue(src_node_position));
		}
	}


	void assignNodeValue(const unsigned int src_node_position, NODE_SCALAR<char*> * src_node_pointer)
	{
		if (src_node_position >= input_string_nodes_required)
		{
			return;
		}

		if (src_node_pointer)
		{
			string_node_is_local_list.assignNodeValue(src_node_position, false);
			string_nodes_list.assignNodeValue(src_node_position, src_node_pointer);
			parameters_have_changed = true;
		}
		else
		{
			string_node_is_local_list.assignNodeValue(src_node_position, true);
			local_string_nodes_list.assignNodeValue(src_node_position, string_nodes_list.getNodeValue(src_node_position)->getScalarValue());
			string_nodes_list.assignNodeValue(src_node_position, &local_string_nodes_list.getNodeValue(src_node_position));
		}
	}
	
	unsigned int getInputNumericNodesRequired()
	{
		return input_numeric_nodes_required;
	}
	
	unsigned int getInputStringNodesRequired()
	{
		return input_string_nodes_required;
	}
	
	char * getNumericInputName(const unsigned int src_variable_index)
	{
		return numeric_nodes_names_list.getNodeValue(src_variable_index).getScalarValue();
	}

	char * getStringInputName(const unsigned int src_variable_index)
	{
		return string_nodes_names_list.getNodeValue(src_variable_index).getScalarValue();
	}


protected:

	unsigned int input_numeric_nodes_required;
	unsigned int input_string_nodes_required;

	GENERIC_LIST<NODE_SCALAR<char*>> numeric_nodes_names_list;
	GENERIC_LIST<NODE_SCALAR<bool>> numeric_node_is_local_list;
	GENERIC_LIST<NODE_SCALAR<double>> local_numeric_nodes_list;
	GENERIC_LIST<NODE_SCALAR<char*>> string_nodes_names_list;
	GENERIC_LIST<NODE_SCALAR<bool>> string_node_is_local_list;
	GENERIC_LIST<NODE_SCALAR<char*>> local_string_nodes_list;

	GENERIC_LIST<NODE_SCALAR<double>*> numeric_nodes_list;
	GENERIC_LIST<NODE_SCALAR<char*>*> string_nodes_list;

	unsigned int parameters_count;

	bool src_A_is_assigned;
	bool src_B_is_assigned;

	IMG_DATA * dst_img;
	IMG_DATA * src_img_A;
	IMG_DATA * src_img_B;
	
	unsigned int width_A;
	unsigned int height_A;
	unsigned int width_B;
	unsigned int height_B;

	unsigned int computable_width;
	unsigned int computable_height;

	int ULa_x;
	int ULa_y;
	int LRa_x;
	int LRa_y;

	int ULb_x;
	int ULb_y;
	int LRb_x;
	int LRb_y;

	int ULi_x;
	int ULi_y;

	int ULg_x;
	int ULg_y;

	char image_operation_name[64];

	// Use this function when implementing the copy constructor, and assignation operator (operator =), in derived classes:
	void copyFromImageOperation(const IMAGE_OPERATION& src_image_operation)
	{
		this->inputs_have_changed = src_image_operation.inputs_have_changed;

		this->parameters_have_changed = src_image_operation.parameters_have_changed;

		this->input_numeric_nodes_required = src_image_operation.input_numeric_nodes_required;
		this->input_string_nodes_required = src_image_operation.input_string_nodes_required;

		this->numeric_nodes_names_list = src_image_operation.numeric_nodes_names_list;
		this->numeric_node_is_local_list = src_image_operation.numeric_node_is_local_list;

		this->numeric_nodes_list = src_image_operation.numeric_nodes_list;

		const unsigned int numeric_nodes_count = this->local_numeric_nodes_list.getNodesCount();
		for (unsigned int node_index = 0; node_index < numeric_nodes_count; node_index++)
		{
			// Copy the current numeric value to the local list:
			NODE_SCALAR<double> new_numeric_node(this->numeric_nodes_list.getNodeValue(node_index)->getScalarValue());
			this->local_numeric_nodes_list.assignNodeValue(node_index, new_numeric_node);

			// Verify if the source list is linked to an external node, or to its local node:
			if (this->numeric_node_is_local_list.getNodeValue(node_index).getScalarValue())
			{
				this->numeric_nodes_list.assignNodeValue(node_index, &this->local_numeric_nodes_list.getNodeValue(node_index));
			}
		}

		this->string_nodes_names_list = src_image_operation.string_nodes_names_list;
		this->string_nodes_list = src_image_operation.string_nodes_list;

		const unsigned int string_nodes_count = this->string_nodes_list.getNodesCount();
		for (unsigned int node_index = 0; node_index < string_nodes_count; node_index++)
		{
			// Copy the current numeric value to the local list:
			NODE_SCALAR<char*> new_string_node(this->string_nodes_list.getNodeValue(node_index)->getScalarValue());
			this->local_string_nodes_list.assignNodeValue(node_index, new_string_node);

			// Verify if the source list is linked to an external node, or to its local node:
			if (this->string_node_is_local_list.getNodeValue(node_index).getScalarValue())
			{
				this->string_nodes_list.assignNodeValue(node_index, &this->local_string_nodes_list.getNodeValue(node_index));
			}
		}

		this->operation_A = src_image_operation.operation_A;
		this->operation_B = src_image_operation.operation_B;
		
		if (this->inputs_have_changed)
		{
			copyImageData(src_image_operation.dst_img, this->dst_img);
		}

		this->src_img_A = this->operation_A->getImageData();
		this->src_img_B = this->operation_B->getImageData();

		this->src_A_is_assigned = src_image_operation.src_A_is_assigned;
		this->src_B_is_assigned = src_image_operation.src_B_is_assigned;

		this->parameters_count = src_image_operation.parameters_count;
		
		this->width_A = src_image_operation.width_A;
		this->height_A = src_image_operation.height_A;

		this->width_B = src_image_operation.width_B;
		this->height_B = src_image_operation.height_B;

		this->computable_width = src_image_operation.computable_width;
		this->computable_height = src_image_operation.computable_height;

		this->ULa_x = src_image_operation.ULa_x;
		this->ULa_y = src_image_operation.ULa_y;
		this->LRa_x = src_image_operation.LRa_x;
		this->LRa_y = src_image_operation.LRa_y;

		this->ULb_x = src_image_operation.ULb_x;
		this->ULb_y = src_image_operation.ULb_y;
		this->LRb_x = src_image_operation.LRb_x;
		this->LRb_y = src_image_operation.LRb_y;

		this->ULi_x = src_image_operation.ULi_x;
		this->ULi_y = src_image_operation.ULi_y;

		this->ULg_x = src_image_operation.ULg_x;
		this->ULg_y = src_image_operation.ULg_y;

		sprintf(this->image_operation_name, "%s-copy", src_image_operation.image_operation_name);
	}

	// Each derived class must define this function:
	virtual void performOperation() = 0;
	
	void computeActiveROI()
	{
		// Compute the sum of the intersection of the both images:
		POSITION_NODE * x_posititons_root = NULL;
		POSITION_NODE * y_posititons_root = NULL;

		// Compute global computing area:
		if (src_A_is_assigned && src_B_is_assigned)
		{
			ULg_x = (ULa_x < ULb_x) ? ULa_x : ULb_x;
			ULg_y = (ULa_y < ULb_y) ? ULa_y : ULb_y;
			ULi_x = (ULa_x > ULb_x) ? ULa_x : ULb_x;
			ULi_y = (ULa_y > ULb_y) ? ULa_y : ULb_y;

			x_posititons_root = newPositionLeaf(ULa_x);
			y_posititons_root = newPositionLeaf(ULa_y);
			addPositionLeaf(x_posititons_root, LRa_x);
			addPositionLeaf(y_posititons_root, LRa_y);

			addPositionLeaf(x_posititons_root, ULb_x);
			addPositionLeaf(y_posititons_root, ULb_y);
			addPositionLeaf(x_posititons_root, LRb_x);
			addPositionLeaf(y_posititons_root, LRb_y);
		}
		else if (src_A_is_assigned && !src_B_is_assigned)
		{
			ULg_x = ULa_x;
			ULg_y = ULa_y;

			ULi_x = ULa_x;
			ULi_y = ULa_y;

			x_posititons_root = newPositionLeaf(ULa_x);
			y_posititons_root = newPositionLeaf(ULa_y);
			addPositionLeaf(x_posititons_root, LRa_x);
			addPositionLeaf(y_posititons_root, LRa_y);
		}
		else
		{
			ULg_x = ULb_x;
			ULg_y = ULb_y;

			ULi_x = ULb_x;
			ULi_y = ULb_y;

			x_posititons_root = newPositionLeaf(ULb_x);
			y_posititons_root = newPositionLeaf(ULb_y);
			addPositionLeaf(x_posititons_root, LRb_x);
			addPositionLeaf(y_posititons_root, LRb_y);
		}


		const unsigned int x_positions_count = x_posititons_root->tree_depth;
		const unsigned int y_positions_count = y_posititons_root->tree_depth;

		int * x_positions = dumpPositionsTree(x_posititons_root);
		int * y_positions = dumpPositionsTree(y_posititons_root);

		freePositionsTree(x_posititons_root);
		freePositionsTree(y_posititons_root);

		computable_width = *(x_positions + x_positions_count - 1) - *(x_positions) + 1;
		computable_height = *(y_positions + y_positions_count - 1) - *(y_positions) + 1;

		if (!dst_img)
		{
			dst_img = createVoidImage(computable_width, computable_height);
		}

		if ((dst_img->width != computable_width) || (dst_img->height != computable_height))
		{
			if (dst_img->image_data)
			{
				free(dst_img->image_data);
			}
			dst_img->width = computable_width;
			dst_img->height = computable_height;
			dst_img->image_data = (double*)calloc(computable_width * computable_height, sizeof(double));
		}


		if (dst_img->head_roi.next_roi)
		{
			ROI_BBOX * next_roi_node = dst_img->head_roi.next_roi;
			ROI_BBOX * current_roi_node;

			while (next_roi_node)
			{
				current_roi_node = next_roi_node;
				next_roi_node = current_roi_node->next_roi;

				free(current_roi_node);
			}

			dst_img->head_roi.next_roi = NULL;
			dst_img->tail_roi = &dst_img->head_roi;
		}

		dst_img->head_roi.ROI_type = RBT_AREA;
		dst_img->head_roi.UL_x = ULg_x;
		dst_img->head_roi.UL_y = ULg_y;
		dst_img->head_roi.UR_x = ULg_x + computable_width - 1;
		dst_img->head_roi.UR_y = ULg_y;
		dst_img->head_roi.LR_x = ULg_x + computable_width - 1;
		dst_img->head_roi.LR_y = ULg_y + computable_height - 1;
		dst_img->head_roi.LL_x = ULg_x;
		dst_img->head_roi.LL_y = ULg_y + computable_height - 1;

		// Compute the ROIs:
		for (unsigned int x_positions_index = 0; x_positions_index < (x_positions_count - 1); x_positions_index++)
		{
			for (unsigned int y_positions_index = 0; y_positions_index < (y_positions_count - 1); y_positions_index++)
			{
				const int ULroi_x = *(x_positions + x_positions_index);
				const int ULroi_y = *(y_positions + y_positions_index);
				const int URroi_x = *(x_positions + x_positions_index + 1);
				const int URroi_y = *(y_positions + y_positions_index);
				const int LRroi_x = *(x_positions + x_positions_index + 1);
				const int LRroi_y = *(y_positions + y_positions_index + 1);
				const int LLroi_x = *(x_positions + x_positions_index);
				const int LLroi_y = *(y_positions + y_positions_index + 1);

				const int center_x = (int)floor((double)(ULroi_x + LRroi_x) / 2.0);
				const int center_y = (int)floor((double)(ULroi_y + LRroi_y) / 2.0);

				int current_roi_type_int = (int)RBT_UNCOMPUTED;
				
				// Verify if the current roi is inside the source image:
				if (src_A_is_assigned && ULa_x < center_x && ULa_y < center_y && center_x < LRa_x && center_y < LRa_y)
				{
					current_roi_type_int += (int)RBT_SOURCE;
				}

				// Verify if the current roi is inside the target image:
				if (src_B_is_assigned && ULb_x < center_x && ULb_y < center_y && center_x < LRb_x && center_y < LRb_y)
				{
					current_roi_type_int += (int)RBT_TARGET;
				}

				addImageROI(dst_img,
					(ROI_BBOX_TYPE)current_roi_type_int,
					ULroi_x, ULroi_y,
					URroi_x, URroi_y,
					LRroi_x, LRroi_y,
					LLroi_x, LLroi_y);
			}
		}

		free(x_positions);
		free(y_positions);
	}


private:
	bool inputs_have_changed;
	bool parameters_have_changed;

	IMAGE_OPERATION * operation_A;
	IMAGE_OPERATION * operation_B;
};

#endif // IMAGE_OPERATION_CLASS_H_INCLUDED
