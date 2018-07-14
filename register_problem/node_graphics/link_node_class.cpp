#include "link_node_class.h"

LINK_NODE::LINK_NODE()
{
	triangles_count = 16;
	vertices_positions = (glm::vec3*)malloc(3 * triangles_count * sizeof(glm::vec3));
	color_values = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));
	uv_values = (GLfloat*)malloc(2 * 3 * triangles_count * sizeof(GLfloat));
	normal_vectors = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));

	initializeVerticesPositions();
	initializeColorValues();
	initializeUVValues();
}



LINK_NODE::LINK_NODE(const LINK_NODE& src_node_figure)
{
	triangles_count = 16;
	vertices_positions = (glm::vec3*)malloc(3 * triangles_count * sizeof(glm::vec3));
	color_values = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));
	uv_values = (GLfloat*)malloc(2 * 3 * triangles_count * sizeof(GLfloat));
	normal_vectors = (GLfloat*)malloc(3 * 3 * triangles_count * sizeof(GLfloat));

	copyFigure2d(src_node_figure);
}



LINK_NODE LINK_NODE::operator=(const LINK_NODE& src_node_figure)
{
	if (this != &src_node_figure)
	{
		copyFigure2d(src_node_figure);
	}

	return *this;
}



LINK_NODE::~LINK_NODE()
{
}



void LINK_NODE::initializeVerticesPositions()
{
	glm::vec3 * vertices_positions_ptr = vertices_positions;

	const glm::vec3 vertex_A(0.0f, 0.0f, 0.3f);

	const glm::vec3 vertex_B(0.0f, 0.5f, 0.3f);
	
	const glm::vec3 vertex_C(0.191341f, 0.461939f, 0.3f);
	const glm::vec3 vertex_D(0.353535f, 0.353535f, 0.3f);
	const glm::vec3 vertex_E(0.461939f, 0.191341f, 0.3f);

	const glm::vec3 vertex_F(0.5f, 0.0f, 0.3f);

	const glm::vec3 vertex_G(0.461939f, -0.191341f, 0.3f);
	const glm::vec3 vertex_H(0.353535f, -0.353535f, 0.3f);
	const glm::vec3 vertex_I(0.191341f, -0.461939f, 0.3f);
	
	const glm::vec3 vertex_J(0.0f, -0.5f, 0.3f);

	const glm::vec3 vertex_K(-0.191341f, -0.461939f, 0.3f);
	const glm::vec3 vertex_L(-0.353535f, -0.353535f, 0.3f);
	const glm::vec3 vertex_M(-0.461939f, -0.191341f, 0.3f);

	const glm::vec3 vertex_N(-0.5f, 0.0f, 0.3f);
	
	const glm::vec3 vertex_O(-0.461939f, 0.191341f, 0.3f);
	const glm::vec3 vertex_P(-0.353535f, 0.353535f, 0.3f);
	const glm::vec3 vertex_Q(-0.191341f, 0.461939f, 0.3f);

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_B;
	*(vertices_positions_ptr++) = vertex_C;

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_C;
	*(vertices_positions_ptr++) = vertex_D;

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_D;
	*(vertices_positions_ptr++) = vertex_E;

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_E;
	*(vertices_positions_ptr++) = vertex_F;

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_F;
	*(vertices_positions_ptr++) = vertex_G;

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_G;
	*(vertices_positions_ptr++) = vertex_H;

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_H;
	*(vertices_positions_ptr++) = vertex_I;
	
	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_I;
	*(vertices_positions_ptr++) = vertex_J;

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_J;
	*(vertices_positions_ptr++) = vertex_K;

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_K;
	*(vertices_positions_ptr++) = vertex_L;

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_L;
	*(vertices_positions_ptr++) = vertex_M;

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_M;
	*(vertices_positions_ptr++) = vertex_N;

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_N;
	*(vertices_positions_ptr++) = vertex_O;

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_O;
	*(vertices_positions_ptr++) = vertex_P;

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_P;
	*(vertices_positions_ptr++) = vertex_Q;

	*(vertices_positions_ptr++) = vertex_A;
	*(vertices_positions_ptr++) = vertex_Q;
	*(vertices_positions_ptr++) = vertex_B;
}


void LINK_NODE::initializeColorValues()
{
	GLfloat * color_values_ptr = color_values;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;


	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;


	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;


	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;

	*(color_values_ptr++) = 55.0f;
	*(color_values_ptr++) = 171.0f;
	*(color_values_ptr++) = 200.0f;
}


void LINK_NODE::initializeUVValues()
{
	GLfloat * uv_values_ptr = uv_values;

	const GLfloat uv_value_A_x = 0.5f;
	const GLfloat uv_value_A_y = 1.0f - 0.75f;

	const GLfloat uv_value_J_x = 0.497685f;
	const GLfloat uv_value_J_y = 1.0f - 0.502133f;

	const GLfloat uv_value_I_x = 0.687597f;
	const GLfloat uv_value_I_y = 1.0f - 0.520561f;

	const GLfloat uv_value_H_x = 0.848932f;
	const GLfloat uv_value_H_y = 1.0f - 0.573933f;

	const GLfloat uv_value_G_x = 0.957177f;
	const GLfloat uv_value_G_y = 1.0f - 0.654098f;

	const GLfloat uv_value_F_x = 1.0f;
	const GLfloat uv_value_F_y = 1.0f - 0.75f;

	const GLfloat uv_value_E_x = 0.958948f;
	const GLfloat uv_value_E_y = 1.0f - 0.843834f;

	const GLfloat uv_value_D_x = 0.852205f;
	const GLfloat uv_value_D_y = 1.0f - 0.924501f;

	const GLfloat uv_value_C_x = 0.691874f;
	const GLfloat uv_value_C_y = 1.0f - 0.978624f;

	const GLfloat uv_value_B_x = 0.5f;
	const GLfloat uv_value_B_y = 1.0f - 1.0f;
	
	const GLfloat uv_value_Q_x = 0.312403f;
	const GLfloat uv_value_Q_y = 1.0f - 0.979509f;

	const GLfloat uv_value_P_x = 0.151069f;
	const GLfloat uv_value_P_y = 1.0f - 0.926138f;

	const GLfloat uv_value_O_x = 0.042823f;
	const GLfloat uv_value_O_y = 1.0f - 0.845972f;

	const GLfloat uv_value_N_x = 0.0f;
	const GLfloat uv_value_N_y = 1.0f - 0.75f;

	const GLfloat uv_value_M_x = 0.042674f;
	const GLfloat uv_value_M_y = 1.0f - 0.656233f;

	const GLfloat uv_value_L_x = 0.147795f;
	const GLfloat uv_value_L_y = 1.0f - 0.575570f;

	const GLfloat uv_value_K_x = 0.308126f;
	const GLfloat uv_value_K_y = 1.0f - 0.521447f;

	// Triangle 1
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_B_x;
	*(uv_values_ptr++) = uv_value_B_y;

	*(uv_values_ptr++) = uv_value_C_x;
	*(uv_values_ptr++) = uv_value_C_y;

	// Triangle 2
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_C_x;
	*(uv_values_ptr++) = uv_value_C_y;

	*(uv_values_ptr++) = uv_value_D_x;
	*(uv_values_ptr++) = uv_value_D_y;

	// Triangle 3
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_D_x;
	*(uv_values_ptr++) = uv_value_D_y;

	*(uv_values_ptr++) = uv_value_E_x;
	*(uv_values_ptr++) = uv_value_E_y;

	// Triangle 4
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_E_x;
	*(uv_values_ptr++) = uv_value_E_y;

	*(uv_values_ptr++) = uv_value_F_x;
	*(uv_values_ptr++) = uv_value_F_y;

	// Triangle 5
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_F_x;
	*(uv_values_ptr++) = uv_value_F_y;

	*(uv_values_ptr++) = uv_value_G_x;
	*(uv_values_ptr++) = uv_value_G_y;

	// Triangle 6
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_G_x;
	*(uv_values_ptr++) = uv_value_G_y;

	*(uv_values_ptr++) = uv_value_H_x;
	*(uv_values_ptr++) = uv_value_H_y;

	// Triangle 7
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;
	
	*(uv_values_ptr++) = uv_value_H_x;
	*(uv_values_ptr++) = uv_value_H_y;

	*(uv_values_ptr++) = uv_value_I_x;
	*(uv_values_ptr++) = uv_value_I_y;

	// Triangle 8
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_I_x;
	*(uv_values_ptr++) = uv_value_I_y;

	*(uv_values_ptr++) = uv_value_J_x;
	*(uv_values_ptr++) = uv_value_J_y;

	// Triangle 9
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_J_x;
	*(uv_values_ptr++) = uv_value_J_y;

	*(uv_values_ptr++) = uv_value_K_x;
	*(uv_values_ptr++) = uv_value_K_y;

	// Triangle 10
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_K_x;
	*(uv_values_ptr++) = uv_value_K_y;

	*(uv_values_ptr++) = uv_value_L_x;
	*(uv_values_ptr++) = uv_value_L_y;

	// Triangle 11
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_L_x;
	*(uv_values_ptr++) = uv_value_L_y;

	*(uv_values_ptr++) = uv_value_M_x;
	*(uv_values_ptr++) = uv_value_M_y;

	// Triangle 12
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_M_x;
	*(uv_values_ptr++) = uv_value_M_y;

	*(uv_values_ptr++) = uv_value_N_x;
	*(uv_values_ptr++) = uv_value_N_y;

	// Triangle 13
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_N_x;
	*(uv_values_ptr++) = uv_value_N_y;

	*(uv_values_ptr++) = uv_value_O_x;
	*(uv_values_ptr++) = uv_value_O_y;

	// Triangle 14
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_O_x;
	*(uv_values_ptr++) = uv_value_O_y;

	*(uv_values_ptr++) = uv_value_P_x;
	*(uv_values_ptr++) = uv_value_P_y;

	// Triangle 15
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_P_x;
	*(uv_values_ptr++) = uv_value_P_y;

	*(uv_values_ptr++) = uv_value_Q_x;
	*(uv_values_ptr++) = uv_value_Q_y;

	// Triangle 16
	*(uv_values_ptr++) = uv_value_A_x;
	*(uv_values_ptr++) = uv_value_A_y;

	*(uv_values_ptr++) = uv_value_Q_x;
	*(uv_values_ptr++) = uv_value_Q_y;

	*(uv_values_ptr++) = uv_value_B_x;
	*(uv_values_ptr++) = uv_value_B_y;
}


void LINK_NODE::initializeNormalVectors()
{
	GLfloat * normal_vectors_ptr = normal_vectors;

	// Triangle 1
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 2
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 3
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 4
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 5
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 6
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 7
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 8
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 9
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 10
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 11
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 12
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 13
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 14
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 15
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 0.0f;
	*(normal_vectors_ptr++) = 1.0f;

	// Triangle 16
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
