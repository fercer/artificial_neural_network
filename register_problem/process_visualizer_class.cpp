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


void PROCESS_VISUALIZER::addNodeOperation(NODE_SCALAR<double> * src_numeric_node) 
{
	std::vector<FIGURE_2D*>::iterator graphic_node_ptr = src_numeric_node->getGraphicNode();
	for (unsigned int node_index = 0; node_index < src_numeric_node->getGraphicNodeCount(); node_index++)
	{
		(*graphic_node_ptr)->translateFigure(5.0f, 0.0f);
		global_renderer.addFigure(*(graphic_node_ptr++));
	}
}


void PROCESS_VISUALIZER::addNodeOperation(NODE_SCALAR<char*> * src_string_node)
{
	std::vector<FIGURE_2D*>::iterator graphic_node_ptr = src_string_node->getGraphicNode();
	for (unsigned int node_index = 0; node_index < src_string_node->getGraphicNodeCount(); node_index++)
	{
		global_renderer.addFigure(*(graphic_node_ptr++));
	}
}


void PROCESS_VISUALIZER::showProcess()
{
	initializeGraphicEnvironment();
}


GLuint PROCESS_VISUALIZER::loadShaderProgram(const char * src_vertex_shader_filename, const char * src_fragment_shader_filename)
{
	FILE * vertex_shader_file = fopen(src_vertex_shader_filename, "rb");
	if (!vertex_shader_file)
	{
		fprintf(stderr, "<<Error: The vertex shader file \'%s\' could not be opened>>\n", src_vertex_shader_filename);
		return 0;
	}

	fseek(vertex_shader_file, 0L, SEEK_END);
	long file_size = ftell(vertex_shader_file);

	char * vertex_shader_string = (char*)calloc(file_size + 1, sizeof(char));
	rewind(vertex_shader_file);
	fread(vertex_shader_string, sizeof(char), file_size, vertex_shader_file);
	fclose(vertex_shader_file);

	char const * vertex_shader = vertex_shader_string;
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_shader, NULL);
	glCompileShader(vertexShader);

	GLint shader_status;
	int log_length;
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &shader_status);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &log_length);

	if (shader_status != GL_TRUE)
	{
		char * shader_compile_error = (char*)calloc(log_length + 1, sizeof(char));
		glGetShaderInfoLog(vertexShader, log_length, NULL, shader_compile_error);
		fprintf(stderr, "<<Error: Error while compiling the vertex shader, the following error was launched:\n\'%s\'\n>>\n", shader_compile_error);
		free(shader_compile_error);

		return 0;
	}

	FILE * fragment_shader_file = fopen(src_fragment_shader_filename, "rb");
	if (!fragment_shader_file)
	{
		fprintf(stderr, "<<Error: The fragment shader file \'%s\' could not be opened>>\n", src_fragment_shader_filename);
		free(vertex_shader_string);

		return 0;
	}

	fseek(fragment_shader_file, 0L, SEEK_END);
	file_size = ftell(fragment_shader_file);

	char * fragment_shader_string = (char*)calloc(file_size + 1, sizeof(char));
	rewind(fragment_shader_file);
	fread(fragment_shader_string, sizeof(char), file_size, fragment_shader_file);
	fclose(fragment_shader_file);

	char const * fragment_shader = fragment_shader_string;
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &shader_status);
	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &log_length);

	if (shader_status != GL_TRUE)
	{
		char * shader_compile_error = (char*)calloc(log_length + 1, sizeof(char));
		glGetShaderInfoLog(fragmentShader, log_length, NULL, shader_compile_error);
		fprintf(stderr, "<<Error: Error while compiling the fragment shader, the following error was launched:\n\'%s\'\n>>\n", shader_compile_error);
		free(shader_compile_error);
		free(vertex_shader_string);
		free(fragment_shader_string);

		return 0;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &shader_status);
	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &log_length);
	if (shader_status != GL_TRUE) {
		char * shader_compile_error = (char*)calloc(log_length + 1, sizeof(char));

		glGetProgramInfoLog(shaderProgram, log_length, NULL, shader_compile_error);
		fprintf(stderr, "<<Error: Error while compiling the shader progam, the following error was launched:\n\'%s\'\n>>\n", shader_compile_error);
		free(shader_compile_error);
		free(vertex_shader_string);
		free(fragment_shader_string);

		return 0;
	}

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	free(vertex_shader_string);
	free(fragment_shader_string);

	return shaderProgram;
}


GLuint PROCESS_VISUALIZER::loadTextures(IMG_DATA * src_textures, const unsigned int src_color_channels)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, src_textures->width / src_color_channels, src_textures->height, 0, GL_RGB, GL_UNSIGNED_BYTE, src_textures->image_data.unsigned_character_image_data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	return textureID;
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

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.2f, 0.15f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);


	GLuint vertex_array_id;
	glGenVertexArrays(1, &vertex_array_id);
	glBindVertexArray(vertex_array_id);

	GLuint shaderProgram = loadShaderProgram("D:/Apps/artificial_neural_network/register_problem/shaders/VertexShader.vertexshader", "D:/Apps/artificial_neural_network/register_problem/shaders/FragmentShader.fragmentshader");
	
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
	
	GLuint texture_data = loadTextures(global_renderer.getTextureData(), global_renderer.getTextureColorChannels());
	GLuint TextureID = glGetUniformLocation(shaderProgram, "myTextureSampler");


	GLuint nodes_positions_buffer;
	glGenBuffers(1, &nodes_positions_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, nodes_positions_buffer);
	glBufferData(GL_ARRAY_BUFFER, global_renderer.getTriangesCount() * 3 * 3 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	GLuint nodes_colors_buffer;
	glGenBuffers(1, &nodes_colors_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, nodes_colors_buffer);
	glBufferData(GL_ARRAY_BUFFER, global_renderer.getTriangesCount() * 3 * 3 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	GLuint nodes_uv_buffer;
	glGenBuffers(1, &nodes_uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, nodes_uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, global_renderer.getTriangesCount() * 2 * 3 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	double last_time = glfwGetTime();
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&  !glfwWindowShouldClose(window))
	{
		double delta_time = last_time - glfwGetTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		/* Use the shader program: */
		glUseProgram(shaderProgram);

		/* Send the MVP matrix */
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &VP_matrix[0][0]);

		// Set our "myTextureSampler" sampler to use Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_data);
		glUniform1i(TextureID, 0);

		/* Update the vertex positions: */
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, nodes_positions_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, global_renderer.getTriangesCount() * 3 * 3 * sizeof(GLfloat), global_renderer.getVerticesPositions());
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Color values */
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, nodes_colors_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, global_renderer.getTriangesCount() * 3 * 3 * sizeof(GLfloat), global_renderer.getColorValues());
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Color values */
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, nodes_uv_buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, global_renderer.getTriangesCount() * 2 * 3 * sizeof(GLfloat), global_renderer.getUVValues());
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 3 * global_renderer.getTriangesCount());
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &nodes_positions_buffer);
	glDeleteBuffers(1, &nodes_colors_buffer);
	glDeleteBuffers(1, &nodes_uv_buffer);
	glDeleteProgram(shaderProgram);
	glDeleteTextures(1, &texture_data);
	glDeleteVertexArrays(1, &vertex_array_id);
	
	glfwTerminate();
}
