CFLAGS = -c
CC = gcc
LIBS =  -lm #For Math Library
LIB = ../lib

all: client

client: client.o users.o booking.o function.o
	${CC} client.o users.o booking.o function.o -o client

client.o: client.c
	${CC} ${CFLAGS} client.c

users.o: users.c
	${CC} ${CFLAGS} users.c

booking.o: booking.c
	${CC} ${CFLAGS} booking.c

function.o: ${LIB}/function.c
	${CC} ${CFLAGS} ${LIB}/function.c

clean:
	rm -f *.o *~

