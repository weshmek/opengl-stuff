#include "shader_query.h"
#include <GL/glew.h>

enum SHADER_QUERY_STATE {
	SHADER_QUERY_BEGIN,
	SHADER_QUERY_PERCENT_SIGN_READ,
	SHADER_QUERY_ATTRIB_QUERY
};
int vquery_shader(const char *fmt, GLuint program, va_list v)
{
	enum SHADER_QUERY_STATE state;
	char c;

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
				/*
				 * command parsing goes here
				 */
				if (c == 'a') {
					state = SHADER_QUERY_ATTRIB_QUERY;
					continue;
				}
				goto fail;

			case SHADER_QUERY_ATTRIB_QUERY:
			{
				const char *name;
				GLint *dest;

				name = va_arg(v, const char *);
				dest = va_arg(v, GLint *);

				*dest = glGetAttribLocation(program, name);
				goto done_reading_command;
			}
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
	return 0;
fail:
	return -1;
}

int query_shader(const char *fmt, GLuint program, ...)
{
	va_list v;
	int ret;

	va_start(v, program);
	ret = vquery_shader(fmt, program, v);
	va_end(v);
	return ret;
}
