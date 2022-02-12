#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <vertex_array_compiler.h>
#include <error_check.h>

struct vertex {
	struct {
		float x;
		float y;
		float z;
	} pos;
	struct {
		float r;
		float g;
		float b;
		float alpha;
	} col;
	struct {
		float x;
		float y;
		float z;
	} normal;
};

int main(void)
{
	GLFWwindow *window;
	GLuint vao;
	GLuint vbo;
	int num_enabled;


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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	printf("%d\n", glIsVertexArray(vao) == GL_TRUE);
	printf("%d\n", glIsBuffer(vbo) == GL_TRUE);
	printf("vao = %u\nvbo = %u\n", vao, vbo);
	num_enabled = compile_vertex_array(vao, "%f%e", 1, 4, GL_FLOAT, offsetof(struct vertex, pos),
					  vbo);
	printf("num_enabled = %d\n", num_enabled);
	check_error();
	glBindVertexArray(vao);
	num_enabled = compile_bound_vertex_array("%f%f%+%+%+",
					2, 4, GL_FLOAT, offsetof(struct vertex, col),
					3, 4, GL_FLOAT, offsetof(struct vertex, normal),
					1, 2, 3);
	printf("num_enabled = %d\n", num_enabled);

/*
	//glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		//Put drawing code here
		glfwSwapBuffers(window);

		glfwPollEvents();

	}
*/
	glfwTerminate();
	return 0;
}
