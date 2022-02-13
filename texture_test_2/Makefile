CFLAGS=-I../shared
CFLAGS+=-lGL -lGLEW -lglfw -lm
a.out : texture_test_2.o ../shared/shader_reader.o ../shared/shader_compiler.o /usr/local/lib/libSOIL.a input_functions.o
	${CC} ${CFLAGS} $^
a.out : texture_test_2.o ../shared/shader_reader.o ../shared/shader_compiler.o /usr/local/lib/libSOIL.a input_functions.o
