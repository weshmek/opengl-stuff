#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader_compiler.h"
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

	program = compile_shaders("%f%v", "assets/fragment_shader.fs", "out_colour", "assets/vertex_shader.vs");
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
