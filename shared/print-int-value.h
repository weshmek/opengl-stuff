#ifndef __PRINT_INT_VALUE_H__
#define __PRINT_INT_VALUE_H__
#define gl_print_int_value(name) \
	do { \
		GLint __value;\
		glGetIntegerv(name, &__value);\
		printf("%s = %d\n", #name, __value); \
	} while(0);


#endif
