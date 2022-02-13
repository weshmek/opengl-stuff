#ifndef __ERROR_CHECK_H__
#define __ERROR_CHECK_H__

#define check_error()\
	do {\
		int error;\
		do {\
			error = glGetError();\
			if (error != GL_NO_ERROR)\
				printf("%s:%d - error! %d\n", __FILE__, __LINE__, error);\
		} while (error != GL_NO_ERROR);\
	} while(0)


#endif
