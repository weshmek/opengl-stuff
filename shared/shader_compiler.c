#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shader_compiler.h"
#include "shader_reader.h"

/*
 * Format string (copied from header so I don't need to keep opening it)
 * %[n](v|g)|(te)|(tc)|(f[nn])
 * n = number of source files comprising shader
 * %f = fragment shader
 * 	fragment shader is followed by nn (int, const char*) pairs
 * 	referring to colour attachment binding and variable name.
 * %v = vertex shader
 * %d = geometry shader
 * %tc = tesselation control shader
 * %te = tesselation geometry shader
 * return value = the program handle
 *
 * Examples
 * 	GLuint program = compile_shaders("%v%f", "assets/vertex_shader.vs", "assets/fragment_shader.fs", "out_colour");
 * 	GLuint program = compile_shaders("%2v%3f%2g", "assets/universal_header.sf", "assets/vertex_shader.vs", "assets/universal_header.sf", "assets/fragment_shader_1.fs", "assets/fragment_shader_2.fs", "out_colour", "assets/universal_header.sf", "assets/geometry_shader.gs");
 * 	GLuint program = compile_shaders("%f2", "assets/fragment_shader.fs", 0, "colour1", 1, "colour2")
 */

enum READ_STATE {
	READ_STATE_READ_BEGIN,
	READ_STATE_READING_NUMBER,
	READ_STATE_PCT_SIGN_READ,
	READ_STATE_READ_TESSELATION_SHADER,
	READ_STATE_POST_COMPILE_FRAGMENT_SHADER,
	NUM_READ_STATES,
};

static GLuint compile_single_shader(GLenum shader_type, int num_files, const char **shader_source_files)
{
	unsigned int i = 0;
	const char **shader_sources;
	GLint shader;

	/* for debugging */
	char *buffer;
	GLint status, shader_source_length, shader_info_log_length;

	printf("%s:%d shader_type = %s\n", __FILE__, __LINE__, shader_type == GL_GEOMETRY_SHADER ? "GL_GEOMETRY_SHADER" : shader_type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" : shader_type == GL_COMPUTE_SHADER ? "GL_COMPUTE_SHADER" : shader_type == GL_FRAGMENT_SHADER ? "GL_FRAGMENT_SHADER" : shader_type == GL_TESS_EVALUATION_SHADER ? "GL_TESS_EVALUATION_SHADER" : shader_type == GL_TESS_CONTROL_SHADER ? "GL_TESS_CONTROL_SHADER" : "UNKNOWN SHADER TYPE");

	shader = glCreateShader(shader_type);

	shader_sources = malloc(num_files * sizeof(const char *));

	for (i = 0; i < num_files; i++) {
		printf("shader file: %s\n", shader_source_files[i]);
		shader_sources[i] = read_shader(shader_source_files[i]);
		printf("shader source: %s\n", shader_sources[i]);
	}
	glShaderSource(shader, num_files, shader_sources, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &shader_info_log_length);
	glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &shader_source_length);
	printf("shader %d:\nCompile Status: %s\nInfo log length: %d\nSource length: %d\n",
		(int) shader,
		status == GL_TRUE ? "GL_TRUE" : "GL_FALSE",
		(int) shader_info_log_length,
		(int) shader_source_length);
	if (status != GL_TRUE){
		buffer = malloc(shader_info_log_length);
		glGetShaderInfoLog(shader, shader_info_log_length, NULL, buffer);
		printf("buffer: %s\n", buffer);
		free(buffer);
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

struct shader_program vcompile_shaders(const char *fmt, va_list v)
{

	GLint program;
	int num_shader_paths;
	int num_frag_data_binds;
	char c;
	enum READ_STATE state;
	const char **shader_sources;
	struct shader_program ret;
	/* for printing errors */
	GLint status, program_info_log_length, program_num_attached_shaders, program_num_active_attributes;
	char *buffer;

	memset(&ret, 0, sizeof(ret));

	state = READ_STATE_READ_BEGIN;
	program = glCreateProgram();
	if (program == 0)
		goto fail;

	while ((c = *fmt++)) {
		switch (state) {
			case READ_STATE_READ_BEGIN:
			{
				/*
				 * only valid character is %
				 */
				if (c == '%') {
					state = READ_STATE_PCT_SIGN_READ;
					num_shader_paths = 0;
					num_frag_data_binds = 0;
					continue;
				}
				goto fail;
			}
			case READ_STATE_PCT_SIGN_READ:
			case READ_STATE_READING_NUMBER:
			{
				/*
				 * next character can be a digit or [fgvct]
				 */
				if ((c <= '9') && (c >= '0')) {
					state = READ_STATE_READING_NUMBER;
					num_shader_paths = (num_shader_paths * 10) + (c - '0');
					continue;
				} else {
					/*
					 * finished reading digits; num_shader_paths is now known
					 */
					int i;

					if (num_shader_paths == 0)
						num_shader_paths = 1;
					shader_sources = malloc(num_shader_paths * sizeof(const char*));

					for (i = 0; i < num_shader_paths; i++)
						shader_sources[i] = va_arg(v, const char*);
				}
				if (c == 'f') {
					/*
					 * compile fragment shader
					 */

					ret.fragment_shader = compile_single_shader(GL_FRAGMENT_SHADER, num_shader_paths, shader_sources);

					glAttachShader(program, ret.fragment_shader);

					state = READ_STATE_POST_COMPILE_FRAGMENT_SHADER;
					continue;
				}
				if (c == 'g') {
					/*
					 * compile geometry shader
					 */

					ret.geometry_shader = compile_single_shader(GL_GEOMETRY_SHADER, num_shader_paths, shader_sources);

					glAttachShader(program, ret.geometry_shader);
					state = READ_STATE_READ_BEGIN;
					goto done_compiling_single_shader;
				}
				if (c == 'v') {
					/*
					 * compile vertex shader
					 */

					ret.vertex_shader = compile_single_shader(GL_VERTEX_SHADER, num_shader_paths, shader_sources);

					glAttachShader(program, ret.vertex_shader);
					state = READ_STATE_READ_BEGIN;
					goto done_compiling_single_shader;
				}
				if (c == 'c') {
					/*
					 * compile compute shader
					 */
					ret.compute_shader = compile_single_shader(GL_COMPUTE_SHADER, num_shader_paths, shader_sources);

					glAttachShader(program, ret.compute_shader);
					state = READ_STATE_READ_BEGIN;
					goto done_compiling_single_shader;
				}
				if (c == 't') {
					state = READ_STATE_READ_TESSELATION_SHADER;
					continue;
				}

				/*
				 * unrecognized shader type;
				 * clean up shader source list and fail
				 */
				goto cleanup_fail;
			}
			case READ_STATE_READ_TESSELATION_SHADER:
			{
				if (c == 'e') {
					/*
					 * compile tesselation evaluation shader
					 */
					ret.tesselation_evaluation_shader = compile_single_shader(GL_TESS_EVALUATION_SHADER, num_shader_paths, shader_sources);

					glAttachShader(program, ret.tesselation_evaluation_shader);
					state = READ_STATE_READ_BEGIN;
					goto done_compiling_single_shader;
				}
				if (c == 'c') {
					/*
					 * compile tesselation control shader
					 */
					ret.tesselation_control_shader = compile_single_shader(GL_TESS_CONTROL_SHADER, num_shader_paths, shader_sources);

					glAttachShader(program, ret.tesselation_control_shader);
					state = READ_STATE_READ_BEGIN;
					goto done_compiling_single_shader;
				}
				goto cleanup_fail;
			}
			case READ_STATE_POST_COMPILE_FRAGMENT_SHADER:
			{
				/*
				 * after reading fragment shader,
				 * bind fragment shader outputs to
				 * colours, if any
				 */
				if (c == '%') {
					int i;

					state = READ_STATE_PCT_SIGN_READ;
					num_shader_paths = 0;
					for (i = 0; i < num_frag_data_binds; i++) {
						int bind_point = va_arg(v, int);
						const char *bind_name = va_arg(v, const char*);

						glBindFragDataLocation(program, bind_point, bind_name);
						printf("Fragment shader colour attachment %d bound to %s\n", bind_point, bind_name);
					}
					num_frag_data_binds = 0;
					goto done_compiling_single_shader;
				} else if ((c <= '9') && (c >= '0')) {
					num_frag_data_binds = (num_frag_data_binds * 10) +  (c - '0');
					continue;
				}
				goto cleanup_fail;
			}
			default:
				goto fail;
		}
done_compiling_single_shader:
		free(shader_sources);
		continue;
cleanup_fail:
		free(shader_sources);
		goto fail;
	}

	if (state == READ_STATE_POST_COMPILE_FRAGMENT_SHADER) {
		/*
		 * after reading fragment shader,
		 * bind fragment shader outputs to
		 * colours, if any
		 */
		int i;

		for (i = 0; i < num_frag_data_binds; i++) {
			int bind_point = va_arg(v, int);
			const char *bind_name = va_arg(v, const char*);

			glBindFragDataLocation(program, bind_point, bind_name);
			printf("%s:%d - Fragment shader colour attachment %d bound to %s\n", __FILE__, __LINE__, bind_point, bind_name);
		}
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
	if (status != GL_TRUE){
		buffer = malloc(program_info_log_length);
		glGetProgramInfoLog(program, program_info_log_length, NULL, buffer);
		printf("buffer: %s\n", buffer);
		free(buffer);
		goto fail;
	}

	ret.program = program;
	glReleaseShaderCompiler();
	return ret;
fail:
	ret.program = 0;
	glReleaseShaderCompiler();
	return ret;
}

struct shader_program compile_shaders(const char *fmt, ...)
{
	va_list v;
	struct shader_program ret;

	va_start(v, fmt);
	ret = vcompile_shaders(fmt, v);
	va_end(v);
	return ret;
}
