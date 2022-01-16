CFLAGS=-I../shared
CFLAGS+=-lGL -lglfw -lm -g
OBJS=targeting-test.o ../shared/shader_reader.o ../shared/shader_compiler.o
DEPS=$(OBJS:.o=.d)
a.out : targeting-test.o ../shared/shader_reader.o ../shared/shader_compiler.o ../glew-2.1.0/lib/libGLEW.a
	${CC} ${CFLAGS} $^
a.out : targeting-test.o ../shared/shader_reader.o ../shared/shader_compiler.o ../glew-2.1.0/lib/libGLEW.a


%.d : %.c
	${CC} -I../shared -MM -MF $@ $^

depend : ${DEPS}

-include ${DEPS}
