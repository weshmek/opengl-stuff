#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "info_print.h"
#include <stdio.h>
#include <stdlib.h>
#include "shader_compiler.h"
#include "print-int-value.h"
#include <math.h>
#include <string.h>

struct cube_vertex {
	struct {
		float x;
		float y;
		float z;
	} posn;

	struct {
		float r;
		float g;
		float b;
		float a;
	} col;
};

struct instance_state {
	int random_offset;
	int target_id;
};

#define my_offsetof(structure, element) \
	&(((structure *) 0x0) element)

#define cube_vertex_posn_offset \
	my_offsetof(struct cube_vertex, ->posn)
#define cube_vertex_col_offset \
	my_offsetof(struct cube_vertex, ->col)

static const struct cube_vertex spike_vertices[] = {
	{ .posn = { 0.0f, 0.0f, 0.0f }, .col = { 1.0f, 0.0f, 0.0f, 1.0f } },
	{ .posn = { 1.0f, 0.0f, 0.0f }, .col = { 1.0f, 0.0f, 0.0f, 1.0f } }, /* x line is red */

	{ .posn = { 0.0f, 0.0f, 0.0f }, .col = { 0.0f, 1.0f, 0.0f, 1.0f } },
	{ .posn = { 0.0f, 1.0f, 0.0f }, .col = { 0.0f, 1.0f, 0.0f, 1.0f } }, /* y line is green */

	{ .posn = { 0.0f, 0.0f, 0.0f }, .col = { 0.0f, 0.0f, 1.0f, 1.0f } },
	{ .posn = { 0.0f, 0.0f, 1.0f }, .col = { 0.0f, 0.0f, 1.0f, 1.0f } }  /* z line is blue */
};

#define RED { 1.0f, 0.0f, 0.0f, 0.5f }
#define GREEN { 0.0f, 1.0f, 0.0f, 1.0f }
#define BLUE { 0.0f, 0.0f, 1.0f, 1.0f }
#define YELLOW { 1.0f, 1.0f, 0.0f, 1.0f }
#define BLACK { 0.0f, 0.0f, 0.0f, 1.0f }
#define PINK { 1.0f, 0.0f, 1.0f, 1.0f }
#define WHITE {1.0f, 1.0f, 1.0f, 1.0f }

static const struct cube_vertex reticle_vertices[] = {
	{ { -1.0f, 0.0f, 5.0f }, WHITE },
	{ { 1.0f, 0.0f, 5.0f }, WHITE },
	{ { 0.0f, -1.0f, 5.0f }, WHITE },
	{ { 0.0f, 1.0f, 5.0f }, WHITE }

};

static const struct cube_vertex target_box_vertices[] = {
	{ { -11.0f, -11.0f, -11.0f }, GREEN },
	{ { 11.0f, -11.0f, -11.0f }, GREEN },
	{ { -11.0f, 11.0f, -11.0f }, GREEN },
	{ { 11.0f, 11.0f, -11.0f }, GREEN },
	{ { -11.0f, -11.0f, 11.0f }, GREEN },
	{ { 11.0f, -11.0f, 11.0f }, GREEN },
	{ { -11.0f, 11.0f, 11.0f }, GREEN },
	{ { 11.0f, 11.0f, 11.0f }, GREEN }
};

static const struct cube_vertex wall_vertices_for_elements[] = {
	/* faces */
/* 0 */		{ { -10.0f, 10.0f, 0.0f }, GREEN },
/* 1 */		{ { -10.0f, -10.0f, 0.0f }, GREEN },

/* 2 */		{ { 10.0f, 10.0f, 0.0f }, BLUE },
/* 3 */		{ { 10.0f, -10.0f, 0.0f }, BLUE },

/* 4 */		{ { 0.0f, 10.0f, -10.0f }, RED },
/* 5 */		{ { 0.0f, -10.0f, -10.0f }, RED },
	
/* 6 */		{ { 0.0f, 10.0f, 10.0f }, YELLOW },
/* 7 */		{ { 0.0f, -10.0f, 10.0f }, YELLOW },

	/* corners */
/* 8 */		{ { -10.0f, -10.0f, -10.0f }, BLACK },
/* 9 */		{ { -10.0f, -10.0f, 10.0f }, BLACK },
/* 10 */	{ { -10.0f, 10.0f, -10.0f }, BLACK },
/* 11 */	{ { -10.0f, 10.0f, 10.0f }, BLACK },
/* 12 */	{ { 10.0f, -10.0f, -10.0f }, BLACK },
/* 13 */	{ { 10.0f, -10.0f, 10.0f }, BLACK },
/* 14 */	{ { 10.0f, 10.0f, -10.0f }, BLACK },
/* 15 */	{ { 10.0f, 10.0f, 10.0f }, BLACK },

	/* top & bottom centres */
/* 16 */	{ { 0.0f, 10.0f, 0.0f }, WHITE },
/* 17 */	{ { 0.0f, -10.0f, 0.0f }, WHITE },
};

static const GLchar wall_elements[][3] = {
	{ 0, 9, 11 },
	{ 0, 1, 9 },
	{ 0, 10, 1 },
	{ 1, 10, 8 },

	{ 2, 15, 13 },
	{ 2, 13, 3 },
	{ 2, 3, 14 },
	{ 3, 12, 14 },

	{ 4, 14, 12 },
	{ 4, 12, 5 },
	{ 4, 5, 10 },
	{ 5, 8, 10 },

	{ 6, 13, 15 },
	{ 6, 7, 13 },
	{ 6, 11, 7 },
	{ 7, 11, 9 },

	{ 15, 2, 16 },
	{ 15, 16, 6 },
	{ 6, 16, 11 },
	{ 16, 0, 11 },
	{ 16, 4, 10 },
	{ 10, 0, 16 },
	{ 14, 4, 16 },
	{ 2, 14, 16 },
};

static const struct cube_vertex tetrahedron_vertices[] = {
/* 0 */		{ { 0.0f, 10.0f, 0.0f }, WHITE },
/* 1 */		{ { 10.0f * cosf(0.0f), -10.0f, 10.0f * sinf(0.0f) }, RED },
/* 2 */		{ { 10.0f * cosf((2.0f * M_PI) / 3.0f), -10.0f, 10.0f * sinf((2.0f * M_PI) / 3.0f) }, BLUE },
/* 3 */		{ { 10.0f * cosf((4.0f * M_PI) / 3.0f), -10.0f, 10.0f * sinf((4.0f * M_PI) / 3.0f) }, GREEN }
};

static const GLchar tetrahedron_elements[][3] = {
	{ 0, 1, 2 },
	{ 0, 2, 3 },
	{ 0, 3, 1 }
};
	
static struct {
	float x;
	float y;
	float z;
} camera_pos;

static struct {
	float theta;
	float phi;
} camera_look;

static float camera_scl;

static GLenum clip_mode = GL_ZERO_TO_ONE;
static GLenum clip_origin = GL_LOWER_LEFT;
static void (*depth_clamp_func)(GLenum) = glDisable;
static GLdouble near = 0.0f;
static GLdouble far = 1.0f;
static GLenum front_face = GL_CCW;
static GLenum cull_face_mode = GL_BACK;
static void (*cull_face_func)(GLenum) = glDisable;
static GLenum polygon_mode = GL_FILL;

static void (*depth_test_func)(GLenum) = glEnable;
static GLenum depth_func = GL_LESS;
static GLenum read_buffer = GL_COLOR_ATTACHMENT1;

enum POLYHEDRON {
	POLYHEDRON_CUBE = 0,
	POLYHEDRON_TETRA,
	POLYHEDRON_TETRAHEDRON = POLYHEDRON_TETRA,
	NUM_POLYHEDRA
};

enum POLYHEDRON polyhedron_to_draw = POLYHEDRON_CUBE;

enum TARGET_RENDER_MODE {
	TARGET_RENDER_MODE_OFF,
	TARGET_RENDER_MODE_SPIKE,
	TARGET_RENDER_MODE_RETICLE
};

enum TARGET_RENDER_MODE spike_render_mode = TARGET_RENDER_MODE_RETICLE;

#define check_error()\
	do {\
		int error;\
		do {\
			error = glGetError();\
			if (error != GL_NO_ERROR)\
				printf("%s:%d - error! %d\n", __FILE__, __LINE__, error);\
		} while (error != GL_NO_ERROR);\
	} while(0);

void camera_move_forward(void) {
	camera_pos.x += sinf(camera_look.phi);
	camera_pos.z += -cosf(camera_look.phi);
}

void camera_move_back(void) {
	camera_pos.x -= sinf(camera_look.phi);
	camera_pos.z -= -cosf(camera_look.phi);
}

void camera_move_left(void) {
	camera_pos.x += cosf(camera_look.phi);
	camera_pos.z += sinf(camera_look.phi);
}

void camera_move_right(void) {
	camera_pos.x -= cosf(camera_look.phi);
	camera_pos.z -= sinf(camera_look.phi);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_W:
			camera_move_forward();
			break;
		case GLFW_KEY_S:
			camera_move_back();
			break;
		case GLFW_KEY_A:
			camera_move_left();
			break;
		case GLFW_KEY_D:
			camera_move_right();
			break;
		case GLFW_KEY_Q:
			camera_pos.y += 1.0f;
			break;
		case GLFW_KEY_E:
			camera_pos.y -= 1.0f;
			break;
		case GLFW_KEY_M:
			if (clip_mode == GL_ZERO_TO_ONE)
				clip_mode = GL_NEGATIVE_ONE_TO_ONE;
			else if (clip_mode == GL_NEGATIVE_ONE_TO_ONE)
				clip_mode = GL_ZERO_TO_ONE;
			break;
		case GLFW_KEY_O:
			if (clip_origin == GL_LOWER_LEFT)
				clip_origin = GL_UPPER_LEFT;
			else if (clip_origin == GL_UPPER_LEFT)
				clip_origin = GL_LOWER_LEFT;
			break;
		case GLFW_KEY_UP:
			if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
				near += 0.01f;
			else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
				far += 0.01f;
			break;
		case GLFW_KEY_DOWN:
			if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
				near -= 0.01f;
			else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
				far -= 0.01f;
			break;
		case GLFW_KEY_C:
			if (depth_clamp_func == glEnable) {
				depth_clamp_func = glDisable;
			} else if (depth_clamp_func == glDisable) {
				depth_clamp_func = glEnable;
			}
			break;
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, 1);
			break;
		case GLFW_KEY_G:
			if (front_face == GL_CCW)
				front_face = GL_CW;
			else if (front_face == GL_CW)
				front_face = GL_CCW;
			break;
		case GLFW_KEY_H:
			if (cull_face_mode == GL_BACK)
				cull_face_mode = GL_FRONT_AND_BACK;
			else if (cull_face_mode == GL_FRONT_AND_BACK)
				cull_face_mode = GL_FRONT;
			else if (cull_face_mode == GL_FRONT)
				cull_face_mode = GL_BACK;
			break;
		case GLFW_KEY_J:
			if (cull_face_func == glEnable)
				cull_face_func = glDisable;
			else if (cull_face_func == glDisable)
				cull_face_func = glEnable;
			break;
		case GLFW_KEY_R:
			camera_pos.x = camera_pos.y = camera_pos.z = 0.0f;
			camera_scl = 1.0f;
			camera_look.phi = camera_look.theta = 0.0f;
			break;
		case GLFW_KEY_Z:
			if (depth_test_func == glDisable)
				depth_test_func = glEnable;
			else if (depth_test_func == glEnable)
				depth_test_func = glDisable;
			break;
		case GLFW_KEY_X:
			switch (depth_func) {
				case GL_LESS:
					depth_func = GL_LEQUAL;
					break;
				case GL_LEQUAL:
					depth_func = GL_EQUAL;
					break;
				case GL_EQUAL:
					depth_func = GL_GREATER;
					break;
				case GL_GREATER:
					depth_func = GL_GEQUAL;
					break;
				case GL_GEQUAL:
					depth_func = GL_NOTEQUAL;
					break;
				case GL_NOTEQUAL:
					depth_func = GL_NEVER;
					break;
				case GL_NEVER:
					depth_func = GL_ALWAYS;
					break;
				case GL_ALWAYS:
					depth_func = GL_LESS;
					break;
			}
			break;
		case GLFW_KEY_P:
			switch (polygon_mode) {
				case GL_FILL:
					polygon_mode = GL_LINE;
					break;
				case GL_LINE:
					polygon_mode = GL_POINT;
					break;
				case GL_POINT:
					polygon_mode = GL_FILL;
					break;
			}
			break;
		case GLFW_KEY_SPACE:
			switch (polyhedron_to_draw) {
				case POLYHEDRON_CUBE:
					polyhedron_to_draw = POLYHEDRON_TETRA;
					break;
				case POLYHEDRON_TETRAHEDRON:
					polyhedron_to_draw = POLYHEDRON_CUBE;
					break;
			}
			break;
		case GLFW_KEY_T:
			switch (spike_render_mode) {
			case TARGET_RENDER_MODE_OFF:
				spike_render_mode = TARGET_RENDER_MODE_SPIKE;
				break;
			case TARGET_RENDER_MODE_SPIKE:
				spike_render_mode = TARGET_RENDER_MODE_RETICLE;
				break;
			case TARGET_RENDER_MODE_RETICLE:
				spike_render_mode = TARGET_RENDER_MODE_OFF;
				break;
			}
			break;
		}
	}
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
	int width;
	int height;
	static double prev_xpos = 0.0f;
	static double prev_ypos = 0.0f;
	double dx = xpos - prev_xpos;
	double dy = ypos - prev_ypos;


	prev_xpos = xpos;
	prev_ypos = ypos;
	glfwGetWindowSize(window, &width, &height);

	camera_look.theta -= (float)2 * M_PI * dy / height;
	if (camera_look.theta > M_PI / 2.0f)
		camera_look.theta = M_PI / 2.0f;
	if (camera_look.theta < - M_PI / 2.0f)
		camera_look.theta = -M_PI / 2.0f;
	camera_look.phi -= 2 * M_PI * (float)dx / width;
	while (camera_look.phi < 0)
		camera_look.phi += 2 * M_PI;
	while (camera_look.phi >= 2 * M_PI)
		camera_look.phi -= 2 * M_PI;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	camera_scl += ((float) yoffset) * 0.01f;
}

void bind_vertices(GLuint vao, GLuint vbo, const void *pts, GLint size, GLuint pos_attrib, GLuint col_attrib)
{
	GLenum error;

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, pts, GL_STATIC_DRAW);
	glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, sizeof(struct cube_vertex), cube_vertex_posn_offset);
	glVertexAttribPointer(col_attrib, 4, GL_FLOAT, GL_FALSE, sizeof(struct cube_vertex), cube_vertex_col_offset);
	check_error();
	glEnableVertexAttribArray(pos_attrib);
	glEnableVertexAttribArray(col_attrib);
	error = glGetError();
	
	if (error != GL_NO_ERROR)
		printf("%s:%d - error! - %d\n", __FILE__, __LINE__, error);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	check_error();
}

static void buffer_vertex_data(GLuint vbo, const void *pts, GLint size)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, pts, GL_STATIC_DRAW);

	check_error();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static void bind_elements(GLuint ebo, const void *epts, GLint esize)
{
	GLenum error;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, esize, epts, GL_STATIC_DRAW);

	if (error != GL_NO_ERROR)
		printf("%s:%d - error! - %d\n", __FILE__, __LINE__, error);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void format_vertices(GLuint vao, GLuint pos_attrib, GLuint col_attrib)
{
	glBindVertexArray(vao);

	glVertexAttribFormat(pos_attrib, 3, GL_FLOAT, GL_FALSE, (GLint)(uintptr_t)cube_vertex_posn_offset);
	glVertexAttribFormat(col_attrib, 4, GL_FLOAT, GL_FALSE, (GLint)(uintptr_t)cube_vertex_col_offset);
	glEnableVertexAttribArray(pos_attrib);
	glEnableVertexAttribArray(col_attrib);

	glBindVertexArray(vao);
	check_error();
}

#define win_width 640
#define win_height 480

#define NUM_OBJECTS_TO_ANIMATE 300

const GLenum attachments[] = {
	GL_COLOR_ATTACHMENT0,
	GL_COLOR_ATTACHMENT1
};

int main(void)
{
	GLFWwindow *window;
	struct shader_program program;
	struct shader_program triangle_program;
	struct shader_program spike_program;
	struct shader_program reticle_program;
	GLuint pos_attrib, col_attrib, random_offset_attrib, target_id_attrib;
	GLuint reticle_pos_attrib, reticle_col_attrib;
	GLint z_offsets_uniform_location;
	GLint offsets_uniform;
	GLint p_uniform_location;
	GLint r_uniform_location;
	GLint s_uniform_location;
	GLint timer_uniform_location;
	GLint camera_pos_uniform_location;
	const GLubyte *renderer;
	const GLubyte *vendor;
	const GLubyte *version;
	const GLubyte *shading_language_version;
	int t;
	GLint num_inputs, num_outputs;
	struct instance_state instance_states[NUM_OBJECTS_TO_ANIMATE];
	int i;
	GLuint framebuffer;
	GLuint renderbuffers[3];
	char resource_name[10];
	GLenum framebuffer_status;

	GLuint spike_vao, spike_vbo;
	GLuint reticle_vao, reticle_vbo;
	GLuint r_uniform_location_spike;

	GLuint wall_vao, wall_vbo, wall_ebo, timer_bo, tetra_vbo, tetra_ebo, target_box_vbo;

	camera_pos.x = camera_pos.y = 0.0f;
	camera_pos.x = 26.0f;
	camera_pos.z = -0.0f;
	camera_look.theta = camera_look.phi = 0.0f;
	camera_scl = 1.0f;


	if (!glfwInit())
		return -1;
	
	for (i = 0; i < NUM_OBJECTS_TO_ANIMATE; i++) {
		instance_states[i].random_offset = random() % 50;
		instance_states[i].target_id = i + 1;
	}

	window = glfwCreateWindow(win_width, win_height, "Hello World!", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glewExperimental = GL_TRUE;
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glewInit();

	renderer = glGetString(GL_RENDERER);
	vendor = glGetString(GL_VENDOR);
	version = glGetString(GL_VERSION);
	shading_language_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

	glGenFramebuffers(1, &framebuffer);
	printf("framebuffer = %u\n", framebuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenRenderbuffers(3, renderbuffers);
	printf("renderbuffers = %u, %u %u\n", renderbuffers[0], renderbuffers[1], renderbuffers[2]);

	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffers[0]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, win_width, win_height);

	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffers[1]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_R32I, win_width, win_height);

	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffers[2]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, win_width, win_height);

	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
					GL_RENDERBUFFER, renderbuffers[0]);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
					GL_RENDERBUFFER, renderbuffers[1]);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
					GL_RENDERBUFFER, renderbuffers[2]);
	check_error();

	framebuffer_status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	printf("framebuffer_status = %s\n", framebuffer_status == GL_FRAMEBUFFER_COMPLETE ? "GL_FRAMEBUFFER_COMPLETE" :
						framebuffer_status == GL_FRAMEBUFFER_UNDEFINED ? "GL_FRAMEBUFFER_UNDEFINED" :
						framebuffer_status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT ? "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" :
						framebuffer_status == GL_FRAMEBUFFER_UNSUPPORTED ? "GL_FRAMEBUFFER_UNSUPPORTED" :
						framebuffer_status == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE ? "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" :
						framebuffer_status == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS ? "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" :
						"Unknown status");
	program = compile_shaders("%f2%v", "assets/fragment_shader.fs", 1, "out_colour", 0, "out_id", "assets/vertex_shader.vs");
	if (program.program == 0)
		return -1;
	pos_attrib = glGetAttribLocation(program.program, "position");
	col_attrib = glGetAttribLocation(program.program, "in_colour");
	random_offset_attrib = glGetAttribLocation(program.program, "random_offset");
	target_id_attrib = glGetAttribLocation(program.program, "target_id");
	printf("pos_attrib = %d\ncol_attrib = %d\nrandom_offset_attrib = %d\ntarget_id_attrib = %d\n", pos_attrib, col_attrib, random_offset_attrib, target_id_attrib);
	glGetProgramInterfaceiv(program.program, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &num_inputs);
	glGetProgramInterfaceiv(program.program, GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES, &num_outputs);
	printf("num_inputs = %d\nnum_outputs = %d\n", num_inputs, num_outputs);

	for (i = 0; i < num_inputs; i++) {
		GLsizei length;
		memset(resource_name, '\0', sizeof(resource_name));
		glGetProgramResourceName(program.program,
						GL_PROGRAM_INPUT, i, sizeof(resource_name) - 1,
						&length, resource_name);
		printf("resource %d = %s\n", i, resource_name);
	}
	glGenBuffers(1, &wall_vbo);
	glGenBuffers(1, &wall_ebo);
	glGenBuffers(1, &timer_bo);
	glGenBuffers(1, &tetra_vbo);
	glGenBuffers(1, &tetra_ebo);
	glGenBuffers(1, &target_box_vbo);
	glGenVertexArrays(1, &wall_vao);
	bind_elements(wall_ebo, wall_elements, sizeof(wall_elements));
	bind_elements(tetra_ebo, tetrahedron_elements, sizeof(tetrahedron_elements));
	format_vertices(wall_vao, pos_attrib, col_attrib);
	buffer_vertex_data(wall_vbo, wall_vertices_for_elements, sizeof(wall_vertices_for_elements));
	buffer_vertex_data(tetra_vbo, tetrahedron_vertices, sizeof(tetrahedron_vertices));
	buffer_vertex_data(target_box_vbo, target_box_vertices, sizeof(target_box_vertices));
	check_error();


	{
		/* bind vertex attribs to wall_vao */
		glBindVertexArray(wall_vao);
		
		glBindVertexBuffer(pos_attrib, wall_vbo, 0, sizeof(struct cube_vertex));
		glBindVertexBuffer(random_offset_attrib + pos_attrib + 1, tetra_vbo, 0, sizeof(struct cube_vertex));
		glBindVertexBuffer(random_offset_attrib + pos_attrib + 2, target_box_vbo, 0, sizeof(struct cube_vertex));
		glBindVertexArray(0);
		check_error();
	}
	{
		/* bind random offset attrib to wall_vao */
		glBindVertexArray(wall_vao);
		glVertexAttribIFormat(random_offset_attrib, 1, GL_INT, 0);
		glVertexAttribIFormat(target_id_attrib, 1, GL_INT, (GLuint)(uintptr_t)my_offsetof(struct instance_state, ->target_id));
		glBindVertexBuffer(random_offset_attrib, timer_bo, 0, sizeof(struct instance_state));
		glVertexAttribBinding(random_offset_attrib, random_offset_attrib);
		glVertexAttribBinding(target_id_attrib, random_offset_attrib);
		glEnableVertexAttribArray(random_offset_attrib);
		glEnableVertexAttribArray(target_id_attrib);
		glVertexBindingDivisor(random_offset_attrib, 1);
		glBindVertexArray(0);
		check_error();
	}

	glBindBuffer(GL_ARRAY_BUFFER, timer_bo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(instance_states), instance_states, GL_STATIC_DRAW);
	check_error();

	{
		/*
		 * prepare target reticle program
		 */
		glGenBuffers(1, &reticle_vbo);
		printf("reticle_vbo = %d\n", reticle_vbo);
		glGenVertexArrays(1, &reticle_vao);
		printf("reticle_vao = %d\n", reticle_vao);
		reticle_program = compile_shaders("%v%f1", "assets/reticle_vertex_shader.vs", "assets/reticle_fragment_shader.fs", 0, "out_col");

		reticle_pos_attrib = glGetAttribLocation(reticle_program.program, "vrt_pos");
		reticle_col_attrib = glGetAttribLocation(reticle_program.program, "vrt_col");
		printf("reticle_pos_attrib = %d\nreticle_col_attrib = %d\n", reticle_pos_attrib, reticle_col_attrib);
		buffer_vertex_data(reticle_vbo, reticle_vertices, sizeof(reticle_vertices));
		format_vertices(reticle_vao, reticle_pos_attrib, reticle_col_attrib);
		glBindVertexArray(reticle_vao);
		glBindVertexBuffer(reticle_pos_attrib, reticle_vbo, 0, sizeof(struct cube_vertex));
		glVertexAttribBinding(reticle_pos_attrib, reticle_pos_attrib);
		glVertexAttribBinding(reticle_col_attrib, reticle_pos_attrib);

		check_error();
	}
	spike_program = compile_shaders("%v%f1", "assets/spike_vertex_shader.vs", "assets/fragment_shader.fs", 0, "out_colour");
	if (spike_program.program == 0)
		return -1;
	pos_attrib = glGetAttribLocation(spike_program.program, "position");
	col_attrib = glGetAttribLocation(spike_program.program, "in_colour");
	printf("spike_program:\npos_attrib = %d\ncol_attrib = %d\n", pos_attrib, col_attrib);
	glGenBuffers(1, &spike_vbo);
	glGenVertexArrays(1, &spike_vao);
	check_error();
	bind_vertices(spike_vao, spike_vbo, spike_vertices, sizeof(spike_vertices), pos_attrib, col_attrib);


	z_offsets_uniform_location = glGetUniformLocation(program.program, "z_offsets");
	printf("z_offsets_uniform_location = %d\n", z_offsets_uniform_location);
	p_uniform_location = glGetUniformLocation(program.program, "P");
	printf("p_uniform_location = %d\n", p_uniform_location);
	r_uniform_location = glGetUniformLocation(program.program, "R");
	printf("r_uniform_location = %d\n", r_uniform_location);
	s_uniform_location = glGetUniformLocation(program.program, "S");
	printf("s_uniform_location = %d\n", s_uniform_location);
	camera_pos_uniform_location = glGetUniformLocation(program.program, "camera_pos");
	printf("camera_pos_uniform_location = %d\n", camera_pos_uniform_location);
	r_uniform_location_spike = glGetUniformLocation(spike_program.program, "R");
	printf("r_uniform_location_spike = %d\n", r_uniform_location_spike);
	timer_uniform_location = glGetUniformLocation(program.program, "timer");
	printf("timer_uniform_location = %d\n", timer_uniform_location);
	printf("Renderer: %s\nVendor: %s\nVersion: %s\nShading Language Version: %s\n", renderer, vendor, version, shading_language_version);
	check_error();

	const GLfloat z_offsets[] = {0.0f, 50.0f, -0.5f, -2.0f};
	const float offsets[] = {0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f};
	//glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
	
	t = 0;
	while (!glfwWindowShouldClose(window))
	{
		const float _p[] = {
			 1.0f, 0.0f, 0.0f, 0.0f ,
			 0.0f, 1.0f, 0.0f, 0.0f ,
			 0.0f, 0.0f, 1.0f, 0.0f ,
			 camera_pos.x, camera_pos.y, camera_pos.z, 1.0f 
		};
		
		const float _r[] = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cosf(camera_look.theta), sinf(camera_look.theta), 0.0f,
			0.0f, -sinf(camera_look.theta), cosf(camera_look.theta), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
			cosf(camera_look.phi), 0.0f, -sinf(camera_look.phi), 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			sinf(camera_look.phi), 0.0f, cosf(camera_look.phi), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		const float _s[] = {
			camera_scl, 0.0f, 0.0f, 0.0f,
			0.0f, camera_scl, 0.0f, 0.0f,
			0.0f, 0.0f, camera_scl, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		float pixel_readback[4];
		GLsizei num_elements_to_draw = 0;
		int targeted_object;


		check_error();
		depth_clamp_func(GL_DEPTH_CLAMP);
		cull_face_func(GL_CULL_FACE);
		depth_test_func(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDepthFunc(depth_func);
		glDepthMask(GL_TRUE);
		glFrontFace(front_face);
		glCullFace(cull_face_mode);
		glClipControl(clip_origin, clip_mode);
		glDrawBuffers(2, attachments);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthRange(near, far);
		glPolygonMode(GL_FRONT_AND_BACK, polygon_mode);
		check_error();
		/* put stuff here */
		glBindVertexArray(wall_vao);
		glUseProgram(program.program);
		glUniform1fv(z_offsets_uniform_location, 4, z_offsets);
		glUniform3fv(camera_pos_uniform_location, 1, (const float*) &camera_pos);
		glUniformMatrix4fv(p_uniform_location, 1, GL_FALSE, _p);
		glUniformMatrix4fv(r_uniform_location, 2, GL_FALSE, _r);
		glUniformMatrix4fv(s_uniform_location, 1, GL_FALSE, _s);
		glUniform1i(timer_uniform_location, t);
		check_error();
		if (polyhedron_to_draw == POLYHEDRON_CUBE) {
			glVertexArrayElementBuffer(wall_vao, wall_ebo);
			glVertexAttribBinding(pos_attrib, pos_attrib);
			glVertexAttribBinding(col_attrib, pos_attrib);
			num_elements_to_draw = sizeof(wall_elements) / sizeof(wall_elements[0][0]);
		} else if (polyhedron_to_draw == POLYHEDRON_TETRA) {
			glVertexArrayElementBuffer(wall_vao, tetra_ebo);
			glVertexAttribBinding(pos_attrib, pos_attrib + random_offset_attrib + 1);
			glVertexAttribBinding(col_attrib, pos_attrib + random_offset_attrib + 1);
			num_elements_to_draw = sizeof(tetrahedron_elements) / sizeof(tetrahedron_elements[0][0]);
		}
		check_error();
		glDrawElementsInstanced(GL_TRIANGLES, num_elements_to_draw, GL_UNSIGNED_BYTE, 0, NUM_OBJECTS_TO_ANIMATE);
		check_error();

		glReadBuffer(GL_COLOR_ATTACHMENT0);
		check_error();
		glReadPixels(win_width / 2, win_height / 2, 1, 1, GL_RED_INTEGER, GL_INT, &targeted_object);
		check_error();
		glReadBuffer(GL_COLOR_ATTACHMENT1);
		glReadPixels(win_width / 2, win_height / 2, 1, 1, GL_RGBA, GL_FLOAT, pixel_readback);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		if (targeted_object != 0) {
			glVertexAttribBinding(pos_attrib, pos_attrib + random_offset_attrib + 2);
			glVertexAttribBinding(col_attrib, pos_attrib + random_offset_attrib + 2);

			glDrawArraysInstancedBaseInstance(GL_POINTS, 0, sizeof(target_box_vertices) / sizeof(target_box_vertices[0]), 1, targeted_object - 1);

		}

		/* draw spike; clamping disabled */
		if (spike_render_mode == TARGET_RENDER_MODE_SPIKE) {
			glDisable(GL_DEPTH_CLAMP);
			glClipControl(clip_origin, GL_NEGATIVE_ONE_TO_ONE);
			glDisable(GL_DEPTH_TEST);
			glBindVertexArray(spike_vao);
			glUseProgram(spike_program.program);
			glUniformMatrix4fv(r_uniform_location_spike, 2, GL_FALSE, _r);
			glDrawBuffers(1, attachments + 1);
			glDrawArrays(GL_LINES, 0, sizeof(spike_vertices) / sizeof(struct cube_vertex)); 

		}

		if (spike_render_mode == TARGET_RENDER_MODE_RETICLE) {
			/*
			 * draw the reticle
			 */
			glBindVertexArray(reticle_vao);
			glUseProgram(reticle_program.program);
			glDrawBuffers(1, attachments + 1);
			glDrawArrays(GL_LINES, 0, sizeof(reticle_vertices) / sizeof(reticle_vertices[0]));
			glBindVertexArray(0);
			glUseProgram(0);
		}
		glReadBuffer(read_buffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		check_error();
		glBlitFramebuffer(0, 0, win_width, win_height, 0, 0, win_width, win_height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
		check_error();
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glfwSwapBuffers(window);


		glfwPollEvents();
		info_print("camera_pos: %f, %f, %f\n", camera_pos.x, camera_pos.y, camera_pos.z);
		info_print("camera_look: %f, %f\n", camera_look.theta, camera_look.phi);
		info_print("camera_scl: %f\n", camera_scl);
		info_print("Clip Control: origin = %s, mode = %s\n", clip_origin == GL_LOWER_LEFT ? "GL_LOWER_LEFT" : "GL_UPPER_LEFT", clip_mode == GL_ZERO_TO_ONE ? "GL_ZERO_TO_ONE" : "GL_NEGATIVE_ONE_TO_ONE");
		info_print("Depth Range: near = %f, far = %f\n", near, far);
		info_print("Depth Clamp: %s\n", depth_clamp_func == glEnable ? "Enabled" : "Disabled");
		info_print("Cull face is %s\n", cull_face_func == glEnable ? "enabled" : "disabled");
		info_print("Front Face: %s\n", front_face == GL_CW ? "GL_CW" : "GL_CCW");
		info_print("Cull Face: %s\n", cull_face_mode == GL_FRONT ? "GL_FRONT" : cull_face_mode == GL_BACK ? "GL_BACK" : "GL_FRONT_AND_BACK");
		info_print("Depth Test: %s\n", depth_test_func == glEnable ? "Enabled" : "Disabled");
		info_print("Depth Func: %s\n", depth_func == GL_LESS ? "GL_LESS" : depth_func == GL_LEQUAL ? "GL_LEQUAL" : depth_func == GL_EQUAL ? "GL_EQUAL" : depth_func == GL_GREATER ? "GL_GREATER" : depth_func == GL_GEQUAL ? "GL_GEQUAL" : depth_func == GL_NOTEQUAL ? "GL_NOTEQUAL" : depth_func == GL_NEVER ? "GL_NEVER" : depth_func == GL_ALWAYS ? "GL_ALWAYS" : "UNKNOWN");
		info_print("Polygon Mode: %s\n", polygon_mode == GL_FILL ? "GL_FILL" : polygon_mode == GL_LINE ? "GL_LINE" : polygon_mode == GL_POINT ? "GL_POINT" : "UNKNOWN");
		info_print("Centre pixel r: %f, g: %f, b: %f, a: %f\n", pixel_readback[0], pixel_readback[1], pixel_readback[2], pixel_readback[3]);
		info_print("polyhedron_to_draw = %s\n", polyhedron_to_draw == POLYHEDRON_CUBE ? "POLYHEDRON_CUBE" : polyhedron_to_draw == POLYHEDRON_TETRA ? "POLYHEDRON_TETRA" : "Unknown polyhedron");
		info_print("spike_render_mode = %s\n", spike_render_mode == TARGET_RENDER_MODE_RETICLE ? "TARGET_RENDER_MODE_RETICLE" : spike_render_mode == TARGET_RENDER_MODE_SPIKE ? "TARGET_RENDER_MODE_SPIKE" : spike_render_mode == TARGET_RENDER_MODE_OFF ? "TARGET_RENDER_MODE_OFF" : "UNKNOWN");
		info_print("t = %d\n", t);
		info_print("targeted_object = %d\n", targeted_object);


		if (!glfwWindowShouldClose(window))
			info_clear();
		t++;
	}
	glfwTerminate();
	return 0;
}
