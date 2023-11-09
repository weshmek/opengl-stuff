CFLAGS=-I../shared
CFLAGS+=-lGL -lglfw -lm -g -lGLEW
SHARED_HEADERS=$(wildcard ../shared/*.h)
SHARED_CFILES=$(wildcard ../shared/*.c)
OBJS=targeting-test-2.o ${SHARED_CFILES:.c=.o}
DEPS=$(OBJS:.o=.d) ${SHARED_CFILES:.c=.d}
a.out : ${OBJS}
	${CC} ${CFLAGS} $^


%.d : %.c
	${CC} -I../shared -MM -MF $@ $^

depend : ${DEPS}

-include ${DEPS}
