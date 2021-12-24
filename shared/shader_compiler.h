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
 * Format string:
 * %[n](fvg)
 * n = number of source files comprising shader
 * %f = fragment shader
 * %v = vertex shader
 * %g = geometry shader
 * %c = compute shader
 * %te = tesselation evaluation shader
 * %tc = tesselation control shader
 */
struct shader_program compile_shaders(const char *fmt, ...);

#endif
