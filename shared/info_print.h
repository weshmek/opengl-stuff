#ifndef __INFO_PRINT_H__
#define __INFO_PRINT_H__
#if defined(DISABLE_INFO_PRINT)
#define info_print(args...)
#define info_clear()
#else
/*
 * info_print only to be used in one file per program!
 * (because of dependance on __COUNTER__)
 */
#define info_print(args...) \
	do { \
		__COUNTER__; \
		printf(args); \
		/* ANSI terminal code to clear the line */ \
		printf("\x1B[K"); \
	} while (0);

#if 0
#define info_clear() \
	do { \
		int i; \
		for (i = 0; i < __COUNTER__; i++) \
		{ \
			printf("\x1B[F"); \
		} \
	} while (0);
#endif

/*
 * only use info_clear once per program!!
 * in same file as info_print
 */
#define info_clear() \
	do { \
		/* ANSI terminal code to move the cursor back __COUNTER__ lines */ \
		printf("\x1B[%dF", __COUNTER__); \
	} while (0);

#endif
#endif
