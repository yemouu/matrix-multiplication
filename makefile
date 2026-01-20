.POSIX:
CC = cc
INCLUDE_FLAGS = -Iinclude
CFLAGS += -Wall -Wextra -Werror $(INCLUDE_FLAGS)

matrix-multiplication: src/main.o src/matrix.o
	$(CC) $(CFLAGS) -o matrix-multiplication src/main.o src/matrix.o
src/main.o: src/main.c
src/matrix.o: src/matrix.c

clean:
	rm matrix-multiplication src/*.o
