#include "node_figure_class.h"

NODE_FIGURE::NODE_FIGURE()
{
	triangles_count = 8;
	vertices_positions = (glm::vec3*)malloc(3 * triangles_count * sizeof(glm::vec3));
	color_values = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));
	uv_values = (GLfloat*)malloc(2 * 3 * triangles_count * sizeof(GLfloat));
	normal_vectors = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));

	initializeVerticesPositions();
	initializeColorValues();
	initializeUVValues();
}



NODE_FIGURE::NODE_FIGURE(const NODE_FIGURE& src_node_figure)
{
	triangles_count = 8;
	vertices_positions = (glm::vec3*)malloc(3 * triangles_count * sizeof(glm::vec3));
	color_values = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));
	uv_values = (GLfloat*)malloc(2 * 3 * triangles_count * sizeof(GLfloat));
	normal_vectors = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));

	copyFigure2d(src_node_figure);
}



NODE_FIGURE NODE_FIGURE::operator=(const NODE_FIGURE& src_node_figure)
{
	if (this != &src_node_figure)
	{
		copyFigure2d(src_node_figure);
	}

	return *this;
}



NODE_FIGURE::~NODE_FIGURE()
{
}



void NODE_FIGURE::initializeVerticesPositions()
{
	glm::vec3 * vertices_positions_ptr = vertices_positions;

	/* Logo to identify the node type: */
	(vertices_positions_ptr)->x = -2.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -2.5f;
	(vertices_positions_ptr)->y = 2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -1.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -2.5f;
	(vertices_positions_ptr)->y = 2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -1.5f;
	(vertices_positions_ptr)->y = 2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -1.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	/* Node's name background: */
	(vertices_positions_ptr)->x = -1.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -1.5f;
	(vertices_positions_ptr)->y = 2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = 2.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -1.5f;
	(vertices_positions_ptr)->y = 2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = 2.5f;
	(vertices_positions_ptr)->y = 2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = 2.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;


	/* Node canvas: */
	(vertices_positions_ptr)->x = -2.5f;
	(vertices_positions_ptr)->y = -2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -2.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = 2.5f;
	(vertices_positions_ptr)->y = -2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = -2.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = 2.5f;
	(vertices_positions_ptr)->y = 1.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	(vertices_positions_ptr)->x = 2.5f;
	(vertices_positions_ptr)->y = -2.0f;
	(vertices_positions_ptr++)->z = 0.0f;

	/* Node Value: */
	(vertices_positions_ptr)->x = -2.25f;
	(vertices_positions_ptr)->y = -1.5f;
	(vertices_positions_ptr++)->z = 0.1f;

	(vertices_positions_ptr)->x = -2.25f;
	(vertices_positions_ptr)->y = -0.5f;
	(vertices_positions_ptr++)->z = 0.1f;

	(vertices_positions_ptr)->x = 2.25f;
	(vertices_positions_ptr)->y = -1.5f;
	(vertices_positions_ptr++)->z = 0.1f;

	(vertices_positions_ptr)->x = -2.25f;
	(vertices_positions_ptr)->y = -0.5f;
	(vertices_positions_ptr++)->z = 0.1f;

	(vertices_positions_ptr)->x = 2.25f;
	(vertices_positions_ptr)->y = -0.5f;
	(vertices_positions_ptr++)->z = 0.1f;

	(vertices_positions_ptr)->x = 2.25f;
	(vertices_positions_ptr)->y = -1.5f;
	(vertices_positions_ptr++)->z = 0.1f;
}



void NODE_FIGURE::initializeColorValues()
{
	GLfloat * color_values_ptr = color_values;

	/* Logo to identify the node type: */
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	/* Node's name background: */
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 111.0f / 255.0f;
	*(color_values_ptr++) = 145.0f / 255.0f;

	/* Node canvas */
	*(color_values_ptr++) = 147.0f / 255.0f;
	*(color_values_ptr++) = 157.0f / 255.0f;
	*(color_values_ptr++) = 172.0f / 255.0f;

	*(color_values_ptr++) = 147.0f / 255.0f;
	*(color_values_ptr++) = 157.0f / 255.0f;
	*(color_values_ptr++) = 172.0f / 255.0f;

	*(color_values_ptr++) = 147.0f / 255.0f;
	*(color_values_ptr++) = 157.0f / 255.0f;
	*(color_values_ptr++) = 172.0f / 255.0f;

	*(color_values_ptr++) = 147.0f / 255.0f;
	*(color_values_ptr++) = 157.0f / 255.0f;
	*(color_values_ptr++) = 172.0f / 255.0f;

	*(color_values_ptr++) = 147.0f / 255.0f;
	*(color_values_ptr++) = 157.0f / 255.0f;
	*(color_values_ptr++) = 172.0f / 255.0f;

	*(color_values_ptr++) = 147.0f / 255.0f;
	*(color_values_ptr++) = 157.0f / 255.0f;
	*(color_values_ptr++) = 172.0f / 255.0f;

	/* Value background */
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
	*(color_values_ptr++) = 1.0f;
}

void NODE_FIGURE::initializeUVValues()
{
	GLfloat * uv_values_ptr = uv_values;

	const GLfloat icon_vertex_A_x = 0.0f;
	const GLfloat icon_vertex_A_y = 1.0f;

	const GLfloat icon_vertex_B_x = 0.0f;
	const GLfloat icon_vertex_B_y = 0.75f;

	const GLfloat icon_vertex_C_x = 0.5f;
	const GLfloat icon_vertex_C_y = 1.0f;
	
	const GLfloat icon_vertex_D_x = 0.5f;
	const GLfloat icon_vertex_D_y = 0.75f;
	
	// Reduce the size of the mapped triangle to fit inside the texture:
	const GLfloat icon_center_ABC_x = (icon_vertex_A_x + icon_vertex_B_x + icon_vertex_C_x) / 3.0f;
	const GLfloat icon_center_ABC_y = (icon_vertex_A_y + icon_vertex_B_y + icon_vertex_C_y) / 3.0f;

	const GLfloat icon_center_BDC_x = (icon_vertex_B_x + icon_vertex_D_x + icon_vertex_C_x) / 3.0f;
	const GLfloat icon_center_BDC_y = (icon_vertex_B_y + icon_vertex_D_y + icon_vertex_C_y) / 3.0f;

	const GLfloat vertex_Ap_x = (icon_vertex_A_x - icon_center_ABC_x) * 0.9 + icon_center_ABC_x;
	const GLfloat vertex_Ap_y = (icon_vertex_A_y - icon_center_ABC_y) * 0.9 + icon_center_ABC_y;

	const GLfloat vertex_Bp_x = (icon_vertex_B_x - icon_center_ABC_x) * 0.9 + icon_center_ABC_x;
	const GLfloat vertex_Bp_y = (icon_vertex_B_y - icon_center_ABC_y) * 0.9 + icon_center_ABC_y;

	const GLfloat vertex_Cp_x = (icon_vertex_C_x - icon_center_ABC_x) * 0.9 + icon_center_ABC_x;
	const GLfloat vertex_Cp_y = (icon_vertex_C_y - icon_center_ABC_y) * 0.9 + icon_center_ABC_y;

	const GLfloat vertex_Dp_x = (icon_vertex_D_x - icon_center_ABC_x) * 0.9 + icon_center_ABC_x;
	const GLfloat vertex_Dp_y = (icon_vertex_D_y - icon_center_ABC_y) * 0.9 + icon_center_ABC_y;

	/* Logo to identify the node type: */
	*(uv_values_ptr++) = vertex_Ap_x;
	*(uv_values_ptr++) = vertex_Ap_y;

	*(uv_values_ptr++) = vertex_Bp_x;
	*(uv_values_ptr++) = vertex_Bp_y;

	*(uv_values_ptr++) = vertex_Cp_x;
	*(uv_values_ptr++) = vertex_Cp_y;

	*(uv_values_ptr++) = vertex_Bp_x;
	*(uv_values_ptr++) = vertex_Bp_y;

	*(uv_values_ptr++) = vertex_Dp_x;
	*(uv_values_ptr++) = vertex_Dp_y;

	*(uv_values_ptr++) = vertex_Cp_x;
	*(uv_values_ptr++) = vertex_Cp_y;

	/* Node's name background: */
	const GLfloat bg_vertex_A_x = 0.0f;
	const GLfloat bg_vertex_A_y = 0.5f;

	const GLfloat bg_vertex_B_x = 0.0f;
	const GLfloat bg_vertex_B_y = 0.75f;

	const GLfloat bg_vertex_C_x = 0.5f;
	const GLfloat bg_vertex_C_y = 0.5f;

	const GLfloat bg_center_ABC_x = (bg_vertex_A_x + bg_vertex_B_x + bg_vertex_C_x) / 3.0f;
	const GLfloat bg_center_ABC_y = (bg_vertex_A_y + bg_vertex_B_y + bg_vertex_C_y) / 3.0f;

	const GLfloat bg_vertex_Ap_x = (bg_vertex_A_x - bg_center_ABC_x) * 0.9 + bg_center_ABC_x;
	const GLfloat bg_vertex_Ap_y = (bg_vertex_A_y - bg_center_ABC_y) * 0.9 + bg_center_ABC_y;

	const GLfloat bg_vertex_Bp_x = (bg_vertex_B_x - bg_center_ABC_x) * 0.9 + bg_center_ABC_x;
	const GLfloat bg_vertex_Bp_y = (bg_vertex_B_y - bg_center_ABC_y) * 0.9 + bg_center_ABC_y;

	const GLfloat bg_vertex_Cp_x = (bg_vertex_C_x - bg_center_ABC_x) * 0.9 + bg_center_ABC_x;
	const GLfloat bg_vertex_Cp_y = (bg_vertex_C_y - bg_center_ABC_y) * 0.9 + bg_center_ABC_y;

	*(uv_values_ptr++) = bg_vertex_Ap_x;
	*(uv_values_ptr++) = bg_vertex_Ap_y;

	*(uv_values_ptr++) = bg_vertex_Bp_x;
	*(uv_values_ptr++) = bg_vertex_Bp_y;

	*(uv_values_ptr++) = bg_vertex_Cp_x;
	*(uv_values_ptr++) = bg_vertex_Cp_y;

	*(uv_values_ptr++) = bg_vertex_Ap_x;
	*(uv_values_ptr++) = bg_vertex_Ap_y;

	*(uv_values_ptr++) = bg_vertex_Bp_x;
	*(uv_values_ptr++) = bg_vertex_Bp_y;

	*(uv_values_ptr++) = bg_vertex_Cp_x;
	*(uv_values_ptr++) = bg_vertex_Cp_y;



	/* Node canvas */
	const GLfloat can_vertex_A_x = 0.5f;
	const GLfloat can_vertex_A_y = 0.5f;

	const GLfloat can_vertex_B_x = 0.5f;
	const GLfloat can_vertex_B_y = 0.75f;

	const GLfloat can_vertex_C_x = 1.0f;
	const GLfloat can_vertex_C_y = 0.5f;

	const GLfloat can_center_ABC_x = (can_vertex_A_x + can_vertex_B_x + can_vertex_C_x) / 3.0f;
	const GLfloat can_center_ABC_y = (can_vertex_A_y + can_vertex_B_y + can_vertex_C_y) / 3.0f;

	const GLfloat can_vertex_Ap_x = (can_vertex_A_x - can_center_ABC_x) * 0.9 + can_center_ABC_x;
	const GLfloat can_vertex_Ap_y = (can_vertex_A_y - can_center_ABC_y) * 0.9 + can_center_ABC_y;

	const GLfloat can_vertex_Bp_x = (can_vertex_B_x - can_center_ABC_x) * 0.9 + can_center_ABC_x;
	const GLfloat can_vertex_Bp_y = (can_vertex_B_y - can_center_ABC_y) * 0.9 + can_center_ABC_y;

	const GLfloat can_vertex_Cp_x = (can_vertex_C_x - can_center_ABC_x) * 0.9 + can_center_ABC_x;
	const GLfloat can_vertex_Cp_y = (can_vertex_C_y - can_center_ABC_y) * 0.9 + can_center_ABC_y;

	*(uv_values_ptr++) = can_center_ABC_x;
	*(uv_values_ptr++) = can_center_ABC_y;

	*(uv_values_ptr++) = can_vertex_Bp_x;
	*(uv_values_ptr++) = can_vertex_Bp_y;

	*(uv_values_ptr++) = can_vertex_Cp_x;
	*(uv_values_ptr++) = can_vertex_Cp_y;

	*(uv_values_ptr++) = can_center_ABC_x;
	*(uv_values_ptr++) = can_center_ABC_y;

	*(uv_values_ptr++) = can_vertex_Bp_x;
	*(uv_values_ptr++) = can_vertex_Bp_y;

	*(uv_values_ptr++) = can_vertex_Cp_x;
	*(uv_values_ptr++) = can_vertex_Cp_y;

	/* Value background */
	const GLfloat vgb_vertex_A_x = 1.0f;
	const GLfloat vgb_vertex_A_y = 0.75f;

	const GLfloat vgb_vertex_B_x = 0.5f;
	const GLfloat vgb_vertex_B_y = 0.75f;

	const GLfloat vgb_vertex_C_x = 1.0f;
	const GLfloat vgb_vertex_C_y = 0.5f;

	const GLfloat vgb_center_ABC_x = (vgb_vertex_A_x + vgb_vertex_B_x + vgb_vertex_C_x) / 3.0f;
	const GLfloat vgb_center_ABC_y = (vgb_vertex_A_y + vgb_vertex_B_y + vgb_vertex_C_y) / 3.0f;

	const GLfloat vgb_vertex_Ap_x = (vgb_vertex_A_x - vgb_center_ABC_x) * 0.9 + vgb_center_ABC_x;
	const GLfloat vgb_vertex_Ap_y = (vgb_vertex_A_y - vgb_center_ABC_y) * 0.9 + vgb_center_ABC_y;

	const GLfloat vgb_vertex_Bp_x = (vgb_vertex_B_x - vgb_center_ABC_x) * 0.9 + vgb_center_ABC_x;
	const GLfloat vgb_vertex_Bp_y = (vgb_vertex_B_y - vgb_center_ABC_y) * 0.9 + vgb_center_ABC_y;

	const GLfloat vgb_vertex_Cp_x = (vgb_vertex_C_x - vgb_center_ABC_x) * 0.9 + vgb_center_ABC_x;
	const GLfloat vgb_vertex_Cp_y = (vgb_vertex_C_y - vgb_center_ABC_y) * 0.9 + vgb_center_ABC_y;

	*(uv_values_ptr++) = vgb_center_ABC_x;
	*(uv_values_ptr++) = vgb_center_ABC_y;

	*(uv_values_ptr++) = vgb_vertex_Bp_x;
	*(uv_values_ptr++) = vgb_vertex_Bp_y;

	*(uv_values_ptr++) = vgb_vertex_Cp_x;
	*(uv_values_ptr++) = vgb_vertex_Cp_y;

	*(uv_values_ptr++) = vgb_center_ABC_x;
	*(uv_values_ptr++) = vgb_center_ABC_y;

	*(uv_values_ptr++) = vgb_vertex_Bp_x;
	*(uv_values_ptr++) = vgb_vertex_Bp_y;

	*(uv_values_ptr++) = vgb_vertex_Cp_x;
	*(uv_values_ptr++) = vgb_vertex_Cp_y;
}

void NODE_FIGURE::initializeNormalVectors()
{
	GLfloat * normal_vectors_ptr = normal_vectors;

	/* Logo to identify the node type: */
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	/* Node's name background: */
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	/* Node canvas */
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	/* Value background */
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;
}
