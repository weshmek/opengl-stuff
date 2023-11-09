#include "gl-info.h"

void get_gl_info(const GLubyte **renderer, const GLubyte **vendor, const GLubyte **version, const GLubyte **shading_language_version)
{

	*renderer = glGetString(GL_RENDERER);
	*vendor = glGetString(GL_VENDOR);
	*version = glGetString(GL_VERSION);
	*shading_language_version = glGetString(GL_SHADING_LANGUAGE_VERSION);

}
