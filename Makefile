# Makefile
# KRY

CC=gcc
FLAGS=-Wall
BIN=kry
SRC=main.cpp
ZIP=xwilla00.zip

default:
	${CC} ${FLAGS} -o ${BIN} ${SRC}
	./${BIN}

clean:
	rm -f ${BIN} ${ZIP}

pack:
	zip -r ${ZIP} ${SRC} Makefile
