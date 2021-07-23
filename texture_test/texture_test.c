#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "shader_compiler.h"
#include "SOIL.h"

struct triangle_vertex {
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
	float a;
//	float texcoords[2];
};
static const struct triangle_vertex triangle_vertices[] = {
	{ .x = 0.0f, .y = 0.5f, .z = 0.0f, .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f/*, 0.0f, 0.0f*/ },
	{ .x = 0.5f, .y = -0.5f, .z = 0.0f, .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f/*, 1.0f, 1.0f*/ },
	{ .x = -0.5f, .y = -0.5f, .z = 0.0f, .r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f/*, 1.0f, 0.0f*/}
};

int main(void)
{
	GLFWwindow *window;

	GLuint texture;

	GLuint vao;
	GLuint vbo;
	struct shader_program program;
	GLuint pos_attrib;
	GLuint col_attrib;
	GLuint tex_attrib;

	unsigned char *img;
	int width;
	int height;
	int channels;

	if (!glfwInit())
		return -1;
	
	window = glfwCreateWindow(1920, 1080, "Hello World!", NULL, NULL);
	glewExperimental = GL_TRUE;
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewInit();

	program = compile_shaders("%f%v", "assets/fragment_shader.fs", "out_colour", "assets/vertex_shader.vs");

	if (!program.program) {
		glfwTerminate();
		return -1;
	}
	pos_attrib = glGetAttribLocation(program.program, "position");
	col_attrib = glGetAttribLocation(program.program, "in_colour");
	//tex_attrib = glGetAttribLocation(program.program, "texcoord");
	printf("pos_attrib = %d\ncol_attrib = %d\ntex_attrib = %d\n", pos_attrib, col_attrib, tex_attrib);

/*
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	img = SOIL_load_image("assets/3x3rgb.bmp", &width, &height, &channels, SOIL_LOAD_RGBA);

	{
		int i;
		
		for (i = 0; i < width * height; i++) {
			printf("r:%x g:%x b:%x a:%x\n", img[(4 * i)], img[(4 * i) + 1], img[(4 * i) + 2], img[(4 * i) + 3]);
		}
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	
	SOIL_free_image_data(img);

	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
*/
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

	{
		int i;

		for (i = 0; i < sizeof(triangle_vertices) / sizeof(struct triangle_vertex); i++) {
			const struct triangle_vertex *tv = &triangle_vertices[i];
	
			//printf("%d pos: %f, %f, %f\ncol: %f, %f, %f, %f\n", i, tv->position[0], tv->position[1], tv->position[2], tv->colour[0], tv->colour[1], tv->colour[2], tv->colour[3]/*, tv->texcoords[0], tv->texcoords[1]*/);
			printf("%d pos: %f, %f, %f\ncol: %f, %f, %f, %f\n", i, tv->x, tv->y, tv->z, tv->r, tv->g, tv->b, tv->a);
		}
	}

	glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(struct triangle_vertex), &((struct triangle_vertex*)0)->x);
	glVertexAttribPointer(col_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(struct triangle_vertex), &((struct triangle_vertex*)0)->r);
	/*glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(struct triangle_vertex), &((struct triangle_vertex*)0)->texcoords);*/

	glEnableVertexAttribArray(pos_attrib);
	glEnableVertexAttribArray(col_attrib);

	//glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program.program);
		glfwSwapBuffers(window);

		glfwPollEvents();

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(0);

	}
	glfwTerminate();
	return 0;
}
