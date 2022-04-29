CFLAGS = -c
CC = gcc
LIBS =  -lm	#For Math Library
LIB = ../lib

all: server

server: server.o booking.o users.o function.o
	${CC} server.o booking.o users.o function.o -o server

booking.o: booking.c
	${CC} ${CFLAGS} booking.c

server.o: server.c
	${CC} ${CFLAGS} server.c

users.o: users.c
	${CC} ${CFLAGS} users.c

function.o: ${LIB}/function.c
	${CC} ${CFLAGS} ${LIB}/function.c

clean:
	rm -f *.o *~