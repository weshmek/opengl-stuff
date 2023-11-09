CFLAGS=-I../shared
CFLAGS+=-lGL -lglfw -lm -g
SHARED_HEADERS=$(wildcard ../shared/*.h)
SHARED_CFILES=$(wildcard ../shared/*.c)
OBJS=targeting-test-2.o ${SHARED_CFILES:.c=.o}
DEPS=$(OBJS:.o=.d) ${SHARED_CFILES:.c=.d}
a.out : ${OBJS}
	${CC} ${CFLAGS} $^
a.out : targeting-test-2.o ../shared/shader_reader.o ../shared/shader_compiler.o ../glew-2.1.0/lib/libGLEW.a ../shared/vertex_array_compiler.o


%.d : %.c
	${CC} -I../shared -MM -MF $@ $^

depend : ${DEPS}

-include ${DEPS}
