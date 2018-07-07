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

	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);


	/* ----------------------------- Test generating a simple triangle -------------------------------- */
	NODE_FIGURE node_1;
	node_1.moveFigure(7.5, 1.0);

	NODE_FIGURE node_2;
	
	FIGURE_RENDERER renderer;
	renderer.addFigure(node_1);
	renderer.addFigure(node_2);



	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(renderer.getVerticesPositions()), renderer.getVerticesPositions(), GL_STATIC_DRAW);

	char vertex_shader_string[] =
		"#version 150\n\
in vec2 position;\n\
void main()\n\
{\n\
gl_Position = vec4(position, 0.0, 1.0);\n\
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
		"#version 150\n\
out vec4 outColor;\n\
void main()\n\
{\n\
outColor = vec4(1.0, 0.7, 0.3, 1.0);\n\
}\n";

	char const *fragment_shader = fragment_shader_string;
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

	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);


	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glfwTerminate();
}
