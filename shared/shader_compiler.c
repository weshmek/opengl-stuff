#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "shader_compiler.h"
#include "shader_reader.h"

/*
 * Format string (copied from header so I don't need to keep opening it)
 * %[n](fvg)
 * n = number of source files comprising shader
 * %f = fragment shader (followed by 0 Data location in arg list)
 * %v = vertex shader
 * %d = geometry shader
 * return value = the program handle
 *
 * Examples
 * 	GLuint program = compile_shaders("%v%f", "assets/vertex_shader.vs", "assets/fragment_shader.fs", "out_colour");
 * 	GLuint program = compile_shaders("%2v%3f%2g", "assets/universal_header.sf", "assets/vertex_shader.vs", "assets/universal_header.sf", "assets/fragment_shader_1.fs", "assets/fragment_shader_2.fs", "out_colour", "assets/universal_header.sf", "assets/geometry_shader.gs");
 */

enum READ_STATE {
	READ_STATE_READ_BEGIN,
	READ_STATE_READING_NUMBER,
	READ_STATE_PCT_SIGN_READ,
	NUM_READ_STATES,
};

static GLuint compile_single_shader(GLenum shader_type, int num_files, va_list *v) {
				/* TODO */
				/* TODO */
				/* TODO */
	unsigned int i = 0;
	const char **shader_sources;
	const char *shader_source_file;
	GLint shader;

	/* for debugging */
	char buffer[512];
	GLint status, shader_source_length, shader_info_log_length;

	printf("%s %d shader_type = %s\n", __FILE__, __LINE__, shader_type == GL_GEOMETRY_SHADER ? "GL_GEOMETRY_SHADER" : shader_type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" : shader_type == GL_FRAGMENT_SHADER ? "GL_FRAGMENT_SHADER" : "UNKNOWN SHADER TYPE");
	num_files = num_files ? num_files : 1;
	shader = glCreateShader(shader_type);
	shader_sources = malloc(num_files * sizeof(const char *));
	for (i = 0; i < num_files; i++) {
		shader_source_file = va_arg(*v, const char *);
		printf("shader file: %s\n", shader_source_file);
		shader_sources[i] = read_shader(shader_source_file);
		printf("shader source: %s\n", shader_sources[i]);
	}
	glShaderSource(shader, num_files, shader_sources, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shader_info_log_length);
	glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &shader_source_length);
	glGetShaderInfoLog(shader, sizeof(buffer), NULL, buffer);
	printf("shader: %d:\nCompile Status: %s\nInfo log length: %d\nSource length: %d\n",
		(int) shader,
		status == GL_TRUE ? "GL_TRUE" : "GL_FALSE",
		(int) shader_info_log_length,
		(int) shader_source_length);
	if ((int)shader_info_log_length > 0) {
		printf("buffer: %s\n", buffer);
		goto fail;
	}
	for (i = 0; i < num_files; i++) {
		free((void*)shader_sources[i]);
	}
	free(shader_sources);

	return shader;

fail:
	for (i = 0; i < num_files; i++) {
		free((void*)shader_sources[i]);
	}
	free(shader_sources);
	
	return 0;
}

GLint compile_shaders(const char *fmt, ...)
{
	GLint program;
	va_list v;
	int num_shader_paths;
	char c;
	enum READ_STATE state;
	const char *shader_path;
	/* for printing errors */
	GLint status, program_info_log_length, program_num_attached_shaders, program_num_active_attributes;
	char buffer[512];

	va_start(v, fmt);
	state = READ_STATE_READ_BEGIN;
	program = glCreateProgram();
	if (program == 0)
		goto fail;

	while ((c = *fmt++)) {
		if (state == READ_STATE_READ_BEGIN) {
			/*
			 * only valid character is %
			 */
			if (c == '%') {
				state = READ_STATE_PCT_SIGN_READ;
				num_shader_paths = 0;
				continue;
			}
			goto fail;
		}
		if (state == READ_STATE_PCT_SIGN_READ || state == READ_STATE_READING_NUMBER) {
			/*
			 * next character can be a digit or fgv
			 */
			if ((c <= '9') && (c >= '0')) {
				state = READ_STATE_READING_NUMBER;
				num_shader_paths = num_shader_paths * 10 + (c - '0');
				continue;
			}
			if (c == 'f') {
				/*
				 * read fragment shader
				 */
				GLint fragment_shader;
				const char *fragment_shader_zero_bind;
				
				fragment_shader = compile_single_shader(GL_FRAGMENT_SHADER, num_shader_paths, &v);
				
				glAttachShader(program, fragment_shader);
				fragment_shader_zero_bind = va_arg(v, const char*);
				glBindFragDataLocation(program, 0, fragment_shader_zero_bind);
				printf("fragment_shader_zero_bind = %s\n", fragment_shader_zero_bind);

				state = READ_STATE_READ_BEGIN;
				continue;
			}
			if (c == 'g') {
				/*
				 * read geometry shader
				 */
				GLint geometry_shader;

				geometry_shader = compile_single_shader(GL_GEOMETRY_SHADER, num_shader_paths, &v);

				glAttachShader(program, geometry_shader);
				state = READ_STATE_READ_BEGIN;
				continue;
			}
			if (c == 'v') {
				/*
				 * read vertex shader
				 */
				GLint vertex_shader;

				vertex_shader = compile_single_shader(GL_VERTEX_SHADER, num_shader_paths, &v);
				
				glAttachShader(program, vertex_shader);
				state = READ_STATE_READ_BEGIN;
				continue;
			}
			goto fail;
		}

		goto fail;
	}

	
	glLinkProgram(program);
	
	/*
	 * For debug info
	 */
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &program_info_log_length);
	glGetProgramiv(program, GL_ATTACHED_SHADERS, &program_num_attached_shaders);
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &program_num_active_attributes);
	printf("program: %d\nLink Status: %s\nInfo log length: %d\nNumber of attached shaders: %d\nNumber of active attributes: %d\n",
		(int) program,
		status == GL_TRUE ? "GL_TRUE" : "GL_FALSE",
		(int) program_info_log_length,
		(int) program_num_attached_shaders,
		(int) program_num_active_attributes);

	va_end(v);
	return program;
fail:
	printf("%s %d", __FUNCTION__, __LINE__);
	va_end(v);
	return 0;
}
