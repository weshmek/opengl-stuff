#include "shader_query.h"
#include <GL/glew.h>

enum SHADER_QUERY_STATE {
	SHADER_QUERY_BEGIN,
	SHADER_QUERY_PERCENT_SIGN_READ,
	SHADER_QUERY_ATTRIB_QUERY,
	SHADER_QUERY_UNIFORM_QUERY,
	SHADER_QUERY_NUMBERS_READ,

	NUM_SHADER_QUERY_STATES
};

int vquery_shader(GLuint program, const char *fmt, va_list v)
{
	enum SHADER_QUERY_STATE state;
	char c;
	int num_reps;
	unsigned int num_uniforms_queried, num_attribs_queried;

	num_uniforms_queried = num_attribs_queried = 0;
	state = SHADER_QUERY_BEGIN;
	c = *fmt;

	if (c == '%')
		state = SHADER_QUERY_PERCENT_SIGN_READ;
	else
		goto fail;

	do {
		c = *(fmt + 1);
		switch (state) {
			case SHADER_QUERY_BEGIN:
				/*
				 * should never be in this state
				 */
				goto fail;

			case SHADER_QUERY_PERCENT_SIGN_READ:
				num_reps = 0;
			case SHADER_QUERY_NUMBERS_READ:
				/*
				 * command parsing goes here
				 */
				if (c == 'a') {
					state = SHADER_QUERY_ATTRIB_QUERY;
					continue;
				}
				if (c == 'u') {
					state = SHADER_QUERY_UNIFORM_QUERY;
					continue;
				}
				if ((c <= '9') && (c >= '0')) {
					num_reps = (10 * num_reps) + (c - '0');
					state = SHADER_QUERY_NUMBERS_READ;
					continue;
				}
				goto fail;
			default:
				if (num_reps == 0)
					num_reps = 1;
				for (; num_reps > 0; num_reps--) {
					switch (state) {

						case SHADER_QUERY_ATTRIB_QUERY:
						{
							const char *name;
							GLint *dest;

							name = va_arg(v, const char *);
							dest = va_arg(v, GLint *);

							*dest = glGetAttribLocation(program, name);
							num_attribs_queried++;
							continue;
						}
						case SHADER_QUERY_UNIFORM_QUERY:
						{
							const char *name;
							GLint *dest;

							name = va_arg(v, const char *);
							dest = va_arg(v, GLint *);

							*dest = glGetUniformLocation(program, name);
							num_uniforms_queried++;
							continue;
						}
						default:
							goto fail;
					}
				}
				goto done_reading_command;
			}
done_reading_command:
		if (c == '%') {
			state = SHADER_QUERY_PERCENT_SIGN_READ;
			continue;
		}
		if (c == '\0') {
			continue;
		}
		goto fail;
	} while (*++fmt);
	return (num_uniforms_queried << 16) | num_attribs_queried;
fail:
	return -1;
}

int query_shader(GLuint program, const char *fmt, ...)
{
	va_list v;
	int ret;

	va_start(v, fmt);
	ret = vquery_shader(program, fmt, v);
	va_end(v);
	return ret;
}
