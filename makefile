.POSIX:
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iinclude

matrix-multiplication: src/main.o src/matrix.o
	$(CC) $(CFLAGS) -o matrix-multiplication src/main.o src/matrix.o
src/main.o: src/main.c
src/matrix.o: src/matrix.c

clean:
	rm matrix-multiplication src/*.o
