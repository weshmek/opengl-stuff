#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shader_compiler.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SOIL.h>
#include "input_functions.h"

#define NUM_STOP_SIGN_POINTS 10

struct stop_sign_vertex {
	struct {
		float x;
		float y;
		float z;
	} position;
	struct {
		float r;
		float g;
		float b;
		float alpha;
	} colour;
	struct {
		float s;
		float t;
	} texcoords;
};

struct stop_sign_vertex *calculate_stop_sign_points(void) {
	struct stop_sign_vertex *vertices;
	int i;
	const int n = 8;

	vertices = malloc(NUM_STOP_SIGN_POINTS * sizeof(struct stop_sign_vertex));
	for (i = 1; i < NUM_STOP_SIGN_POINTS; i++) {
		float theta;

		theta = ((2.0f * M_PI * i) / n) + ((2.0f * M_PI) / (2.0f * n));
		vertices[i].position.y = cosf(theta);
		vertices[i].position.x = sinf(theta);
		vertices[i].position.z = 0.0f;
		vertices[i].colour.r = 1.0f;
		vertices[i].colour.g = 0.0f;
		vertices[i].colour.b = 0.0f;
		vertices[i].colour.alpha = 1.0f;
		vertices[i].texcoords.s = (0.55f * cosf(theta - (M_PI / 2.0f))) + 0.5f;
		//vertices[i].texcoords.s *= 2.0f;
		//vertices[i].texcoords.s += 0.5;
		vertices[i].texcoords.t = (1.5f * sinf(theta - (M_PI / 2.0f))) + 0.5f;
	}
		vertices[0].position.x = 0.0f;
		vertices[0].position.y = 0.0f;
		vertices[0].position.z = 0.0f;
		vertices[0].colour.r = 1.0f;
		vertices[0].colour.g = 0.0f;
		vertices[0].colour.b = 0.0f;
		vertices[0].colour.alpha = 1.0f;
		vertices[0].texcoords.s = 0.5f;
		//vertices[0].texcoords.s *= 2;
		//vertices[0].texcoords.s += 0.5;
		vertices[0].texcoords.t = 0.5f;

	return vertices;
}

struct stop_sign_vertex *calculate_back_face(void) {
	struct stop_sign_vertex *vertices;
	int i;
	const int n = 8;

	vertices = malloc(NUM_STOP_SIGN_POINTS * sizeof(struct stop_sign_vertex));
	for (i = 1; i < NUM_STOP_SIGN_POINTS; i++) {
		float theta;

		theta = ((2.0f * M_PI * i) / n) + ((2.0f * M_PI) / (2.0f * n));
		vertices[i].position.y = 1.15f * cosf(theta);
		vertices[i].position.x = 1.15f * sinf(theta);
		vertices[i].position.z = 0.0f;
		vertices[i].colour.r = 0.5f;
		vertices[i].colour.g = 0.5f;
		vertices[i].colour.b = 0.5f;
		vertices[i].colour.alpha = 1.0f;
		vertices[i].texcoords.s = 0.0f;
		//vertices[i].texcoords.s *= 2.0f;
		//vertices[i].texcoords.s += 0.5;
		vertices[i].texcoords.t = 0.0f;
	}
		vertices[0].position.x = 0.0f;
		vertices[0].position.y = 0.0f;
		vertices[0].position.z = 0.0f;
		vertices[0].colour.r = 0.5f;
		vertices[0].colour.g = 0.5f;
		vertices[0].colour.b = 0.5f;
		vertices[0].colour.alpha = 1.0f;
		vertices[0].texcoords.s = 0.0f;
		//vertices[0].texcoords.s *= 2;
		//vertices[0].texcoords.s += 0.5;
		vertices[0].texcoords.t = 0.0f;

	return vertices;
}
struct stop_sign_vertex *calculate_white_border(void) {
	struct stop_sign_vertex *vertices;
	int i;
	const int n = 8;

	vertices = malloc(NUM_STOP_SIGN_POINTS * sizeof(struct stop_sign_vertex));
	for (i = 1; i < NUM_STOP_SIGN_POINTS; i++) {
		float theta;

		theta = ((2.0f * M_PI * i) / n) + ((2.0f * M_PI) / (2.0f * n));
		vertices[i].position.y = 1.15f * cosf(theta);
		vertices[i].position.x = 1.15f * sinf(theta);
		vertices[i].position.z = 0.0f;
		vertices[i].colour.r = 1.0f;
		vertices[i].colour.g = 1.0f;
		vertices[i].colour.b = 1.0f;
		vertices[i].colour.alpha = 1.0f;
		vertices[i].texcoords.s = 0.0f;
		//vertices[i].texcoords.s *= 2.0f;
		//vertices[i].texcoords.s += 0.5;
		vertices[i].texcoords.t = 0.0f;
	}
		vertices[0].position.x = 0.0f;
		vertices[0].position.y = 0.0f;
		vertices[0].position.z = 0.0f;
		vertices[0].colour.r = 1.0f;
		vertices[0].colour.g = 1.0f;
		vertices[0].colour.b = 1.0f;
		vertices[0].colour.alpha = 1.0f;
		vertices[0].texcoords.s = 0.0f;
		//vertices[0].texcoords.s *= 2;
		//vertices[0].texcoords.s += 0.5;
		vertices[0].texcoords.t = 0.0f;

	return vertices;
}
float rotation_matrix_x[][4] =
	{{ 1.0f, 0.0f, 0.0f, 0.0f},
	 { 0.0f, 1.0f, 0.0f, 0.0f},
	 { 0.0f, 0.0f, 1.0f, 0.0f},
	 { 0.0f, 0.0f, 0.0f, 1.0f} };


float rotation_matrix_y[][4] =
	{{ 1.0f, 0.0f, 0.0f, 0.0f},
	 { 0.0f, 1.0f, 0.0f, 0.0f},
	 { 0.0f, 0.0f, 1.0f, 0.0f},
	 { 0.0f, 0.0f, 0.0f, 1.0f} };


float rotation_matrix_z[][4] =
	{{ 1.0f, 0.0f, 0.0f, 0.0f},
	 { 0.0f, 1.0f, 0.0f, 0.0f},
	 { 0.0f, 0.0f, 1.0f, 0.0f},
	 { 0.0f, 0.0f, 0.0f, 1.0f} };


GLuint rot_matrix_x_uniform;
GLuint rot_matrix_y_uniform;
GLuint rot_matrix_z_uniform;
int main(void)
{
	GLFWwindow *window;
	GLuint vbo;
	GLuint white_border_vbo;
	GLuint back_face_vbo;
	GLuint vao;
	GLuint white_border_vao;
	GLuint back_face_vao;
	GLuint texture;
	struct shader_program program;
	GLuint pos_attrib;
	GLuint col_attrib;
	GLuint tex_attrib;
	GLint  active_texture;

	int i;


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

	program = compile_shaders("%2v%f1", "assets/rotation_matrix.vs", "assets/vertex_shader.vs", "assets/fragment_shader.fs", 0, "out_colour");
	if (!program.program) {
		glfwTerminate();
		return -1;
	}

	pos_attrib = glGetAttribLocation(program.program, "position");
	col_attrib = glGetAttribLocation(program.program, "in_colour");
	tex_attrib = glGetAttribLocation(program.program, "texcoord");
	printf("pos_attrib: %d\ncol_attrib: %d\ntex_attrib: %d\n", pos_attrib, col_attrib, tex_attrib);
	
	rot_matrix_x_uniform = glGetUniformLocation(program.program, "rotation_matrix_x");
	rot_matrix_y_uniform = glGetUniformLocation(program.program, "rotation_matrix_y");
	rot_matrix_z_uniform = glGetUniformLocation(program.program, "rotation_matrix_z");
	printf("rot_matrix_x_uniform: %u\nrot_matrix_y_uniform: %u\nrot_matrix_z_uniform: %u\n", rot_matrix_x_uniform, rot_matrix_y_uniform, rot_matrix_z_uniform);

	{
		unsigned char *img;
		int width;
		int height;
		int channels;
		/* set up textures */
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		img = SOIL_load_image("assets/stop_text.png", &width, &height, &channels, SOIL_LOAD_RGBA);
		printf("w: %d, h: %d\n", width, height);
		/*for (i = 0; i < width * height; i++) {
			printf("r: %x g: %x b: %x a %x\n", img[(4 * i)], img[(4 * i) + 1], img[(4 * i) + 2], img[(4 * i) + 3]);
		}*/

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
		SOIL_free_image_data(img);

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &white_border_vbo);
	glGenBuffers(1, &back_face_vbo);
	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &white_border_vao);
	glGenVertexArrays(1, &back_face_vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	struct stop_sign_vertex *vertices = calculate_stop_sign_points();
	for (i = 0; i < NUM_STOP_SIGN_POINTS; i++) {
		printf("point %d: %f,%f,%f\n", i, vertices[i].position.x, vertices[i].position.y, vertices[i].position.z);
	}
	glBufferData(GL_ARRAY_BUFFER, NUM_STOP_SIGN_POINTS * sizeof(struct stop_sign_vertex), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(struct stop_sign_vertex), &(((struct stop_sign_vertex*)0x0)->position));
	glVertexAttribPointer(col_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(struct stop_sign_vertex), &(((struct stop_sign_vertex*)0x0)->colour));
	glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, sizeof(struct stop_sign_vertex), &(((struct stop_sign_vertex*)0x0)->texcoords));
	//glVertexAttribPointer(col_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(struct stop_sign_vertex), (GLvoid*)(3 * sizeof(float)));

	glEnableVertexAttribArray(col_attrib);
	glEnableVertexAttribArray(pos_attrib);
	glEnableVertexAttribArray(tex_attrib);

	free(vertices);

	glBindVertexArray(white_border_vao);
	glBindBuffer(GL_ARRAY_BUFFER, white_border_vbo);
	struct stop_sign_vertex *white_border_vertices = calculate_white_border();

	glBufferData(GL_ARRAY_BUFFER, NUM_STOP_SIGN_POINTS * sizeof(struct stop_sign_vertex), white_border_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(struct stop_sign_vertex), &(((struct stop_sign_vertex*)0x0)->position));
	glVertexAttribPointer(col_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(struct stop_sign_vertex), &(((struct stop_sign_vertex*)0x0)->colour));

	glEnableVertexAttribArray(col_attrib);
	glEnableVertexAttribArray(pos_attrib);

	free(white_border_vertices);

	glBindVertexArray(back_face_vao);
	glBindBuffer(GL_ARRAY_BUFFER, back_face_vbo);
	struct stop_sign_vertex *back_face_vertices = calculate_back_face();

	glBufferData(GL_ARRAY_BUFFER, NUM_STOP_SIGN_POINTS * sizeof(struct stop_sign_vertex), back_face_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(struct stop_sign_vertex), &(((struct stop_sign_vertex*)0x0)->position));
	glVertexAttribPointer(col_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(struct stop_sign_vertex), &(((struct stop_sign_vertex*)0x0)->colour));

	glEnableVertexAttribArray(col_attrib);
	glEnableVertexAttribArray(pos_attrib);

	free(back_face_vertices);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glEnable(GL_CULL_FACE);

	glGetIntegerv(GL_ACTIVE_TEXTURE, &active_texture);
	printf("active_texture = %d\n", active_texture);

	//glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program.program);
		//glDrawElements(GL_TRIANGLES, sizeof(stop_sign_elements) / sizeof(stop_sign_elements[0][0]), GL_UNSIGNED_INT, 0);
		glUniformMatrix4fv(rot_matrix_x_uniform, 1, GL_FALSE, rotation_matrix_x);
		glUniformMatrix4fv(rot_matrix_y_uniform, 1, GL_FALSE, rotation_matrix_y);
		glUniformMatrix4fv(rot_matrix_z_uniform, 1, GL_FALSE, rotation_matrix_z);

		glFrontFace(GL_CW);
		glBindVertexArray(white_border_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_STOP_SIGN_POINTS);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_STOP_SIGN_POINTS);

		glFrontFace(GL_CCW);
		glBindVertexArray(back_face_vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_STOP_SIGN_POINTS);
		

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;
}
