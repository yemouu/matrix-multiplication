.POSIX:
CC = cc
INCLUDE_FLAGS = -Iinclude
CFLAGS += -g -Werror -Wall -Wextra $(INCLUDE_FLAGS)

matrix-multiplication: src/main.o src/matrix.o src/strassen.o
	$(CC) $(CFLAGS) -o matrix-multiplication src/main.o src/matrix.o src/strassen.o
src/main.o: src/main.c
src/strassen.o: src/strassen.c
src/matrix.o: src/matrix.c

clean:
	rm -f matrix-multiplication src/*.o
