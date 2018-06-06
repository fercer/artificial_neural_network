#ifndef IMAGE_OPERATION_CLASS_H_INCLUDED
#define IMAGE_OPERATION_CLASS_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_functions.h"


class IMAGE_OPERATION 
{
public:
	IMAGE_OPERATION()
	{
		inputs_have_changed = false;

		operation_A = NULL;
		operation_B = NULL;
		src_A_is_assigned = false;
		src_B_is_assigned = false;

		dst_img = (IMG_DATA*) malloc(sizeof(IMG_DATA));
		dst_img->head_roi.next_roi = NULL;
		dst_img->head_roi.ROI_type = RBT_AREA;
		dst_img->head_roi.UL_x = 0;
		dst_img->head_roi.UL_y = 0;
		dst_img->head_roi.UR_x = 0;
		dst_img->head_roi.UR_y = 0;
		dst_img->head_roi.LR_x = 0;
		dst_img->head_roi.LR_y = 0;
		dst_img->head_roi.LL_x = 0;
		dst_img->head_roi.LL_y = 0;

		dst_img->tail_roi = &dst_img->head_roi;
		dst_img->image_data = NULL;

		src_img_A = NULL;
		src_img_B = NULL;

		parameter_A = 0.0;
		parameter_B = 0.0;
		parameter_C = 0.0;
		parameter_D = 0.0;

		strcpy(image_name, "");
	}

	virtual ~IMAGE_OPERATION() 
	{
		freeImageData(dst_img);
	}
	
	IMG_DATA * getImageData() 
	{
		if (inputs_have_changed)
		{
			if (operation_A)
			{
				src_img_A = operation_A->getImageData();
			}

			if (operation_B)
			{
				src_img_B = operation_B->getImageData();
			}

			performOperation();
			inputs_have_changed = false;
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

	void setParameterA(const double src_parameter_A)
	{
		parameter_A = src_parameter_A;
	}

	void setParameterB(const double src_parameter_B)
	{
		parameter_B = src_parameter_B;
	}

	void setParameterC(const double src_parameter_C)
	{
		parameter_C = src_parameter_C;
	}

	void setParameterD(const double src_parameter_D)
	{
		parameter_D = src_parameter_D;
	}

	void setImageName(const char * src_image_name)
	{
		strcpy(image_name, src_image_name);
	}

protected:
	double parameter_A;
	double parameter_B;
	double parameter_C;
	double parameter_D;
	
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

	int ULb_x;
	int ULb_y;

	int ULg_x;
	int ULg_y;

	char image_name[1024];

	// Use this function when implementing the copy constructor, and assignation operator (operator =), in derived classes:
	void copyFromImageOperation(const IMAGE_OPERATION& src_image_operation)
	{
		this->inputs_have_changed = src_image_operation.inputs_have_changed;

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

		this->parameter_A = src_image_operation.parameter_A;
		this->parameter_B = src_image_operation.parameter_B;
		this->parameter_C = src_image_operation.parameter_C;
		this->parameter_D = src_image_operation.parameter_D;

		this->width_A = src_image_operation.width_A;
		this->height_A = src_image_operation.height_A;

		this->width_B = src_image_operation.width_B;
		this->height_B = src_image_operation.height_B;

		this->computable_width = src_image_operation.computable_width;
		this->computable_height = src_image_operation.computable_height;

		this->ULa_x = src_image_operation.ULa_x;
		this->ULa_y = src_image_operation.ULa_y;

		this->ULb_x = src_image_operation.ULb_x;
		this->ULb_y = src_image_operation.ULb_y;

		this->ULg_x = src_image_operation.ULg_x;
		this->ULg_y = src_image_operation.ULg_y;

		strcpy(this->image_name, src_image_operation.image_name);
	}

	// Each derived class must define this function:
	virtual void performOperation() = 0;
	
	void computeActiveROI()
	{
		// Compute the sum of the intersection of the both images:
		int LRa_x = 0;
		int LRa_y = 0;

		if (src_A_is_assigned)
		{
			ULa_x = src_img_A->head_roi.UL_x;
			ULa_y = src_img_A->head_roi.UL_y;
			LRa_x = src_img_A->head_roi.LR_x;
			LRa_y = src_img_A->head_roi.LR_y;

			width_A = src_img_A->width;
			height_A = src_img_A->height;
		}

		int LRb_x = 0;
		int LRb_y = 0;
		if (src_B_is_assigned)
		{
			width_B = src_img_B->width;
			height_B = src_img_B->height;

			ULb_x = src_img_B->head_roi.UL_x;
			ULb_y = src_img_B->head_roi.UL_y;

			LRb_x = src_img_B->head_roi.LR_x;
			LRb_y = src_img_B->head_roi.LR_y;
		}

		POSITION_NODE * x_posititons_root = NULL;
		POSITION_NODE * y_posititons_root = NULL;

		// Compute global computing area:
		if (src_A_is_assigned && src_B_is_assigned)
		{
			ULg_x = (ULa_x < ULb_x) ? ULa_x : ULb_x;
			ULg_y = (ULa_y < ULb_y) ? ULa_y : ULb_y;

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

			x_posititons_root = newPositionLeaf(ULa_x);
			y_posititons_root = newPositionLeaf(ULa_y);
			addPositionLeaf(x_posititons_root, LRa_x);
			addPositionLeaf(y_posititons_root, LRa_y);
		}
		else
		{
			ULg_x = ULb_x;
			ULg_y = ULb_y;

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

		computable_width = *(x_positions + x_positions_count - 1) - *(x_positions);
		computable_height = *(y_positions + y_positions_count - 1) - *(y_positions);

		if (dst_img->image_data)
		{
			if ((dst_img->width != computable_width) || (dst_img->height != computable_height))
			{
				free(dst_img->image_data);

				dst_img->width = computable_width;
				dst_img->height = computable_height;

				dst_img->image_data = (double*)calloc(computable_width * computable_height, sizeof(double));
			}
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

			dst_img->tail_roi = &dst_img->head_roi;
		}

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
					ULroi_x - *(x_positions),
					ULroi_y - *(y_positions),
					URroi_x - *(x_positions),
					URroi_y - *(y_positions),
					LRroi_x - *(x_positions),
					LRroi_y - *(y_positions),
					LLroi_x - *(x_positions),
					LLroi_y - *(y_positions));
			}
		}

		free(x_positions);
		free(y_positions);
	}


private:
	bool inputs_have_changed;

	IMAGE_OPERATION * operation_A;
	IMAGE_OPERATION * operation_B;
};

#endif // IMAGE_OPERATION_CLASS_H_INCLUDED
