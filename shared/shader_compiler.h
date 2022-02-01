#ifndef __SHADER_COMPILER_H__
#define __SHADER_COMPILER_H__
#include <GL/glew.h>

struct shader_program {
	GLint program;
	GLint vertex_shader;
	GLint geometry_shader;
	GLint fragment_shader;
	GLint compute_shader;
	GLint tesselation_control_shader;
	GLint tesselation_evaluation_shader;
	/* and more...*/

};
/*
 * Format string
 * {%[n]v|g|te|tc|f[nn]}+
 * n = number of source files comprising shader
 * %f = fragment shader
 * 	fragment shader is followed by nn (int, const char*) pairs
 * 	referring to colour attachment binding and variable name.
 * %v = vertex shader
 * %d = geometry shader
 * %tc = tesselation control shader
 * %te = tesselation geometry shader
 * return value = the program handle
 * if compilation fails, the program handle is 0
 *
 * Examples
 * 	GLuint program = compile_shaders("%v%f", "assets/vertex_shader.vs", "assets/fragment_shader.fs", "out_colour");
 * 	GLuint program = compile_shaders("%2v%3f%2g", "assets/universal_header.sf", "assets/vertex_shader.vs", "assets/universal_header.sf", "assets/fragment_shader_1.fs", "assets/fragment_shader_2.fs", "out_colour", "assets/universal_header.sf", "assets/geometry_shader.gs");
 * 	GLuint program = compile_shaders("%f2", "assets/fragment_shader.fs", 0, "colour1", 1, "colour2")
 */

struct shader_program compile_shaders(const char *fmt, ...);

#endif
