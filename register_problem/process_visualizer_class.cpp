#include "process_visualizer_class.h"



PROCESS_VISUALIZER::PROCESS_VISUALIZER()
{
}



PROCESS_VISUALIZER::PROCESS_VISUALIZER(const PROCESS_VISUALIZER & src_xml_handler)
{
}



PROCESS_VISUALIZER PROCESS_VISUALIZER::operator=(const PROCESS_VISUALIZER & src_xml_handler)
{
	return *this;
}



PROCESS_VISUALIZER::~PROCESS_VISUALIZER()
{
}

void PROCESS_VISUALIZER::showProcess()
{
	initializeGraphicEnvironment();
}



void PROCESS_VISUALIZER::initializeGraphicEnvironment()
{

	NODE_FIGURE node_1;
	node_1.moveFigure(1.5, 2.0);

	NODE_FIGURE node_2;
	node_2.moveFigure(2.0, 1.5);

	FIGURE_RENDERER my_renderer;
	my_renderer.addFigure(&node_1);
	my_renderer.addFigure(&node_2);
	

	if (!glfwInit())
	{
		fprintf(stderr, "<<Error: The OpenGL environment (GLFW) could not be initialized>>\n");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		fprintf(stderr, "<<Error: Could not create a window>>\n");
		return;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum glew_is_intialized = glewInit();
	if(glew_is_intialized != GLEW_OK)
	{
		glfwTerminate();
		fprintf(stderr, "<<Error: The OpenGL environment (GLEW) could not be initialized>>\n");
		return;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.2f, 0.15f, 1.0f);
	
	GLuint vertex_array_id;
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);

	char vertex_shader_string[] =
		"#version 330 core\n\
layout(location = 0) in vec3 vertexPosition;\n\
layout(location = 1) in vec3 vertexColor;\n\
out vec3 triangleColor;\n\
uniform mat4 MVP;\n\
void main()\n\
{\n\
gl_Position = MVP * vec4(vertexPosition, 1.0f);\n\
triangleColor = vertexColor;\n\
}\n";
	
	char const * vertex_shader = vertex_shader_string;
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_shader, NULL);
	glCompileShader(vertexShader);

	GLint shader_status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shader_status);

	if (shader_status != GL_TRUE)
	{
		char shader_compile_error[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, shader_compile_error);
		fprintf(stderr, "<<Error: Error while compiling the vertex shader, the following error was launched:\n\'%s\'\n>>\n", shader_compile_error);
		glfwTerminate();
		return;
	}

	char fragment_shader_string[] =
		"#version 330 core\n\
in vec3 triangleColor; \n\
out vec3 outColor;\n\
void main()\n\
{\n\
outColor = triangleColor;\n\
}\n";

	char const * fragment_shader = fragment_shader_string;
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shader_status);

	if (shader_status != GL_TRUE)
	{
		char shader_compile_error[512];
		glGetShaderInfoLog(fragmentShader, 512, NULL, shader_compile_error);
		fprintf(stderr, "<<Error: Error while compiling the fragment shader, the following error was launched:\n\'%s\'\n>>\n", shader_compile_error);
		glfwTerminate();
		return;
	}
	

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	GLuint MatrixID = glGetUniformLocation(shaderProgram, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	//glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.01f, 100.0f);

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 10), // Camera is at (0, 0, 3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glm::mat4 VP_matrix = Projection * View;


	GLuint nodes_positions_buffer;
	glGenBuffers(1, &nodes_positions_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, nodes_positions_buffer);
	glBufferData(GL_ARRAY_BUFFER, my_renderer.getTriangesCount() * 3 * 3 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	GLuint nodes_colors_buffer;
	glGenBuffers(1, &nodes_colors_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, nodes_colors_buffer);
	glBufferData(GL_ARRAY_BUFFER, my_renderer.getTriangesCount() * 3 * 3 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	double last_time = glfwGetTime();
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&  !glfwWindowShouldClose(window))
	{
		double delta_time = last_time - glfwGetTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		/* Use the shader program: */
		glUseProgram(shaderProgram);

		/* Send the MVP matrix */
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &VP_matrix[0][0]);

		/* Update the vertex positions: */
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, nodes_positions_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, my_renderer.getTriangesCount() * 3 * 3 * sizeof(GLfloat), my_renderer.getVerticesPositions());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Color values */
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, nodes_colors_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, my_renderer.getTriangesCount() * 3 * 3 * sizeof(GLfloat), my_renderer.getColorValues());
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
		glDrawArrays(GL_TRIANGLES, 0, 3 * my_renderer.getTriangesCount());
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &nodes_positions_buffer);
	glDeleteBuffers(1, &nodes_colors_buffer);
	glDeleteProgram(shaderProgram);
	glDeleteVertexArrays(1, &vertex_array_id);
	
	glfwTerminate();
}
