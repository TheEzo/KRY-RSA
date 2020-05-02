# Makefile
# KRY

CC=g++
FLAGS=-Wall -Wextra -pedantic -lgmpxx -lgmp
BIN=kry
SRC=src/main.cpp
ZIP=xwilla00.zip

default:
	${CC} ${FLAGS} -o ${BIN} ${SRC}

clean:
	rm -f ${BIN} ${ZIP}

pack:
	zip -r ${ZIP} ${SRC} Makefile
