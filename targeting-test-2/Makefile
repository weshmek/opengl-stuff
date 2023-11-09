CFLAGS=-I../shared
CFLAGS+=-lGL -lglfw -lm -g
OBJS=targeting-test-2.o ../shared/shader_reader.o ../shared/shader_compiler.o ../shared/vertex_array_compiler.o
DEPS=$(OBJS:.o=.d)
a.out : targeting-test-2.o ../shared/shader_reader.o ../shared/shader_compiler.o ../glew-2.1.0/lib/libGLEW.a ../shared/vertex_array_compiler.o
	${CC} ${CFLAGS} $^
a.out : targeting-test-2.o ../shared/shader_reader.o ../shared/shader_compiler.o ../glew-2.1.0/lib/libGLEW.a ../shared/vertex_array_compiler.o


%.d : %.c
	${CC} -I../shared -MM -MF $@ $^

depend : ${DEPS}

-include ${DEPS}
