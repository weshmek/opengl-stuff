#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader_reader.h"
#include <stdio.h>
#include <stdlib.h>

float **calculate_octagon_points(void)
{
	

}

static const float triangle_vertices[][7] = {
	{ 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
	{ 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
	{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f}
};

GLuint compile_shaders(void)
{
	const char *vertex_shader_text;
	const char *fragment_shader_text;
	char buffer[512];
	GLuint vertex_shader;
	GLuint fragment_shader;
	GLint status;
	GLint shader_source_length;
	GLint shader_info_log_length;
	GLint program_info_log_length;
	GLint program_num_attached_shaders;
	GLint program_num_active_attributes;
	GLuint program;


	vertex_shader_text = read_shader("assets/vertex_shader.vs");
	printf("vertex_shader_text: %s\n", vertex_shader_text);
	fragment_shader_text = read_shader("assets/fragment_shader.fs");
	printf("fragment_shader_text: %s\n", fragment_shader_text);
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &status);
	glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &shader_info_log_length);
	glGetShaderiv(vertex_shader, GL_SHADER_SOURCE_LENGTH, &shader_source_length);
	glGetShaderInfoLog(vertex_shader, sizeof(buffer), NULL, buffer);
	printf("vertex shader %d:\nCompile Status: %s\nInfo log length: %d\nSource length: %d\n", (int)vertex_shader, status == GL_TRUE ? "GL_TRUE" : "GL_FALSE", (int)shader_info_log_length, (int)shader_source_length);
	if ((int)shader_info_log_length > 0)
		printf("buffer: %s\n", buffer);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &status);
	glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &shader_info_log_length);
	glGetShaderiv(fragment_shader, GL_SHADER_SOURCE_LENGTH, &shader_source_length);
	glGetShaderInfoLog(fragment_shader, sizeof(buffer), NULL, buffer);
	printf("fragment shader %d:\nCompile Status: %s\nInfo log length: %d\nSource length: %d\n", (int)fragment_shader, status == GL_TRUE ? "GL_TRUE" : "GL_FALSE", (int)shader_info_log_length, (int)shader_source_length);
	if ((int)shader_info_log_length > 0)
		printf("buffer: %s\n", buffer);

	
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glBindFragDataLocation(program, 0, "out_colour");

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &program_info_log_length);
	glGetProgramiv(program, GL_ATTACHED_SHADERS, &program_num_attached_shaders);
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &program_num_active_attributes);
	printf("program %d:\nLink Status: %s\nInfo log length: %d\nNumber of attached shaders: %d\nNumber of active attributes: %d\n",
		(int) program,
		status == GL_TRUE ? "GL_TRUE" : "GL_FALSE",
		(int) program_info_log_length,
		(int) program_num_attached_shaders,
		(int) program_num_active_attributes);
	free((void*)vertex_shader_text);
	free((void*)fragment_shader_text);
	return program;

}

int main(void)
{
	GLFWwindow *window;
	GLuint vbo;
	GLuint vao;
	GLuint program;
	GLuint pos_attrib;
	GLuint col_attrib;
	GLuint offsets_uniform;


	if (!glfwInit())
		return -1;
	
	window = glfwCreateWindow(640, 480, "Hello World!", NULL, NULL);
	glewExperimental = GL_TRUE;
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewInit();
	glfwSwapInterval(1); //Lock buffer swap to display refresh rate

	program = compile_shaders();
	pos_attrib = glGetAttribLocation(program, "position");
	col_attrib = glGetAttribLocation(program, "in_colour");

	printf("pos_attrib: %d\ncol_attrib: %d\n", pos_attrib, col_attrib);

	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(triangle_vertices[0]), 0);
	glVertexAttribPointer(col_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(triangle_vertices[0]), (GLvoid*)(3 * sizeof(float)));
	glEnableVertexAttribArray(col_attrib);
	glEnableVertexAttribArray(pos_attrib);
	
	offsets_uniform = glGetUniformLocation(program, "offsets");
	printf("offsets_uniform: %d\n", (int)offsets_uniform);

	const float offsets[] = {0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f};

	//glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);
		glUniform3fv(offsets_uniform, 2, offsets);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 3, 2);
		glUseProgram(0);
		glfwSwapBuffers(window);

		glfwPollEvents();

	}
	glfwTerminate();
	return 0;
}
