#ifndef __SHADER_QUERY_H__
#define __SHADER_QUERY_H__
#include <stdarg.h>
#include <GL/glew.h>

/*
 * %a: const char*, GLuint*
 * 	Reads a vertex attrib
 */
int query_shader(const char *fmt, GLuint program, ...);
int vquery_shader(const char *fmt, GLuint program, va_list v);
#endif
