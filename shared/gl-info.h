#ifndef __GL_INFO_H__
#define __GL_INFO_H__
#include <GL/glew.h>

void get_gl_info(const GLubyte **renderer, const GLubyte **vendor, const GLubyte **version, const GLubyte **shading_language_version);

#endif
