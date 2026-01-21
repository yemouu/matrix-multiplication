.POSIX:
CC = cc
INCLUDE_FLAGS = -Iinclude
CFLAGS += -g -Werror -Wall -Wextra $(INCLUDE_FLAGS)

matrix-multiplication: src/main.o src/matrix.o
	$(CC) $(CFLAGS) -o matrix-multiplication src/main.o src/matrix.o
src/main.o: src/main.c
src/matrix.o: src/matrix.c

clean:
	rm -f matrix-multiplication src/*.o
