#include "vertex_array_compiler.h"

/*
 * *compile*vertex_array
 * [[%e][%i][%f[n]][%l][%b][%a][%d]]
 * %e: GLuint, the name of the element buffer
 * $i: GLuint attribindex
 *     GLint size (1 to 4),
 *     GLenum type,
 *     GLuint relativeoffset
 * %f[n]: GLuint attribindex,
 *     GLint size (1 to 4),
 *     GLenum type,
 *     GLuint relativeoffset
 *     n present indicates normalized
 * %l: GLuint attribindex,
 *     GLint size (1 to 4),
 *     GLenum type,
 *     GLuint relativeoffset
 * %b: GLuint bindingindex,
 *     GLuint buffer,
 *     GLintptr offset,
 *     GLsizei stride
 * %a: GLuint attribindex,
 *     GLuint bindingindex
 * %d: GLuint bindingindex,
 *     GLuint divisor
 * %-: GLuint attibindex, index to disable
 * %+: GLuint attribindex, index to enable
 *
 * the *compile_bound_vertex_array calls should not affect the value of bound vertex array
 * calls to *compile*vertex_array should not affect any attributes not specified in the argument liss
 */


enum VERTEX_ARRAY_COMPILER_STATE {
	VERTEX_ARRAY_COMPILER_BEGIN,
	VERTEX_ARRAY_COMPILER_PCT_SIGN_READ,
	VERTEX_ARRAY_COMPILER_FLOAT_ARRAY_READ,
	VERTEX_ARRAY_COMPILER_NORMALIZED_FLOAT_ARRAY_READ,
	VERTEX_ARRAY_COMPILER_INT_ARRAY_READ,
	VERTEX_ARRAY_COMPILER_INTEGER_ARRAY_READ = VERTEX_ARRAY_COMPILER_INT_ARRAY_READ,
	VERTEX_ARRAY_COMPILER_DBL_ARRAY_READ,
	VERTEX_ARRAY_COMPILER_DOUBLE_ARRAY_READ = VERTEX_ARRAY_COMPILER_DBL_ARRAY_READ,
	VERTEX_ARRAY_COMPILER_BUFFER_BINDING_READ,
	VERTEX_ARRAY_COMPILER_ARRAY_ATTRIB_BINDING_READ,
	VERTEX_ARRAY_COMPILER_DIVISOR_READ,
	VERTEX_ARRAY_COMPILER_ELEMENT_BUFFER_BINDING_READ,
	VERTEX_ARRAY_COMPILER_DISABLE_ATTRIB_READ,
	VERTEX_ARRAY_COMPILER_ENABLE_ATTRIB_READ,
	NUM_VERTEX_ARRAY_COMPILER_STATES
};

int vcompile_vertex_array(const GLuint vao, const char *fmt, va_list v)
{
	enum VERTEX_ARRAY_COMPILER_STATE state;
	char c;
	int ret;

	state = VERTEX_ARRAY_COMPILER_BEGIN;
	ret = 0;

	c = *fmt;
	if (c == '%')
		state = VERTEX_ARRAY_COMPILER_PCT_SIGN_READ;
	else
		goto fail;

	do {
		c = *(fmt + 1);
		switch (state) {
			case VERTEX_ARRAY_COMPILER_BEGIN:
				/*
				 * should never be in this state
				 */
				if (c == '%') {
					state = VERTEX_ARRAY_COMPILER_PCT_SIGN_READ;
					continue;
				}
				goto fail;
			case VERTEX_ARRAY_COMPILER_PCT_SIGN_READ:
				if (c == 'i') {
					state = VERTEX_ARRAY_COMPILER_INT_ARRAY_READ;
					continue;
				}
				if (c == 'e') {
					state = VERTEX_ARRAY_COMPILER_ELEMENT_BUFFER_BINDING_READ;
					continue;
				}
				if (c == 'f') {
					state = VERTEX_ARRAY_COMPILER_FLOAT_ARRAY_READ;
					continue;
				}
				if (c == 'l') {
					state = VERTEX_ARRAY_COMPILER_DOUBLE_ARRAY_READ;
					continue;
				}
				if (c == 'b') {
					state = VERTEX_ARRAY_COMPILER_BUFFER_BINDING_READ;
					continue;
				}
				if (c == 'a') {
					state = VERTEX_ARRAY_COMPILER_ARRAY_ATTRIB_BINDING_READ;
					continue;
				}
				if (c == 'd') {
					state = VERTEX_ARRAY_COMPILER_DIVISOR_READ;
					continue;
				}
				if (c == '-') {
					state = VERTEX_ARRAY_COMPILER_DISABLE_ATTRIB_READ;
					continue;
				}
				if (c == '+') {
					state = VERTEX_ARRAY_COMPILER_ENABLE_ATTRIB_READ;
					continue;
				}
				goto fail;
			case VERTEX_ARRAY_COMPILER_INT_ARRAY_READ:
				{
					GLuint attribindex = va_arg(v, GLuint);
					GLint size = va_arg(v, GLint);
					GLenum type = va_arg(v, GLenum);
					GLuint relativeoffset = va_arg(v, GLuint);

					glVertexArrayAttribIFormat(vao, attribindex, size, type, relativeoffset);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_ELEMENT_BUFFER_BINDING_READ:
				{
					GLuint buffer = va_arg(v, GLuint);

					glVertexArrayElementBuffer(vao, buffer);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_DOUBLE_ARRAY_READ:
				{
					GLuint attribindex = va_arg(v, GLuint);
					GLint size = va_arg(v, GLint);
					GLenum type = va_arg(v, GLenum);
					GLuint relativeoffset = va_arg(v, GLuint);

					glVertexArrayAttribLFormat(vao, attribindex, size, type, relativeoffset);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_FLOAT_ARRAY_READ:
				if (c == 'n') {
					state = VERTEX_ARRAY_COMPILER_NORMALIZED_FLOAT_ARRAY_READ;
					continue;
				}
				{
					GLuint attribindex = va_arg(v, GLuint);
					GLint size = va_arg(v, GLint);
					GLenum type = va_arg(v, GLenum);
					GLuint relativeoffset = va_arg(v, GLuint);

					glVertexArrayAttribFormat(vao, attribindex, size, type, GL_FALSE, relativeoffset);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_BUFFER_BINDING_READ:
				{
					GLuint bindingindex = va_arg(v, GLuint);
					GLuint buffer = va_arg(v, GLuint);
					GLintptr offset = va_arg(v, GLintptr);
					GLsizei stride = va_arg(v, GLsizei);

					glVertexArrayVertexBuffer(vao, bindingindex, buffer, offset, stride);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_ARRAY_ATTRIB_BINDING_READ:
				{
					GLuint attribindex = va_arg(v, GLuint);
					GLuint bindingindex = va_arg(v, GLuint);

					glVertexArrayAttribBinding(vao, attribindex, bindingindex);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_DIVISOR_READ:
				{
					GLuint bindingindex = va_arg(v, GLuint);
					GLuint divisor = va_arg(v, GLuint);

					glVertexArrayBindingDivisor(vao, bindingindex, divisor);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_NORMALIZED_FLOAT_ARRAY_READ:
				{
					GLuint attribindex = va_arg(v, GLuint);
					GLint size = va_arg(v, GLint);
					GLenum type = va_arg(v, GLenum);
					GLuint relativeoffset = va_arg(v, GLuint);

					glVertexArrayAttribFormat(vao, attribindex, size, type, GL_TRUE, relativeoffset);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_DISABLE_ATTRIB_READ:
				{
					GLuint index = va_arg(v, GLuint);

					glDisableVertexArrayAttrib(vao, index);
					ret--;
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_ENABLE_ATTRIB_READ:
				{
					GLuint index = va_arg(v, GLuint);

					glEnableVertexArrayAttrib(vao, index);
					ret++;
					goto command_read_finish;
				}
			default:
				goto fail;

		}
command_read_finish:
		if (c == '%') {
			state = VERTEX_ARRAY_COMPILER_PCT_SIGN_READ;
			continue;
		}
		if (c == '\0')
			continue;
		goto fail;
	} while (*++fmt);
	return ret;
fail:
	return -1;
}

int compile_vertex_array(const GLuint vao, const char *fmt, ...)
{
	va_list v;
	int ret;

	va_start(v, fmt);
	ret = vcompile_vertex_array(vao, fmt, v);
	va_end(v);
	return ret;
}

int vcompile_bound_vertex_array(const char *fmt, va_list v)
{
	enum VERTEX_ARRAY_COMPILER_STATE state;
	char c;
	int ret;

	state = VERTEX_ARRAY_COMPILER_BEGIN;
	ret = 0;

	c = *fmt;
	if (c == '%')
		state = VERTEX_ARRAY_COMPILER_PCT_SIGN_READ;
	else
		goto fail;

	do {
		c = *(fmt + 1);
		switch (state) {
			case VERTEX_ARRAY_COMPILER_BEGIN:
				/*
				 * should never be in this state
				 */
				if (c == '%') {
					state = VERTEX_ARRAY_COMPILER_PCT_SIGN_READ;
					continue;
				}
				goto fail;
			case VERTEX_ARRAY_COMPILER_PCT_SIGN_READ:
				if (c == 'i') {
					state = VERTEX_ARRAY_COMPILER_INT_ARRAY_READ;
					continue;
				}
				if (c == 'e') {
					state = VERTEX_ARRAY_COMPILER_ELEMENT_BUFFER_BINDING_READ;
					continue;
				}
				if (c == 'f') {
					state = VERTEX_ARRAY_COMPILER_FLOAT_ARRAY_READ;
					continue;
				}
				if (c == 'l') {
					state = VERTEX_ARRAY_COMPILER_DOUBLE_ARRAY_READ;
					continue;
				}
				if (c == 'b') {
					state = VERTEX_ARRAY_COMPILER_BUFFER_BINDING_READ;
					continue;
				}
				if (c == 'a') {
					state = VERTEX_ARRAY_COMPILER_ARRAY_ATTRIB_BINDING_READ;
					continue;
				}
				if (c == 'd') {
					state = VERTEX_ARRAY_COMPILER_DIVISOR_READ;
					continue;
				}
				if (c == '-') {
					state = VERTEX_ARRAY_COMPILER_DISABLE_ATTRIB_READ;
					continue;
				}
				if (c == '+') {
					state = VERTEX_ARRAY_COMPILER_ENABLE_ATTRIB_READ;
					continue;
				}
				goto fail;
			case VERTEX_ARRAY_COMPILER_INT_ARRAY_READ:
				{
					GLuint attribindex = va_arg(v, GLuint);
					GLint size = va_arg(v, GLint);
					GLenum type = va_arg(v, GLenum);
					GLuint relativeoffset = va_arg(v, GLuint);

					glVertexAttribIFormat(attribindex, size, type, relativeoffset);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_ELEMENT_BUFFER_BINDING_READ:
				{
					GLuint buffer = va_arg(v, GLuint);

					/*
					 * OpenGL does not have a command to attach an element buffer
					 * to the currently-bound vertex array
					 */
					/*glVertexArrayElementBuffer(vao, buffer);*/
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_DOUBLE_ARRAY_READ:
				{
					GLuint attribindex = va_arg(v, GLuint);
					GLint size = va_arg(v, GLint);
					GLenum type = va_arg(v, GLenum);
					GLuint relativeoffset = va_arg(v, GLuint);

					glVertexAttribLFormat(attribindex, size, type, relativeoffset);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_FLOAT_ARRAY_READ:
				if (c == 'n') {
					state = VERTEX_ARRAY_COMPILER_NORMALIZED_FLOAT_ARRAY_READ;
					continue;
				}
				{
					GLuint attribindex = va_arg(v, GLuint);
					GLint size = va_arg(v, GLint);
					GLenum type = va_arg(v, GLenum);
					GLuint relativeoffset = va_arg(v, GLuint);

					glVertexAttribFormat(attribindex, size, type, GL_FALSE, relativeoffset);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_BUFFER_BINDING_READ:
				{
					GLuint bindingindex = va_arg(v, GLuint);
					GLuint buffer = va_arg(v, GLuint);
					GLintptr offset = va_arg(v, GLintptr);
					GLsizei stride = va_arg(v, GLsizei);

					glBindVertexBuffer(bindingindex, buffer, offset, stride);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_ARRAY_ATTRIB_BINDING_READ:
				{
					GLuint attribindex = va_arg(v, GLuint);
					GLuint bindingindex = va_arg(v, GLuint);

					glVertexAttribBinding(attribindex, bindingindex);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_DIVISOR_READ:
				{
					GLuint bindingindex = va_arg(v, GLuint);
					GLuint divisor = va_arg(v, GLuint);

					glVertexBindingDivisor(bindingindex, divisor);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_NORMALIZED_FLOAT_ARRAY_READ:
				{
					GLuint attribindex = va_arg(v, GLuint);
					GLint size = va_arg(v, GLint);
					GLenum type = va_arg(v, GLenum);
					GLuint relativeoffset = va_arg(v, GLuint);

					glVertexAttribFormat(attribindex, size, type, GL_TRUE, relativeoffset);
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_DISABLE_ATTRIB_READ:
				{
					GLuint index = va_arg(v, GLuint);

					glDisableVertexAttribArray(index);
					ret--;
					goto command_read_finish;
				}
			case VERTEX_ARRAY_COMPILER_ENABLE_ATTRIB_READ:
				{
					GLuint index = va_arg(v, GLuint);

					glEnableVertexAttribArray(index);
					ret++;
					goto command_read_finish;
				}
			default:
				goto fail;

		}
command_read_finish:
		if (c == '%') {
			state = VERTEX_ARRAY_COMPILER_PCT_SIGN_READ;
			continue;
		}
		if (c == '\0')
			continue;
		goto fail;

	} while (*++fmt);

	return ret;
fail:
	return -1;
}

int compile_bound_vertex_array(const char *fmt, ...)
{
	va_list v;
	int ret;

	va_start(v, fmt);
	ret = vcompile_bound_vertex_array(fmt, v);
	va_end(v);
	return ret;
}
