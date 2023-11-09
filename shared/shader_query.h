#ifndef __SHADER_QUERY_H__
#define __SHADER_QUERY_H__
#include <stdarg.h>
#include <GL/glew.h>

/*
 * %a: const char*, GLuint*
 * 	Reads a vertex attrib
 * %u: const char* GLuint*
 * 	Reads a uniform location
 */
int query_shader(GLuint program, const char *fmt, ...);
int vquery_shader(GLuint program, const char *fmt, va_list v);
#endif
