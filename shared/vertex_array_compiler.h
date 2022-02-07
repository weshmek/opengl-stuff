#ifndef __VERTEX_ARRAY_COMPILER_H__
#define __VERTEX_ARRAY_COMPILER_H__
#include <GL/glew.h>
#include <stdarg.h>

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
 * calls to *compile*vertex_array should not affect any attributes not specified in the argument list
 */

int vcompile_bound_vertex_array(const char *fmt, va_list v);
int compile_bound_vertex_array(const char *fmt, ...);
int vcompile_vertex_array(const GLuint vao, const char *fmt, va_list v);
int compile_vertex_array(const GLuint vao, const char *fmt, ...);
#endif
